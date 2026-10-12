// Host contract for eventCollisionWall__11LavaTractorP6Point3T1. Not an EE plan input.
#include <cstdio>
#include <cstring>
extern "C" void eventCollisionWall__11LavaTractorP6Point3T1(void* object, const float* a, const float* b);
static unsigned char object[16];
static unsigned rand_value = 4;
static int rands = 0, plays = 0;
static char name_seen[160], lump_seen[32];
static void* played_object = nullptr;
static float played_volume = 0;
static char token;
extern "C" unsigned eRand__10GameObject(void*) { ++rands; return rand_value; }
extern "C" void* lumpFindResource__FPcT0(char* lump, char* name) {
    std::strncpy(lump_seen, lump, 31); std::strncpy(name_seen, name, 159); return &token;
}
extern "C" int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* h, void* o, float v) {
    ++plays; played_object = o; played_volume = v; return h == &token ? 1 : 0;
}
static float from_bits(unsigned bits) { float f; std::memcpy(&f, &bits, 4); return f; }
#define EXPECT(c, m) do { if (!(c)) { std::fprintf(stderr, "fail: %s\n", m); return 1; } } while (0)
static int run(const float* a, const float* b) {
    rands = plays = 0; name_seen[0] = lump_seen[0] = 0; played_object = nullptr;
    eventCollisionWall__11LavaTractorP6Point3T1(object, a, b);
    return plays;
}
int main() {
    const float limit = from_bits(0xc02967e8u);
    const float a[3] = {1.0f, 0.0f, 0.0f};
    float b[3] = {limit, 0, 0};
    EXPECT(run(a, b) == 0 && rands == 0, "equal to threshold does not fire");
    b[0] = from_bits(0xc02967e7u);  // just above the threshold
    EXPECT(run(a, b) == 0, "just above does not fire");
    b[0] = from_bits(0xc02967e9u);  // just below
    EXPECT(run(a, b) == 1 && rands == 1, "just below fires");
    EXPECT(std::strcmp(name_seen, "lavatractor_coll_metal2.anm") == 0 && std::strcmp(lump_seen, "tractor.lmp") == 0, "name");
    EXPECT(played_object == object && played_volume == 1.0f, "play args");
    // every component contributes, and a and b are not mixed up componentwise
    const float c[3] = {1.0f, 2.0f, 3.0f};
    const float d[3] = {-1.0f, -1.0f, -1.0f};  // dot = -6
    EXPECT(run(c, d) == 1, "sum of three terms");
    const float e[3] = {0.0f, 0.0f, 1.0f};
    const float f[3] = {-10.0f, -10.0f, 0.0f};  // dot = 0
    EXPECT(run(e, f) == 0, "components pair up");
    const float g[3] = {0.0f, 0.0f, 1.0f};
    const float h[3] = {0.0f, 0.0f, -3.0f};
    EXPECT(run(g, h) == 1, "z term");
    rand_value = 3;
    EXPECT(run(c, d) == 1 && std::strcmp(name_seen, "lavatractor_coll_metal1.anm") == 0, "wraps to 1");
    return 0;
}
