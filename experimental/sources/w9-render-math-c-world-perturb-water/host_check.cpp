// Host contract for worldPerturbWater__FP12_worldHeaderG6Point3ff. Not an EE plan input.
// Build with -DNORRATH_HOST_PERTURB: stored 32-bit addresses are offsets into an arena.
#include <cstdio>
#include <cstring>
alignas(16) unsigned char arena[0x2000];
unsigned char* norrath_perturb_arena = arena;
signed char cutSkipMode = 0;
static unsigned char* found[3];
static float lo[3], hi[3];
static int seen_limit = 0;
extern "C" void worldFindWaterPatches__FP12_worldHeaderG6Point3T1PP13PolysetHeaderi(void*, const float* l, const float* h, unsigned char** out, int limit) {
    std::memcpy(lo, l, 12); std::memcpy(hi, h, 12); seen_limit = limit;
    for (int i = 0; i < 3; ++i) out[i] = found[i];
}
extern "C" int worldPerturbWater__FP12_worldHeaderG6Point3ff(void*, const float*, float, float);
static void put32(unsigned off, int v) { std::memcpy(arena + off, &v, 4); }
static void putf(unsigned off, float v) { std::memcpy(arena + off, &v, 4); }
static short cell(unsigned base, int x, int y) { short s; std::memcpy(&s, arena + base + 2704 + 2 * (x * 32 + y), 2); return s; }
int main() {
    // Patch at 0x100 -> record at 0x200 -> base 0x400. Offsets 40/44/48 are zero.
    put32(0x100, 0x200); put32(0x208, 0x400);
    putf(0x108, 0.0f); putf(0x114, 12.0f * 4.0f);     // span (48 * 1/12 + .5) -> 4, inv = 1/48
    found[0] = arena + 0x100; found[1] = nullptr;
    int world = 0;
    const float p[3] = {24.0f, 24.0f, 0.0f};            // gx = gy = 24 * 32 / 48 + 16 = 32 -> clamped box [30, 31]
    cutSkipMode = 1;
    if (worldPerturbWater__FP12_worldHeaderG6Point3ff(&world, p, 100.0f, 2.0f) != 0 || cell(0x400, 31, 31) != 0) return 1;
    cutSkipMode = 0;
    const float q[3] = {0.0f, 0.0f, 0.0f};              // gx = gy = 16, radius 2: box [13, 19]; rs = 2*32/48
    const int visited = worldPerturbWater__FP12_worldHeaderG6Point3ff(&world, q, 300.0f, 2.0f);
    if (visited != 1 || seen_limit != 16) return 2;
    if (lo[0] != -2.0f || lo[2] != -1.0f || hi[1] != 2.0f || hi[2] != 60.0f) return 3;
    // rs2 = (4/3)^2, gain = 300 / rs2 = 168.75. Centre cell: d2 = 0 -> +300.
    if (cell(0x400, 16, 16) != 300) return 4;
    // neighbour at distance 1: d2 = 1 -> 300 - 168.75 = 131.25 -> 131
    if (cell(0x400, 17, 16) != 131 || cell(0x400, 16, 15) != 131) return 5;
    // distance sqrt(2) > rs = 1.333: untouched
    if (cell(0x400, 17, 17) != 0 || cell(0x400, 14, 16) != 0) return 6;
    // second call adds on top and caps at 32767
    put32(0x400 + 2704 + 2 * (16 * 32 + 16), 0);       // reset a halfword pair (zero both)
    short big = 32700; std::memcpy(arena + 0x400 + 2704 + 2 * (16 * 32 + 16), &big, 2);
    worldPerturbWater__FP12_worldHeaderG6Point3ff(&world, q, 300.0f, 2.0f);
    if (cell(0x400, 16, 16) != 32767) return 7;
    // a patch whose record has no base is visited and skipped
    put32(0x208, 0);
    if (worldPerturbWater__FP12_worldHeaderG6Point3ff(&world, q, 300.0f, 2.0f) != 1 || cell(0x400, 17, 16) != 131 + 131) return 8;
    std::printf("world perturb water contract passed\n");
    return 0;
}
