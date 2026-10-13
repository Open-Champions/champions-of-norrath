// Host contract for fxRunCreatureShock. Not an EE plan input. Build with -ffp-contract=off.
// Pointers to the handle record are real host objects; a handle is stored as an int.
#include <cstdio>
#include <cstring>
struct Point3 {
    float x, y, z;
};
extern "C" {
int eRandState;
unsigned char ballSparkParticle[16], ballSmokeParticle[16];
static int closeResult = 1, points = 5, starts, moves, sparks, smokes, moveArg;
static int shadowIndex[16], shadowCount;
int gameCloseToCamera__FP6Point3(Point3*) { return closeResult; }
int modelGetNmShadowPoints__FP12_modelHeader(void*) { return points; }
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
    void*, void*, Point3*, short, int index, Point3* out, float*, void*, float) {
    if (shadowCount < 16) shadowIndex[shadowCount++] = index;
    out->x = static_cast<float>(index);
    out->y = 0.0f;
    out->z = 0.0f;
}
float icos__Fi(int) { return 1.0f; }
float isin__Fi(int) { return 0.0f; }
int P_StartLightning__FG6Point3T0iiffUi(Point3*, Point3*, int a, int b, float, float, unsigned) {
    ++starts;
    return (a == 207 && b == 4) ? 40 + starts : -1;
}
void P_MoveLightning__FiG6Point3T1ii(int, Point3*, Point3*, int a, int b) {
    ++moves;
    moveArg = a * 10 + (b == -1);
}
void P_AddParticle__FP11ParticleDefP6Point3T1f(const void* d, Point3*, Point3*, float) {
    if (d == ballSparkParticle) ++sparks;
    if (d == ballSmokeParticle) ++smokes;
}
void fxRunCreatureShock__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefffUi(void*, void*, Point3*,
                                                                                            short, unsigned char*,
                                                                                            float, float, float,
                                                                                            unsigned);
}
static unsigned lcg(unsigned s) { return s * 1664525u + 1013904223u; }
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    alignas(8) unsigned char state[40];
    Point3 pos = {0, 0, 0};
    auto run = [&](float a) {
        fxRunCreatureShock__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefffUi(
            reinterpret_cast<void*>(1), reinterpret_cast<void*>(2), &pos, 0, state, a, 0.0f, 0.0f, 7u);
    };
    auto word = [&](int off) { int v; std::memcpy(&v, state + off, 4); return v; };
    auto half = [&](int off) { short v; std::memcpy(&v, state + off, 2); return static_cast<int>(v); };

    // Close check fails: nothing happens.
    std::memset(state, 0, sizeof state);
    closeResult = 0;
    run(1.0f);
    expect(starts == 0 && word(12) == 0, "not close to the camera");
    closeResult = 1;

    // Empty records are all rebuilt: two draws each, reduced by the point count.
    unsigned seed = 12345;
    eRandState = static_cast<int>(seed);
    run(1.0f);
    expect(starts == 3 && word(12) == 41 && word(20) == 42 && word(28) == 43, "three bolts started");
    unsigned s = seed;
    bool ok = true;
    for (int i = 0; i < 3; ++i) {
        s = lcg(s);
        ok = ok && half(8 + 8 * i) == static_cast<int>(s >> 16) % 5;
        s = lcg(s);
        ok = ok && half(10 + 8 * i) == static_cast<int>(s >> 16) % 5;
    }
    expect(ok && static_cast<unsigned>(eRandState) == s, "halfwords and generator state");

    // Existing bolts: find a seed whose first draw keeps the first bolt (value >= 20000).
    unsigned keep = 1;
    while ((lcg(keep) >> 16) < 20000u) ++keep;
    starts = moves = sparks = 0;
    shadowCount = 0;
    std::memset(state, 0, sizeof state);
    int h = 99;
    std::memcpy(state + 12, &h, 4);
    short big = 9;
    std::memcpy(state + 8, &big, 2);
    eRandState = static_cast<int>(keep);
    run(3.0f);
    expect(moves >= 1 && moveArg == 941, "bolt moved with a clamped to 1 (94) and -1");
    expect(word(12) == 99, "kept handle");
    expect(half(8) == 0, "halfword above the point count reset");
    expect(sparks >= 2, "sparks for both ends");
    expect(shadowIndex[0] == 0, "reset index used");
    if (failures) return 1;
    std::printf("fxRunCreatureShock contract passed\n");
    return 0;
}
