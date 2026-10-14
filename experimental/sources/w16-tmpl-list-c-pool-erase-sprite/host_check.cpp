// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
alignas(16) static unsigned char arena[0x2000];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "erase.cpp"
#include <cstdio>
#include <cstring>
int main() {
    unsigned* pool = reinterpret_cast<unsigned*>(arena + 0x10);
    unsigned* ent = reinterpret_cast<unsigned*>(arena + 0x100);   // entries of 3 words
    bool ok = true;
    // live chain 1 <-> 2 <-> 3, free chain head 5, count 3; erase each in turn
    for (int victim = 1; victim <= 3; ++victim) {
        std::memset(arena, 0x5a, sizeof arena);
        pool[0] = 3; pool[1] = 8; pool[2] = arena_base + 0x100; pool[3] = 5; pool[4] = 1; pool[5] = 3;
        ent[3 * 1 + 1] = 2; ent[3 * 1 + 2] = 0xffffffffu;
        ent[3 * 2 + 1] = 3; ent[3 * 2 + 2] = 1;
        ent[3 * 3 + 1] = 0xffffffffu; ent[3 * 3 + 2] = 2;
        int r = Erase__t6VIPool1ZP8VISpritei(pool, victim);
        int next = victim == 1 ? 2 : victim == 2 ? 3 : -1;
        int prev = victim == 1 ? -1 : victim == 2 ? 1 : 2;
        if (r != (next == -1 ? 0x7fffffff : next)) ok = false;
        if (pool[0] != 2 || pool[3] != (unsigned)victim) ok = false;
        if (ent[3 * victim + 2] != 0xfffffffeu || ent[3 * victim + 1] != 5) ok = false;
        if (next != -1 && ent[3 * next + 2] != (unsigned)prev) ok = false;
        if (next == -1 && pool[5] != (unsigned)prev) ok = false;
        if (prev != -1 && ent[3 * prev + 1] != (unsigned)next) ok = false;
        if (prev == -1 && pool[4] != (unsigned)next) ok = false;
        if (pool[1] != 8 || pool[2] != arena_base + 0x100) ok = false;
    }
    // sole entry
    std::memset(arena, 0x5a, sizeof arena);
    pool[0] = 1; pool[1] = 8; pool[2] = arena_base + 0x100; pool[3] = 0xffffffffu; pool[4] = 1; pool[5] = 1;
    ent[3] = 0; ent[4] = 0xffffffffu; ent[5] = 0xffffffffu;
    if (Erase__t6VIPool1ZP8VISpritei(pool, 1) != 0x7fffffff || pool[0] != 0 || pool[3] != 1 || pool[4] != 0xffffffffu || pool[5] != 0xffffffffu) ok = false;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
