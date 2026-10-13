/* Host contract. Not a plan input. The three callees are recording stubs. The record
 * layout is the stated one only on a 32-bit target, so the host uses a local mirror
 * with the table pointer in a separate field and checks the logic, not the layout. */
#include <cstdio>
#include <vector>
#include <cstddef>
#include "raise_r.cpp"
static std::vector<long> events;
static void* table_after_init;
static int init_result;
extern "C" int _init_signal_r(Reent* r) { events.push_back(1); r->signal_handlers = static_cast<void (**)(int)>(table_after_init); return init_result; }
extern "C" int _getpid_r(Reent*) { events.push_back(2); return 4242; }
extern "C" int _kill_r(Reent*, int pid, int sig) { events.push_back(3); events.push_back(pid); events.push_back(sig); return 900 + sig; }
static void handler(int sig) { events.push_back(100 + sig); }
int main() {
    int bad = 0;
    void (*table[32])(int) = {};
    table_after_init = table;
    Reent r = {};
    r.signal_handlers = table;
    // Out of range.
    for (int sig : {32, 33, -1, 1000}) {
        r.error_number = 0; events.clear();
        if (_raise_r(&r, sig) != -1 || r.error_number != 22 || !events.empty()) { std::printf("range %d wrong\n", sig); ++bad; }
    }
    // Default: kill(getpid, sig), result passed through.
    events.clear(); r.error_number = 0;
    if (_raise_r(&r, 5) != 905 || events != std::vector<long>{2, 3, 4242, 5}) { std::puts("default wrong"); ++bad; }
    // Ignore.
    table[7] = reinterpret_cast<void (*)(int)>(1); events.clear();
    if (_raise_r(&r, 7) != 0 || !events.empty() || table[7] != reinterpret_cast<void (*)(int)>(1)) { std::puts("ignore wrong"); ++bad; }
    // Error marker.
    table[8] = reinterpret_cast<void (*)(int)>(0xffffffffu); r.error_number = 0;
    if (_raise_r(&r, 8) != 1 || r.error_number != 22 || !events.empty()) { std::puts("error marker wrong"); ++bad; }
    // Function: cleared before the call, called with the signal, result 0.
    table[9] = handler; events.clear();
    if (_raise_r(&r, 9) != 0 || table[9] != nullptr || events != std::vector<long>{109}) { std::puts("function wrong"); ++bad; }
    // Table created on demand; failure returns -1 without further calls.
    r.signal_handlers = nullptr; init_result = 0; events.clear();
    table[3] = handler;
    if (_raise_r(&r, 3) != 0 || events != std::vector<long>{1, 103}) { std::puts("lazy init wrong"); ++bad; }
    r.signal_handlers = nullptr; init_result = -1; events.clear();
    if (_raise_r(&r, 3) != -1 || events != std::vector<long>{1}) { std::puts("init failure wrong"); ++bad; }
    if (bad) return 1;
    std::puts("_raise_r contract passed");
    return 0;
}
