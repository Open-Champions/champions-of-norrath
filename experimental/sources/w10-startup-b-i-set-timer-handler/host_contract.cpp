/* Host contract. Not a plan input. Records live in a host arena addressed by 32-bit
 * offsets; the callees are recording stubs. */
#define NORRATH_HOST_CONTRACT 1
#include "i_set_timer_handler.cpp"
#include <cstdio>
#include <vector>
alignas(16) static unsigned char arena[1 << 16];
extern "C" unsigned char* norrath_host_arena = arena;
extern "C" unsigned norrath_host_gp = 0xdead0001u;
extern "C" { TimerWords g_Timer = {0, -1, 1, 0, 0, 0, -1}; }
static std::vector<int> calls;
static std::vector<unsigned long long> args;
extern "C" unsigned long long iGetTimerSystemTime() { calls.push_back(1); return 0x1122334455667788ull; }
extern "C" void InsertAlarm(Counter* c) { calls.push_back(2); args.push_back(reinterpret_cast<unsigned char*>(c) - arena); }
extern "C" void UnlinkAlarm(Counter* c) { calls.push_back(4); args.push_back(reinterpret_cast<unsigned char*>(c) - arena); }
extern "C" void SetNextComp(unsigned long long s) { calls.push_back(3); args.push_back(s); }
static Counter& rec(unsigned at) { return *reinterpret_cast<Counter*>(arena + at); }
static void reset() { calls.clear(); args.clear(); }
int main() {
    int bad = 0;
    const unsigned at = 0x540;
    const int handle = static_cast<int>((at << 4) | 0x1b);
    rec(at).generation_word = 0x1b;
    // Refusals leave everything alone.
    rec(at).generation_word = 0x1a;
    if (iSetTimerHandler(handle, 5, 0x1000, 9) != static_cast<int>(0x80008002u) || !calls.empty() || rec(at).handler != 0) { std::puts("bad generation not refused"); ++bad; }
    rec(at).generation_word = 0x1b;
    if (iSetTimerHandler(handle | static_cast<int>(0x80000000u), 5, 0x1000, 9) != static_cast<int>(0x80008002u)) { std::puts("negative not refused"); ++bad; }
    g_Timer.running_handle = handle;
    if (iSetTimerHandler(handle, 5, 0x1000, 9) != static_cast<int>(0x80000010u) || !calls.empty()) { std::puts("running not refused"); ++bad; }
    g_Timer.running_handle = -1;
    // Install on a stopped counter: no insert.
    rec(at).flags = 0;
    reset();
    if (iSetTimerHandler(handle, 0x1122334499aabbccull, 0x2e0000, 0x77) != 0 || calls != std::vector<int>{1, 3} ||
        rec(at).flags != 2 || rec(at).handler != 0x2e0000 || rec(at).value != 0x1122334499aabbccull ||
        rec(at).handler_gp != 0xdead0001u || rec(at).handler_argument != 0x77 || args.size() != 1 || args[0] != 0x1122334455667788ull) { std::puts("install on stopped wrong"); ++bad; }
    // Reinstall on a started counter with a handler: unlink, store, insert, next-compare.
    rec(at).flags = 3; reset();
    if (iSetTimerHandler(handle, 11, 0x2e0004, 0x78) != 0 || calls != std::vector<int>{4, 2, 1, 3} ||
        rec(at).flags != 3 || rec(at).handler != 0x2e0004 || rec(at).value != 11 || args[0] != at || args[1] != at) { std::puts("reinstall wrong"); ++bad; }
    // Removal (zero handler) clears bit 1 and keeps the rest of the record.
    rec(at).flags = 0x13; reset();
    if (iSetTimerHandler(handle, 99, 0, 0x55) != 0 || calls != std::vector<int>{4, 1, 3} ||
        rec(at).flags != 0x11 || rec(at).handler != 0 || rec(at).value != 11 || rec(at).handler_argument != 0x78) { std::puts("removal wrong"); ++bad; }
    // Removal when no handler was installed: no unlink.
    rec(at).flags = 1; reset();
    if (iSetTimerHandler(handle, 99, 0, 0) != 0 || calls != std::vector<int>{1, 3} || rec(at).flags != 1) { std::puts("removal without handler wrong"); ++bad; }
    if (bad) return 1;
    std::puts("iSetTimerHandler contract passed");
    return 0;
}
