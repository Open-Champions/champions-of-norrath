// Host contract for findClosestShadowBall__13MissileWeaponP10GameObject; not an EE input. 32-bit words are offsets
// from a host base; the table call and the model functions are stubs that log and return scripted values. The
// authored function was also compared with the original in a private R5900 run. Build with -ffp-contract=off.
// Returns non-zero when wrong.
#define W11_ACTOR_A_HOST 1
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w11_actor_a_host_base;
#include "find_closest_shadow_ball.cpp"

static unsigned char arena[0x4000] __attribute__((aligned(16)));
static unsigned char* const missile = arena + 0x100;
static unsigned char* const target = arena + 0x1000;
static std::string calls;
static unsigned modelResult;
static int pointCount;
static float values[8];
static Point3 points[8];
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
unsigned w11_actor_a_host_call(unsigned fn, void* adjusted, int argc, unsigned long, unsigned long) {
    calls += "vcall(" + num(fn) + "," + num(static_cast<unsigned char*>(adjusted) - arena) + "," + num(argc) + ");";
    return modelResult;
}
extern "C" {
int modelGetNmShadowPoints__FP12_modelHeader(void* m) { calls += "count(" + num(reinterpret_cast<__UINTPTR_TYPE__>(m)) + ");"; return pointCount; }
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
    void* m, void* a, Point3 p, short h, int i, Point3* out, float* v, void* mat, float s) {
    calls += "pos(" + num(reinterpret_cast<__UINTPTR_TYPE__>(m)) + "," + num(static_cast<unsigned char*>(a) - target) + "," + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + "," +
             num(h) + "," + num(i) + "," + (mat ? "mat" : "null") + "," + num(s) + ");";
    *out = points[i]; *v = values[i];
}
}
static void put32(unsigned char* o, unsigned off, unsigned v) { std::memcpy(o + off, &v, 4); }
static void putf(unsigned char* o, unsigned off, float v) { std::memcpy(o + off, &v, 4); }
int main() {
    int bad = 0;
    struct Case { int n; float v[4]; Point3 p[4]; int want; };
    const Case cases[] = {
        {3, {1, 5, 4.9f}, {{1, 1, 1}, {9, 9, 9}, {2, 2, 2}}, 2},         // index 0 is under 0.75 * top; of 1 and 2, index 2 is nearer
        {4, {2, 8, 6, 5.9f}, {{0, 0, 0}, {50, 0, 0}, {10, 0, 0}, {5, 0, 0}}, 2},
        {2, {0, 0}, {{3, 3, 3}, {1, 1, 1}}, 0},         // equal distances keep the first
        {1, {-5}, {{1, 0, 0}}, 0},                    // below top (0.0): skipped, so 0 is the default
        {2, {4, 4}, {{100, 100, 100}, {200, 200, 200}}, 0},
    };
    for (int modelNull = 0; modelNull < 2; ++modelNull)
        for (int tnull = 0; tnull < 2; ++tnull)
            for (const Case& c : cases) {
                std::memset(arena, 0, sizeof arena); w11_actor_a_host_base = arena; calls.clear();
                put32(target, 72, 0x2000); *reinterpret_cast<short*>(arena + 0x2000 + 128) = -16; put32(arena + 0x2000, 132, 0x777);
                putf(missile, 16, 1); putf(missile, 20, 2); putf(missile, 24, 3);
                putf(target, 16, 4); putf(target, 20, 5); putf(target, 24, 6);
                *reinterpret_cast<short*>(target + 56) = -3; putf(target, 224, 0.5f);
                modelResult = modelNull ? 0 : 0x55; pointCount = c.n;
                for (int i = 0; i < 4; ++i) { values[i] = c.v[i]; points[i] = c.p[i]; }
                int got = findClosestShadowBall__13MissileWeaponP10GameObject(missile, tnull ? nullptr : target);
                // Expected result recomputed from the rule.
                int want = 0;
                std::string wantCalls;
                if (!tnull) {
                    wantCalls = "vcall(1911,4080,0);";
                    if (!modelNull) {
                        wantCalls += "count(85);";
                        float top = 0, best = 1e8f;
                        for (int pass = 0; pass < 2 && c.n; ++pass)
                            for (int i = 0; i < c.n; ++i) {
                                wantCalls += "pos(85,288,4:5:6,-3,"+ num(i) + ",null,0.5);";
                                if (pass == 0) { if (top < c.v[i]) top = c.v[i]; continue; }
                                if (c.v[i] < top * 0.75f) continue;
                                float dx = 1 - c.p[i].x, dy = 2 - c.p[i].y, dz = 3 - c.p[i].z;
                                float d = std::sqrt(dx * dx + dy * dy + dz * dz);
                                if (d < best) { want = i; best = d; }
                            }
                    }
                }
                if (!tnull && !modelNull && want != c.want) { std::puts("fail: table expectation"); bad = 1; }
                if (got != want || calls != wantCalls) { std::printf("fail: n=%d model=%d t=%d got %d want %d\n  want %s\n  got  %s\n", c.n, !modelNull, !tnull, got, want, wantCalls.c_str(), calls.c_str()); bad = 1; }
            }
    return bad;
}
