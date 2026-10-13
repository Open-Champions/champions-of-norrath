/* Host contract. Not a plan input. Records live in a host arena addressed by 32-bit
 * offsets; the three callees are recording stubs. */
#define NORRATH_HOST_CONTRACT 1
#include "i_start_timer_counter.cpp"
#include <cstdio>
#include <vector>
alignas(16) static unsigned char arena[1 << 16];
extern "C" unsigned char* norrath_host_arena = arena;
extern "C" { TimerWords g_Timer = {0, -1, 1, 0, 0, 0, -1}; }
static std::vector<int> calls;
static std::vector<unsigned long long> samples;
static unsigned stub_time_lo = 0x12345678u;
extern "C" unsigned long long iGetTimerSystemTime() { calls.push_back(1); return 0x1122334400000000ull | stub_time_lo; }
extern "C" void InsertAlarm(Counter* c) { calls.push_back(2); samples.push_back(static_cast<unsigned long long>(reinterpret_cast<unsigned char*>(c) - arena)); }
extern "C" void SetNextComp(unsigned long long s) { calls.push_back(3); samples.push_back(s); }
static Counter& rec(unsigned at) { return *reinterpret_cast<Counter*>(arena + at); }
int main() {
    int bad = 0;
    const unsigned at = 0x4c0;   // 64-byte aligned
    const int handle = static_cast<int>((at << 4) | 0x35);
    rec(at).generation_word = 0x35; rec(at).flags = 0; rec(at).start_time = 7;
    // Wrong low bits, negative handle.
    rec(at).generation_word = 0x37;
    if (iStartTimerCounter(handle) != static_cast<int>(0x80008002u) || !calls.empty() || rec(at).flags != 0) { std::puts("mismatch handle not refused"); ++bad; }
    rec(at).generation_word = 0x35;
    if (iStartTimerCounter(handle | static_cast<int>(0x80000000u)) != static_cast<int>(0x80008002u)) { std::puts("negative handle not refused"); ++bad; }
    // Handle currently running.
    g_Timer.running_handle = handle;
    if (iStartTimerCounter(handle) != static_cast<int>(0x80000010u) || !calls.empty() || rec(at).flags != 0) { std::puts("running handle not refused"); ++bad; }
    g_Timer.running_handle = -1;
    // Already started: result 1, no calls.
    rec(at).flags = 1;
    if (iStartTimerCounter(handle) != 1 || !calls.empty() || rec(at).start_time != 7) { std::puts("started not detected"); ++bad; }
    // Not started, no handler: time stored, bit 0 set, no alarm calls.
    rec(at).flags = 0x10;
    if (iStartTimerCounter(handle) != 0 || calls != std::vector<int>{1} || rec(at).flags != 0x11 || rec(at).start_time != 0x1122334412345678ull) { std::puts("start without handler wrong"); ++bad; }
    // Handler installed: insert then next-compare with the stored time.
    calls.clear(); samples.clear(); rec(at).flags = 2; stub_time_lo = 0x87654321u;
    if (iStartTimerCounter(handle) != 0 || calls != std::vector<int>{1, 2, 3} || rec(at).flags != 3 ||
        samples.size() != 2 || samples[0] != at || samples[1] != 0x1122334487654321ull) { std::puts("start with handler wrong"); ++bad; }
    if (bad) return 1;
    std::puts("iStartTimerCounter contract passed");
    return 0;
}
