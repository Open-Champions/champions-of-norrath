// Host check only; not a build input. Record words hold arena offsets (see NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include "erase.cpp"
#include <cstdio>
#include <cstring>
alignas(16) static unsigned char arena[0x1000];
unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
static int endCalls;
extern "C" int End__Ct6VIPool1ZP20VIParticleDefinition(const void* pool) { ++endCalls; return pool == arena + 0x100 ? 0x7fffffff : -3; }
int main() {
    unsigned* pool = reinterpret_cast<unsigned*>(arena + 0x100);
    unsigned* ent = reinterpret_cast<unsigned*>(arena + 0x200);   // entries of 3 words
    bool ok = true;
    // chain 1 <-> 2 <-> 3 (indices), free chain head 5, count 3; erase each in turn
    for (int victim = 1; victim <= 3; ++victim) {
        std::memset(arena, 0x5a, sizeof arena);
        pool[0] = 3; pool[1] = 8; pool[2] = 0x200; pool[3] = 5; pool[4] = 1; pool[5] = 3;
        ent[3 * 1 + 1] = 2; ent[3 * 1 + 2] = 0xffffffffu;
        ent[3 * 2 + 1] = 3; ent[3 * 2 + 2] = 1;
        ent[3 * 3 + 1] = 0xffffffffu; ent[3 * 3 + 2] = 2;
        endCalls = 0;
        int r = Erase__t6VIPool1ZP20VIParticleDefinitioni(pool, victim);
        int next = victim == 1 ? 2 : victim == 2 ? 3 : -1;
        int prev = victim == 1 ? -1 : victim == 2 ? 1 : 2;
        if (r != (next == -1 ? 0x7fffffff : next) || endCalls != (next == -1)) ok = false;
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
