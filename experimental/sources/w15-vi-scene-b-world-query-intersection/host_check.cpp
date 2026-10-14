// Host check only; not a build input. The zone query is a recorder with a settable result.
#define NORRATH_HOST_ARENA 1
#include "query_intersection.cpp"
#include <cstdint>
static unsigned char arena[0x4000];
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return reinterpret_cast<unsigned char*>((reinterpret_cast<uintptr_t>(arena) & ~uintptr_t{0xffffffffu}) | a); }
extern "C" void* norrath_host_function(unsigned) { return nullptr; }
static unsigned word_of(const void* p) { return static_cast<unsigned>(reinterpret_cast<uintptr_t>(p)); }
static int zone_calls, zone_result; static unsigned char* zone_seen[8]; static unsigned char* zone_vars;
extern "C" int QueryIntersection__6VIZoneR20VISceneIntersectVars(unsigned char* z, unsigned char* v) { zone_seen[zone_calls++ & 7] = z; zone_vars = v; return zone_result; }
static float& F(unsigned char* base, int off) { return *reinterpret_cast<float*>(base + off); }
static unsigned& W(unsigned char* base, int off) { return *reinterpret_cast<unsigned*>(base + off); }
int main() {
    unsigned char* world = arena; unsigned char* nodes = arena + 0x200; unsigned char* numbers = arena + 0x800; unsigned char* zones = arena + 0x1000; unsigned char* vars = arena + 0x2000;
    W(world, 56) = word_of(nodes); W(world, 64) = word_of(numbers); W(world, 76) = word_of(zones);
    for (int i = 0; i < 4; ++i) W(numbers, 4 * i) = 3 - i;
    // node 0: plane x = 0 (normal (1,0,0), d 0), children: leaf zone slot 0 (+16) and leaf zone slot 1 (+20)
    F(nodes, 0) = 1; F(nodes, 4) = 0; F(nodes, 8) = 0; F(nodes, 12) = 0; W(nodes, 16) = 0x40000000u; W(nodes, 20) = 0x40000001u;
    // eight corner points: first four at x=+1, last four at x=-1 (straddling), centre and extent set per case
    for (int k = 0; k < 8; ++k) { F(vars, 44 + 12 * k) = k < 4 ? 1.0f : -1.0f; F(vars, 48 + 12 * k) = 0; F(vars, 52 + 12 * k) = 0; }
    F(vars, 140) = 5; F(vars, 144) = 0; F(vars, 148) = 0; F(vars, 152) = 1;     // centre x=5, extent 1: wholly in front
    zone_result = 0;
    if (QueryIntersection__7VIWorldiR20VISceneIntersectVars(world, 0, vars) != 0 || zone_calls != 1 || zone_seen[0] != zones + 592 * 3 || zone_vars != vars) return 1;
    F(vars, 140) = -5;                                                           // wholly behind: only the +20 child, zone number 2
    zone_result = -1;
    if (QueryIntersection__7VIWorldiR20VISceneIntersectVars(world, 0, vars) != -1 || zone_calls != 2 || zone_seen[1] != zones + 592 * 2) return 2;
    F(vars, 140) = 0;                                                            // straddles: both children are queried, front first
    zone_result = 0;
    if (QueryIntersection__7VIWorldiR20VISceneIntersectVars(world, 0, vars) != 0 || zone_calls != 4 || zone_seen[2] != zones + 592 * 3 || zone_seen[3] != zones + 592 * 2) return 3;
    zone_result = -1;                                                            // a failing front child ends the walk at once
    if (QueryIntersection__7VIWorldiR20VISceneIntersectVars(world, 0, vars) != -1 || zone_calls != 5) return 4;
    // a top-nibble index without bit 30 has the nibble masked off and is treated as node 0
    F(vars, 140) = 5; zone_result = 0;
    if (QueryIntersection__7VIWorldiR20VISceneIntersectVars(world, 0x20000000, vars) != 0 || zone_calls != 6) return 5;
    // a leaf index goes straight to the zone query
    if (QueryIntersection__7VIWorldiR20VISceneIntersectVars(world, 0x40000002, vars) != 0 || zone_seen[(zone_calls - 1) & 7] != zones + 592 * 1) return 6;
    return 0;
}
