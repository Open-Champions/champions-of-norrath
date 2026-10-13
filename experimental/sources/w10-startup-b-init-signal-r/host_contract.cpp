#include <cstdio>
#include <cstring>
#include "init_signal_r.cpp"
static unsigned pool[40];
static unsigned wanted_size; static void* give;
extern "C" void* _malloc_r(Reent*, unsigned n) { wanted_size = n; return give; }
int main() {
    int bad = 0;
    Reent r; std::memset(&r, 0, sizeof r);
    // Existing table: nothing happens.
    unsigned other[32]; r.signal_handlers = other; wanted_size = 0; give = pool;
    if (_init_signal_r(&r) != 0 || r.signal_handlers != other || wanted_size != 0) { std::puts("existing table wrong"); ++bad; }
    // New table: 128 bytes requested, 32 words cleared, 33rd word untouched.
    std::memset(pool, 0xff, sizeof pool); r.signal_handlers = nullptr;
    if (_init_signal_r(&r) != 0 || r.signal_handlers != pool || wanted_size != 128) { std::puts("new table wrong"); ++bad; }
    for (int i = 0; i < 32; ++i) if (pool[i] != 0) { std::puts("word not cleared"); ++bad; break; }
    if (pool[32] != 0xffffffffu) { std::puts("cleared too much"); ++bad; }
    // Allocation failure: -1 and the null pointer stays stored.
    give = nullptr; r.signal_handlers = nullptr;
    if (_init_signal_r(&r) != -1 || r.signal_handlers != nullptr) { std::puts("failure wrong"); ++bad; }
    if (bad) return 1;
    std::puts("_init_signal_r contract passed");
    return 0;
}
