// Host check only; not a build input. The zone query is a recorder with a settable result.
#define NORRATH_HOST_ARENA 1
#include "collide.cpp"
#include <cstdint>
static unsigned char arena[0x4000];
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return reinterpret_cast<unsigned char*>((reinterpret_cast<uintptr_t>(arena) & ~uintptr_t{0xffffffffu}) | a); }
extern "C" void* norrath_host_function(unsigned) { return nullptr; }
static unsigned word_of(const void* p) { return static_cast<unsigned>(reinterpret_cast<uintptr_t>(p)); }
static int zone_calls, zone_result[4]; static unsigned char* zone_seen[8]; static float seen_a[8][3], seen_b[8][3];
extern "C" int Collide__6VIZoneR8VIEnvRayR7VIVect3T2(unsigned char* z, unsigned char*, const float* a, const float* b) {
    int k = zone_calls++;
    zone_seen[k & 7] = z;
    for (int i = 0; i < 3; ++i) { seen_a[k & 7][i] = a[i]; seen_b[k & 7][i] = b[i]; }
    return zone_result[k & 3];
}
static float& F(unsigned char* base, int off) { return *reinterpret_cast<float*>(base + off); }
static unsigned& W(unsigned char* base, int off) { return *reinterpret_cast<unsigned*>(base + off); }
int main() {
    unsigned char* world = arena; unsigned char* nodes = arena + 0x200; unsigned char* numbers = arena + 0x800; unsigned char* zones = arena + 0x1000; unsigned char* ray = arena + 0x1800;
    W(world, 56) = word_of(nodes); W(world, 64) = word_of(numbers); W(world, 76) = word_of(zones);
    for (int i = 0; i < 4; ++i) W(numbers, 4 * i) = 3 - i;
    // node 0: plane x = 0 with d = 0; +16 child is zone slot 0 (zone 3), +20 child is zone slot 1 (zone 2)
    F(nodes, 0) = 1; F(nodes, 4) = 0; F(nodes, 8) = 0; F(nodes, 12) = 0; W(nodes, 16) = 0x40000000u; W(nodes, 20) = 0x40000001u;
    float a[3] = {2, 1, 1}, b[3] = {4, 1, 1};
    zone_result[0] = 5;
    if (Collide__7VIWorldiR8VIEnvRayR7VIVect3T3(world, 0, ray, a, b) != 1 || zone_calls != 1 || zone_seen[0] != zones + 592 * 3) return 1;   // both in front
    a[0] = -2; b[0] = -4; zone_result[1] = 0;
    if (Collide__7VIWorldiR8VIEnvRayR7VIVect3T3(world, 0, ray, a, b) != 0 || zone_calls != 2 || zone_seen[1] != zones + 592 * 2) return 2;   // both behind
    a[0] = 0; b[0] = 0; zone_result[2] = 0;
    if (Collide__7VIWorldiR8VIEnvRayR7VIVect3T3(world, 0, ray, a, b) != 0 || zone_calls != 3 || zone_seen[2] != zones + 592 * 3) return 3;   // on the plane counts as in front
    // straddling: A at x = 2, B at x = -2 gives the split point (0, 3, 1)
    a[0] = 2; a[1] = 1; a[2] = 1; b[0] = -2; b[1] = 5; b[2] = 1;
    zone_calls = 0; zone_result[0] = 0; zone_result[1] = 0;
    if (Collide__7VIWorldiR8VIEnvRayR7VIVect3T3(world, 0, ray, a, b) != 0 || zone_calls != 2) return 4;
    if (zone_seen[0] != zones + 592 * 3 || zone_seen[1] != zones + 592 * 2) return 5;
    if (seen_a[0][0] != 2 || seen_b[0][0] != 0 || seen_b[0][1] != 3 || seen_a[1][0] != 0 || seen_a[1][1] != 3 || seen_b[1][0] != -2) return 6;
    // the first half hits: the walk stops there
    zone_calls = 0; zone_result[0] = 9;
    if (Collide__7VIWorldiR8VIEnvRayR7VIVect3T3(world, 0, ray, a, b) != 1 || zone_calls != 1) return 7;
    // A behind, B in front: the +20 child is asked first for (A, M)
    zone_calls = 0; zone_result[0] = 0; zone_result[1] = 4;
    a[0] = -2; b[0] = 2;
    if (Collide__7VIWorldiR8VIEnvRayR7VIVect3T3(world, 0, ray, a, b) != 1 || zone_calls != 2 || zone_seen[0] != zones + 592 * 2 || zone_seen[1] != zones + 592 * 3) return 8;
    // a leaf index goes straight to the zone query and returns its result unchanged
    zone_calls = 0; zone_result[0] = 7;
    if (Collide__7VIWorldiR8VIEnvRayR7VIVect3T3(world, 0x40000002, ray, a, b) != 7 || zone_seen[0] != zones + 592 * 1) return 9;
    return 0;
}
