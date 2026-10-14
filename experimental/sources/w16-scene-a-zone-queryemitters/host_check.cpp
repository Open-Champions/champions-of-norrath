// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
#define NORRATH_HOST_ARENA
#include <cstring>
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
static int leafLog[16], leafCount, leafFail = -1;
extern "C" int QueryEmittersLeaf__6VIZoneiR23VISceneEmitterQueryVars(unsigned char*, int node, unsigned char*) {
    leafLog[leafCount++] = node;
    return node == leafFail ? -1 : 7;
}
#include "query_emitters.cpp"
#include <cstdio>
static void setNode(unsigned i, float a, float b, float c, float d, unsigned front, unsigned back) {
    float f[4] = {a, b, c, d};
    std::memcpy(arena + 0x1000 + 24 * i, f, 16);
    std::memcpy(arena + 0x1000 + 24 * i + 16, &front, 4);
    std::memcpy(arena + 0x1000 + 24 * i + 20, &back, 4);
}
static void setVars(float x, float y, float z, float r) {
    float v[3] = {x, y, z};
    std::memcpy(arena + 0x2000 + 12, v, 12);
    std::memcpy(arena + 0x2000 + 24, &r, 4);
}
static int run(int node) { leafCount = 0; return QueryEmitters__6VIZoneiR23VISceneEmitterQueryVars(arena, node, arena + 0x2000); }
int main() {
    unsigned base = arena_base + 0x1000;
    std::memcpy(arena + 72, &base, 4);
    setNode(0, 1, 0, 0, -5, 0x40000001u, 0x40000002u);
    setNode(1, 0, 1, 0, 0, 0x40000003u, 0x40000004u);
    bool ok = true;
#define CK(c) do { if (!(c)) { std::printf("line %d\n", __LINE__); ok = false; } } while (0)
    setVars(10, 0, 0, 1); CK(run(0) == 0 && leafCount == 1 && leafLog[0] == 1);           // in front: only child +16
    setVars(0, 0, 0, 1);  CK(run(0) == 0 && leafCount == 1 && leafLog[0] == 2);           // behind: only child +20
    setVars(5.5f, 0, 0, 1); CK(run(0) == 0 && leafCount == 2 && leafLog[0] == 1 && leafLog[1] == 2);  // straddling, dist > 0: front then back
    setVars(4.5f, 0, 0, 1); CK(run(0) == 0 && leafCount == 2 && leafLog[0] == 2 && leafLog[1] == 1);  // straddling, dist < 0: back then front
    setVars(5, 0, 0, 1);    CK(run(0) == 0 && leafCount == 2 && leafLog[0] == 2 && leafLog[1] == 1);  // dist == 0: back first
    setVars(5, 0, 0, 0);    CK(run(0) == 0 && leafCount == 2 && leafLog[0] == 2 && leafLog[1] == 1);  // exactly on the plane with radius 0: strict tests, so both
    setVars(5.5f, 0, 0, 1); leafFail = 1; CK(run(0) == -1 && leafCount == 1);                       // first visit fails: second is skipped
    leafFail = 2;           CK(run(0) == -1 && leafCount == 2);                                     // second visit fails
    leafFail = -1;
    // flagged nodes: 0x4... goes straight to the leaf routine with the flags removed and returns its raw result; other flags are just masked
    CK(run(0x4000000a) == 7 && leafCount == 1 && leafLog[0] == 10);
    CK(run(static_cast<int>(0xf0000005u)) == 7 && leafLog[0] == 0x05);
    setVars(10, 0, 0, 1); CK(run(0x10000000) == 0 && leafLog[0] == 1);                          // node 0 after masking
    CK(run(static_cast<int>(0xa0000001u)) == 0 && leafCount == 2 && leafLog[0] == 4 && leafLog[1] == 3);   // node 1 after masking: y = 0 straddles its plane, back first
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
