// Host contract for routeMakeLattice__Fv. Not an EE plan input.
#include <cmath>
#include <cstdio>
#include <cstring>
struct _worldHeader;
struct Point3 { float x, y, z; };
_worldHeader* world = nullptr;
int nodecount = 0;
Point3 route_dirs[8];
extern "C" {
alignas(8) unsigned char norrath_route_nodes_bytes[32768];
unsigned char norrath_route_node_used_bytes[4];
}
static int pass_calls;
static bool block_pair_0_2;
extern "C" void swap__H1Zf_RX01T0_v(float* a, float* b) { float t = *a; *a = *b; *b = t; }
extern "C" void swap__H1Zs_RX01T0_v(short* a, short* b) { short t = *a; *a = *b; *b = t; }
extern "C" int worldCanPass__FP12_worldHeaderG6Point3T1fi(_worldHeader*, Point3 a, Point3 b, float radius, int mode) {
    ++pass_calls;
    if (radius != 6.0f || mode != 3) return 0;
    return !(block_pair_0_2 && ((a.x == 0 && b.x == 20) || (a.x == 20 && b.x == 0)));
}
extern "C" float icos__Fi(int a) { return std::cos(a * 3.14159265f / 32768.0f); }
extern "C" float isin__Fi(int a) { return std::sin(a * 3.14159265f / 32768.0f); }
extern "C" void routeMakeLattice__Fv();
static void place(int i, float x, float y, float z) {
    float p[3] = {x, y, z};
    std::memcpy(norrath_route_nodes_bytes + 64 * i, p, 12);
}
static short count_of(int i) { short c; std::memcpy(&c, norrath_route_nodes_bytes + 64 * i + 52, 2); return c; }
static short link_of(int i, int k) { short v; std::memcpy(&v, norrath_route_nodes_bytes + 64 * i + 12 + 2 * k, 2); return v; }
int main() {
    // Three collinear nodes 0, 10, 20 on x: the 0-20 link is dropped as a shortcut candidate.
    const int used = 3;
    std::memcpy(norrath_route_node_used_bytes, &used, 4);
    place(0, 0, 0, 0); place(1, 10, 0, 0); place(2, 20, 0, 0);
    routeMakeLattice__Fv();
    if (nodecount != 3 || pass_calls == 0) return 1;
    if (count_of(0) != 1 || link_of(0, 0) != static_cast<short>(1 | 0x8000)) return 2;
    if (count_of(2) != 1 || link_of(2, 0) != static_cast<short>(1 | 0x8000)) return 3;
    if (count_of(1) != 2 || link_of(1, 0) != static_cast<short>(0 | 0x8000) || link_of(1, 1) != static_cast<short>(2 | 0x8000)) return 4;
    // Direction table: 8 unit vectors at multiples of an eighth of the 16-bit circle.
    if (std::fabs(route_dirs[0].x - 1.0f) > 1e-5f || std::fabs(route_dirs[2].y - 1.0f) > 1e-5f || route_dirs[5].z != 0.0f) return 5;
    // A blocked pair is not linked: node 0 only keeps node 1.
    block_pair_0_2 = true;
    routeMakeLattice__Fv();
    if (count_of(0) != 1 || count_of(2) != 1) return 6;
    // Empty graph: nodecount follows nodeused and the table is still written.
    const int none = 0;
    std::memcpy(norrath_route_node_used_bytes, &none, 4);
    route_dirs[3].x = 5.0f;
    routeMakeLattice__Fv();
    if (nodecount != 0 || route_dirs[3].x == 5.0f) return 7;
    std::printf("route make lattice contract passed\n");
    return 0;
}
