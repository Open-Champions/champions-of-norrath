// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
alignas(16) static unsigned char arena[0x2000];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "next.cpp"
#include <cstdio>
int main() {
    unsigned* pool = reinterpret_cast<unsigned*>(arena + 0x10);
    pool[2] = arena_base + 0x100;
    int values[5] = {3, -1, 0, 0x7ffffffe, -2};
    for (int i = 0; i < 5; ++i) *reinterpret_cast<int*>(arena + 0x100 + i * 12 + 4) = values[i];
    bool ok = true;
    for (int i = 0; i < 5; ++i) {
        int want = values[i] == -1 ? 0x7fffffff : values[i];
        if (Next__Ct6VIPool1ZP12VICollBufferi(pool, i) != want) ok = false;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
