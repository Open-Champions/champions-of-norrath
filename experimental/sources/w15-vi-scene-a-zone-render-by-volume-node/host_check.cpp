// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
#include "render.cpp"
#include <cstdint>
#include <cstring>
alignas(16) static unsigned char arena[0x8000];
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return reinterpret_cast<unsigned char*>((reinterpret_cast<uintptr_t>(arena) & ~uintptr_t{0xffffffffu}) | a); }
extern "C" void* norrath_host_function(unsigned) { return nullptr; }
static unsigned word_of(const void* p) { return static_cast<unsigned>(reinterpret_cast<uintptr_t>(p)); }
static void setw(unsigned char* field, int v) { std::memcpy(field, &v, 4); }
static void setu(unsigned char* field, unsigned v) { std::memcpy(field, &v, 4); }
static void setf(unsigned char* field, float v) { std::memcpy(field, &v, 4); }
static int leafCalls, leafIds[8], leafResult; static unsigned char* leafZone[8]; static unsigned char* leafVars[8];
extern "C" int RenderLeaf__6VIZoneiR15VISceneRendVars(unsigned char* zone, int leaf, unsigned char* vars) {
    leafZone[leafCalls] = zone; leafVars[leafCalls] = vars; leafIds[leafCalls] = leaf; ++leafCalls; return leafResult + leaf;
}
static void plane(unsigned char* n, float a, float b, float c, float d, int c16, int c20) {
    setf(n, a); setf(n + 4, b); setf(n + 8, c); setf(n + 12, d); setw(n + 16, c16); setw(n + 20, c20);
}
int main() {
    const int L = 0x40000000;
    struct Case { int node; int pointCount; float px[3]; float eyeX; int base; int expectRet; int expectCalls; int expectIds[4]; };
    // leaf result = base + leaf id; planes are x > 5
    const Case cases[] = {
        {0, 1, {10, 0, 0}, 0, 0, 0, 1, {1, 0, 0, 0}},          // behind, front only: child +16 (leaf 1)
        {0, 1, {1, 0, 0}, 0, 0, 0, 1, {2, 0, 0, 0}},           // behind, back only: child +20 (leaf 2)
        {0, 1, {10, 0, 0}, 0, -5, -1, 1, {1, 0, 0, 0}},        // first child negative: -1
        {0, 2, {10, 1, 0}, 0, 0, 0, 2, {1, 2, 0, 0}},          // behind, both: +16 then +20
        {0, 2, {10, 1, 0}, 9, 0, 0, 2, {2, 1, 0, 0}},          // not behind, both: +20 then +16
        {0, 2, {10, 1, 0}, 9, -3, -1, 1, {2, 0, 0, 0}},        // not behind, first (+20) negative
        {0, 2, {10, 1, 0}, 0, -2, -1, 1, {1, 0, 0, 0}},        // behind: first leaf (1-2) negative stops the walk
        {0, 0, {0, 0, 0}, 0, 0, 0, 0, {0, 0, 0, 0}},           // no points
        {static_cast<int>(0x80000000u), 1, {10, 0, 0}, 0, 0, 0, 1, {1, 0, 0, 0}},   // high bits masked, node 0
        {L | 9, 1, {10, 0, 0}, 0, 100, 109, 1, {9, 0, 0, 0}},  // leaf node: tail call result returned
        {static_cast<int>(0x50000000u) | 7, 1, {10, 0, 0}, 0, 0, 7, 1, {7, 0, 0, 0}},
        {1, 2, {10, 1, 0}, 0, 0, 0, 4, {5, 5, 5, 5}},          // node 1 has plane children 2 and 3, each with two leaf 5 children
    };
    for (const Case& c : cases) {
        std::memset(arena, 0, sizeof arena);
        unsigned char* zone = arena + 0x100; unsigned char* vars = arena + 0x200; unsigned char* camera = arena + 0x300;
        unsigned char* nodes = arena + 0x400; unsigned char* points = arena + 0x800;
        std::memset(zone, 0x22, 128); std::memset(vars, 0x33, 64);
        setu(zone + 72, word_of(nodes));
        plane(nodes, 1, 0, 0, -5, L | 1, L | 2);
        plane(nodes + 24, 1, 0, 0, -5, 2, 3);
        plane(nodes + 48, 1, 0, 0, -5, L | 5, L | 5);
        plane(nodes + 72, 1, 0, 0, -5, L | 5, L | 5);
        setw(vars + 24, c.pointCount); setu(vars + 28, word_of(points)); setu(vars + 12, word_of(camera));
        if (c.pointCount >= 1) { setf(points, c.px[0]); setf(points + 4, 0); setf(points + 8, 0); }
        if (c.pointCount >= 2) { setf(points + 12, c.px[1]); setf(points + 16, 0); setf(points + 20, 0); }
        setf(camera + 28, c.eyeX);
        leafCalls = 0; leafResult = c.base;
        int ret = RenderByVolume__6VIZoneiR15VISceneRendVars(zone, c.node, vars);
        if (ret != c.expectRet) return 1;
        if (leafCalls != c.expectCalls) return 10 + static_cast<int>(&c - cases);
        for (int i = 0; i < leafCalls; ++i)
            if (leafIds[i] != c.expectIds[i] || leafZone[i] != zone || leafVars[i] != vars) return 3;
    }
    // NaN distance counts as back; third point is never read when the first two already set both flags
    {
        std::memset(arena, 0, sizeof arena);
        unsigned char* zone = arena + 0x100; unsigned char* vars = arena + 0x200; unsigned char* camera = arena + 0x300;
        unsigned char* nodes = arena + 0x400; unsigned char* points = arena + 0x800;
        setu(zone + 72, word_of(nodes)); plane(nodes, 1, 0, 0, -5, L | 1, L | 2);
        setw(vars + 24, 3); setu(vars + 28, word_of(points)); setu(vars + 12, word_of(camera));
        setf(points, 10); setf(points + 12, 1); setf(points + 24, 0);
        float nan = __builtin_nanf(""); setf(points + 12, nan);
        leafCalls = 0; leafResult = 0;
        if (RenderByVolume__6VIZoneiR15VISceneRendVars(zone, 0, vars) != 0 || leafCalls != 2) return 4;
    }
    return 0;
}
