// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
alignas(16) static unsigned char arena[0x2000];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "erase.cpp"
#include <cstdio>
#include <cstring>
static void* deleted;
static int dels;
extern "C" void __builtin_delete(void* p) { deleted = p; ++dels; }
static unsigned addr(unsigned off) { return arena_base + off; }
int main() {
    unsigned* L = reinterpret_cast<unsigned*>(arena + 0x10);
    unsigned* nodes[3] = {reinterpret_cast<unsigned*>(arena + 0x100), reinterpret_cast<unsigned*>(arena + 0x200), reinterpret_cast<unsigned*>(arena + 0x300)};
    unsigned nodeAddr[3] = {addr(0x100), addr(0x200), addr(0x300)};
    bool ok = true;
    // chain a<->b<->c, head a, tail c, count 3; erase each in turn
    for (int victim = 0; victim < 3; ++victim) {
        std::memset(arena, 0x5a, sizeof arena);
        L[0] = 3; L[1] = nodeAddr[0]; L[2] = nodeAddr[2];
        nodes[0][1] = nodeAddr[1]; nodes[0][2] = 0;
        nodes[1][1] = nodeAddr[2]; nodes[1][2] = nodeAddr[0];
        nodes[2][1] = 0; nodes[2][2] = nodeAddr[1];
        dels = 0;

        int r = Erase__t6VIList1ZP10VIFlatFilei(L, nodeAddr[victim]);
        unsigned next = victim < 2 ? nodeAddr[victim + 1] : 0;
        unsigned prev = victim > 0 ? nodeAddr[victim - 1] : 0;
        if (r != (next ? (int)next : 0x7fffffff)) ok = false;

        if (dels != 1 || deleted != nodes[victim] || L[0] != 2) ok = false;
        if (next) { if (nodes[victim + 1][2] != prev) ok = false; } else if (L[2] != prev) ok = false;
        if (prev) { if (nodes[victim - 1][1] != next) ok = false; } else if (L[1] != next) ok = false;
    }
    // sole node
    std::memset(arena, 0x5a, sizeof arena);
    L[0] = 1; L[1] = nodeAddr[0]; L[2] = nodeAddr[0]; nodes[0][1] = 0; nodes[0][2] = 0;

    if (Erase__t6VIList1ZP10VIFlatFilei(L, nodeAddr[0]) != 0x7fffffff || L[0] != 0 || L[1] != 0 || L[2] != 0) ok = false;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
