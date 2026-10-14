// Host check only; not a build input. Record words hold arena offsets.
#define NORRATH_HOST_ARENA 1
alignas(16) static unsigned char arena[0x1000];
unsigned char* const norrath_host_arena_base = arena;
#include "erase.cpp"
#include <cstdio>
#include <cstring>
int main() {
    unsigned* pool = reinterpret_cast<unsigned*>(arena + 0x100);
    unsigned* ent = reinterpret_cast<unsigned*>(arena + 0x200);
    bool ok = true;
    for (int victim = 1; victim <= 3; ++victim) {
        std::memset(arena, 0x5a, sizeof arena);
        pool[0] = 3; pool[1] = 8; pool[2] = 0x200; pool[3] = 5; pool[4] = 1; pool[5] = 3;
        ent[3 * 1 + 1] = 2; ent[3 * 1 + 2] = 0xffffffffu;
        ent[3 * 2 + 1] = 3; ent[3 * 2 + 2] = 1;
        ent[3 * 3 + 1] = 0xffffffffu; ent[3 * 3 + 2] = 2;
        int r = Erase__t6VIPool1ZP15VIParticleMotifi(pool, victim);
        int next = victim == 1 ? 2 : victim == 2 ? 3 : -1;
        int prev = victim == 1 ? -1 : victim == 2 ? 1 : 2;
        if (r != (next == -1 ? 0x7fffffff : next)) ok = false;
        if (pool[0] != 2 || pool[3] != (unsigned)victim) ok = false;
        if (ent[3 * victim + 2] != 0xfffffffeu || ent[3 * victim + 1] != 5) ok = false;
        if (next != -1 && ent[3 * next + 2] != (unsigned)prev) ok = false;
        if (next == -1 && pool[5] != (unsigned)prev) ok = false;
        if (prev != -1 && ent[3 * prev + 1] != (unsigned)next) ok = false;
        if (prev == -1 && pool[4] != (unsigned)next) ok = false;
        if (pool[1] != 8 || pool[2] != 0x200) ok = false;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
