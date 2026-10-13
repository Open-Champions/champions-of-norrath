#define NORRATH_HOST_ARENA
#include <string.h>
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "find_leaf.cpp"

static const void* zone_arg; static const float* point_arg; static int zone_result, zone_calls;
extern "C" int FindLeaf__C6VIZoneRC7VIVect3(const void* z, const float* p) { zone_arg = z; point_arg = p; ++zone_calls; return zone_result; }

static unsigned& W(unsigned off) { return *reinterpret_cast<unsigned*>(arena + off); }
static float& F(unsigned off) { return *reinterpret_cast<float*>(arena + off); }
static void node(unsigned nodes, int i, float a, float b, float c, float d, unsigned front, unsigned back) {
    unsigned o = nodes + 24 * i; F(o) = a; F(o + 4) = b; F(o + 8) = c; F(o + 12) = d; W(o + 16) = front; W(o + 20) = back;
}

int main() {
    const unsigned world = 0, nodes = 512, zones = 2048, zone_objs = 4096;
    memset(arena, 0, sizeof arena);
    const unsigned LEAF = 0x40000000u;
    // node 0 splits on x (x >= 0 front -> node 1, back -> leaf 5), node 1 splits on y (front -> leaf 2, back -> leaf 3)
    node(nodes, 0, 1, 0, 0, 0, 1, LEAF | 5);
    node(nodes, 1, 0, 1, 0, -2, LEAF | 2, LEAF | 3);
    W(world + 52) = 1; W(world + 56) = arena_base + nodes;
    W(world + 64) = arena_base + zones; W(world + 76) = arena_base + zone_objs;
    for (int i = 0; i < 8; ++i) W(zones + 4 * i) = 10 + i;
    float p[3] = {1.0f, 3.0f, 9.0f};
    int zone = -1, leaf = -1;
    zone_result = 77; zone_calls = 0;
    if (FindLeaf__C7VIWorldRC7VIVect3RiT2(arena + world, p, &zone, &leaf) != 0) return 1;
    if (zone != 12 || leaf != 77 || zone_arg != arena + zone_objs + 12 * 592 || point_arg != p || zone_calls != 1) return 2;
    float q[3] = {1.0f, 1.0f, 0.0f};                       // y - 2 < 0 -> back -> leaf 3
    zone_result = -4;
    if (FindLeaf__C7VIWorldRC7VIVect3RiT2(arena + world, q, &zone, &leaf) != -1 || zone != 13 || leaf != -4) return 3;
    float r[3] = {-1.0f, 5.0f, 0.0f};                      // x < 0 -> back of node 0 -> leaf 5
    zone_result = 0;
    if (FindLeaf__C7VIWorldRC7VIVect3RiT2(arena + world, r, &zone, &leaf) != 0 || zone != 15 || leaf != 0) return 4;
    float s[3] = {0.0f, 2.0f, 0.0f};                       // distances of exactly zero go to the front child
    if (FindLeaf__C7VIWorldRC7VIVect3RiT2(arena + world, s, &zone, &leaf) != 0 || zone != 12) return 5;
    // no tree: leaf 0
    W(world + 52) = 0;
    zone_result = 3;
    if (FindLeaf__C7VIWorldRC7VIVect3RiT2(arena + world, p, &zone, &leaf) != 0 || zone != 10 || zone_arg != arena + zone_objs + 10 * 592) return 6;
    // summing order: ((x*a + y*b) + z*c) + d with values where the order matters
    W(world + 52) = 1;
    node(nodes, 0, 1e8f, 1.0f, -1e8f, 0.5f, LEAF | 1, LEAF | 2);
    float t[3] = {1.0f, 1.0f, 1.0f};                       // ((1e8 + 1) - 1e8) + 0.5 = 0.5 in floats: (1e8+1)=1e8 -> 0 + 0.5 >= 0
    if (FindLeaf__C7VIWorldRC7VIVect3RiT2(arena + world, t, &zone, &leaf) != 0 || zone != 11) return 7;
    node(nodes, 0, 1e8f, 1.0f, -1e8f, -0.5f, LEAF | 1, LEAF | 2);
    if (FindLeaf__C7VIWorldRC7VIVect3RiT2(arena + world, t, &zone, &leaf) != 0 || zone != 12) return 8;
    return 0;
}
