// Host check only; not a build input. The zone query is a recorder; it can also move the ray's points.
#define NORRATH_HOST_ARENA 1
#include "collide.cpp"
#include <cstdint>
static unsigned char arena[0x4000];
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return reinterpret_cast<unsigned char*>((reinterpret_cast<uintptr_t>(arena) & ~uintptr_t{0xffffffffu}) | a); }
extern "C" void* norrath_host_function(unsigned) { return nullptr; }
static unsigned word_of(const void* p) { return static_cast<unsigned>(reinterpret_cast<uintptr_t>(p)); }
static float& F(unsigned char* base, int off) { return *reinterpret_cast<float*>(base + off); }
static unsigned& W(unsigned char* base, int off) { return *reinterpret_cast<unsigned*>(base + off); }
static int zone_calls, zone_result[4]; static unsigned char* zone_seen[8]; static float move_p_x_after_first; static unsigned char* vars_ptr; static bool move_after_first;
extern "C" int Collide__6VIZoneR18VISceneCollRayVars(unsigned char* z, unsigned char* v) {
    int k = zone_calls++;
    zone_seen[k & 7] = z; vars_ptr = v;
    if (k == 0 && move_after_first) { F(v, 20) = move_p_x_after_first; F(v, 32) = move_p_x_after_first; }
    return zone_result[k & 3];
}
int main() {
    unsigned char* world = arena; unsigned char* nodes = arena + 0x200; unsigned char* numbers = arena + 0x800; unsigned char* zones = arena + 0x1000; unsigned char* vars = arena + 0x1800;
    W(world, 56) = word_of(nodes); W(world, 64) = word_of(numbers); W(world, 76) = word_of(zones);
    for (int i = 0; i < 4; ++i) W(numbers, 4 * i) = 3 - i;
    // node 0: plane x = 0.0, d = 0; +16 child is zone slot 0 (zone 3), +20 child is zone slot 1 (zone 2)
    F(nodes, 0) = 1; F(nodes, 4) = 0; F(nodes, 8) = 0; F(nodes, 12) = 0; W(nodes, 16) = 0x40000000u; W(nodes, 20) = 0x40000001u;
    auto setPoints = [&](float p, float q) { F(vars, 20) = p; F(vars, 24) = 0; F(vars, 28) = 0; F(vars, 32) = q; F(vars, 36) = 0; F(vars, 40) = 0; };
    // both beyond the tolerance (0.03125) in front
    setPoints(1.0f, 2.0f); zone_result[0] = 3;
    if (Collide__7VIWorldiR18VISceneCollRayVars(world, 0, vars) != 1 || zone_calls != 1 || zone_seen[0] != zones + 592 * 3) return 1;
    // exactly on the tolerance is not "beyond" it, but with both points at or above 0.03125 the second query is not needed
    zone_calls = 0; setPoints(0.03125f, 0.03125f); zone_result[0] = 0; zone_result[1] = 0;
    if (Collide__7VIWorldiR18VISceneCollRayVars(world, 0, vars) != 0 || zone_calls != 1 || zone_seen[0] != zones + 592 * 3) return 2;
    // one point inside the tolerance band asks both children
    zone_calls = 0; setPoints(0.5f, 0.01f); zone_result[0] = 0; zone_result[1] = 0;
    if (Collide__7VIWorldiR18VISceneCollRayVars(world, 0, vars) != 0 || zone_calls != 2 || zone_seen[0] != zones + 592 * 3 || zone_seen[1] != zones + 592 * 2) return 2;
    // both behind
    zone_calls = 0; setPoints(-1.0f, -2.0f); zone_result[0] = 0;
    if (Collide__7VIWorldiR18VISceneCollRayVars(world, 0, vars) != 0 || zone_calls != 1 || zone_seen[0] != zones + 592 * 2) return 3;
    // straddling with P in front: the +16 child first, then +20 (both asked even when the first hits)
    zone_calls = 0; setPoints(1.0f, -1.0f); zone_result[0] = 4; zone_result[1] = 0;
    if (Collide__7VIWorldiR18VISceneCollRayVars(world, 0, vars) != 1 || zone_calls != 2 || zone_seen[0] != zones + 592 * 3 || zone_seen[1] != zones + 592 * 2) return 4;
    // straddling with P behind: the +20 child first, then +16
    zone_calls = 0; setPoints(-1.0f, 1.0f); zone_result[0] = 0; zone_result[1] = 6;
    if (Collide__7VIWorldiR18VISceneCollRayVars(world, 0, vars) != 1 || zone_calls != 2 || zone_seen[0] != zones + 592 * 2 || zone_seen[1] != zones + 592 * 3) return 5;
    // the points are re-read after the first query: moving both to x = 5 makes the second query unnecessary
    zone_calls = 0; setPoints(1.0f, -1.0f); zone_result[0] = 0; move_after_first = true; move_p_x_after_first = 5.0f;
    if (Collide__7VIWorldiR18VISceneCollRayVars(world, 0, vars) != 0 || zone_calls != 1) return 6;
    move_after_first = false;
    // a leaf index goes straight to the zone query and returns its result unchanged
    zone_calls = 0; zone_result[0] = 7;
    if (Collide__7VIWorldiR18VISceneCollRayVars(world, 0x40000002, vars) != 7 || zone_seen[0] != zones + 592 * 1 || vars_ptr != vars) return 7;
    return 0;
}
