// Not an EE plan input. Build: c++ -std=c++17 -Wall -Wextra -Werror -ffp-contract=off host_check.cpp
// Callees are stubs that record their arguments; eRand answers from a queue (32768 when empty).
#include "resurect_effect.cpp"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <deque>
#include <string>
#include <vector>

extern "C" {
signed char netNetworkMode, recallOn;
alignas(8) unsigned char player[4 * 23984];
}
alignas(8) static unsigned char object[1400];
static std::string trace;
static void* allocAnswer;
static std::deque<int> rands;
struct Shadow { Point3 point; float value; };
static std::vector<Shadow> shadows;
struct Particle { Point3 pos, vel; float life; };
static std::vector<Particle> particles;
static int ctorCalls, shadowCalls;
static float lightDuration;
static Point3 lightPos;
extern "C" {
void* blockAlloc__Fi(int n) { trace += "alloc" + std::to_string(n) + ";"; return allocAnswer; }
void __11LightEffectG6Point3ffffffffffffP10GameObjecti(void* self, Point3 p, float a, float b, float c, float d, float e, float f, float g, float h,
                                                       float i, float j, float k, float l, void* o, int m) {
    lightPos = p; lightDuration = l;
    trace += std::string("light") + (self == allocAnswer ? "" : "?") + ":" + std::to_string((int)a) + "," + std::to_string((int)b) + "," + std::to_string((int)c) + "," +
             std::to_string((int)d) + "," + std::to_string((int)e) + "," + std::to_string((int)f) + "," + std::to_string((int)g) + "," + std::to_string((int)h) + "," +
             std::to_string((int)i) + "," + std::to_string((int)j) + "," + std::to_string((int)k) + (o == nullptr ? "" : "?") + std::to_string(m) + ";";
}
void* lumpFindResource__FPcT0(char* l, char* n) { trace += std::string("find:") + l + "," + n + ";"; return &trace; }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* s, void* o, float v) {
    trace += std::string("seq") + (s == &trace ? "" : "?") + ":" + std::to_string(reinterpret_cast<__UINTPTR_TYPE__>(o)) + "," + std::to_string((int)v) + ";";
}
void __11ParticleDefiiiiiiiifffiiiPFPv_v(void* self, int a, int b, int c, int d, int e, int f, int g, int h, float i, float j, float k, int l, int m, int n,
                                         void (*cb)(void*)) {
    ++ctorCalls;
    trace += std::string("pdef") + (self == resurectParticle ? "" : "?") + ":" + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + "," +
             std::to_string(d) + "," + std::to_string(e) + "," + std::to_string(f) + "," + std::to_string(g) + "," + std::to_string(h) + "," + std::to_string((int)i) + "," +
             std::to_string((int)(j * 10)) + "," + std::to_string((int)k) + "," + std::to_string(l) + "," + std::to_string(m) + "," + std::to_string(n) + (cb ? "?" : "") + ";";
}
int modelGetNmShadowPoints__FP12_modelHeader(void* m) { trace += std::string("nm") + (m == reinterpret_cast<void*>(0x4321) ? "" : "?") + ";"; return static_cast<int>(shadows.size()); }
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(void* m, void* st, Point3 pos, short yaw, int index, Point3* out,
                                                                                         float* value, void* matrix, float scale) {
    ++shadowCalls;
    trace += std::string("shadow") + (m == reinterpret_cast<void*>(0x4321) ? "" : "?") + (st == object + 288 ? "" : "?") + ":" + std::to_string((int)pos.x) + "," +
             std::to_string((int)pos.y) + "," + std::to_string((int)pos.z) + "," + std::to_string(yaw) + "," + std::to_string(index) + (matrix ? "?" : "") + "," +
             std::to_string((int)scale) + ";";
    *out = shadows[index].point;
    *value = shadows[index].value;
}
int eRand__10GameObject(void* self) {
    if (self != object) trace += "rand?;";
    if (rands.empty()) return 32768;
    int v = rands.front(); rands.pop_front(); return v;
}
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* def, Point3* p, Point3* v, float life) {
    if (def != resurectParticle) trace += "def?;";
    particles.push_back({*p, *v, life});
}
}
static int bad;
static void expect(const char* what, const std::string& got, const std::string& want) {
    if (got != want) { std::printf("FAIL %s\n got  %s\n want %s\n", what, got.c_str(), want.c_str()); bad = 1; }
}
static bool near(float a, float b) { return std::fabs(a - b) <= 1e-3f * (1.0f + std::fabs(b)); }
static void reset() {
    std::memset(object, 0, sizeof object);
    std::memset(player, 0, sizeof player);
    int idx = 2; std::memcpy(object + 960, &idx, 4);
    float pos[3] = {10.0f, 20.0f, 30.0f}; std::memcpy(object + 16, pos, 12);
    short yaw = 77; std::memcpy(object + 56, &yaw, 2);
    void* model = reinterpret_cast<void*>(0x4321); std::memcpy(object + 1332, &model, sizeof model);
    void* seqObject = reinterpret_cast<void*>(0x9999); std::memcpy(player + 2 * 23984 + 21872, &seqObject, sizeof seqObject);
    netNetworkMode = 0; recallOn = 0; allocAnswer = &trace; rands.clear(); shadows.clear(); particles.clear();
    trace.clear(); ctorCalls = shadowCalls = 0; resurectParticleReady = 0;
}
static short duration() { short d; std::memcpy(&d, object + 1188, 2); return d; }
static unsigned flags() { unsigned f; std::memcpy(&f, object + 12, 4); return f; }

int main() {
    // Duration 60 offline, 90 online, 1 when recalling; flag bit 2 is set; the block is 188 bytes.
    reset(); resurectEffect__6Player(object);
    expect("duration offline", std::to_string(duration()) + "," + std::to_string(flags()), "60,2");
    reset(); netNetworkMode = 1; resurectEffect__6Player(object); expect("duration net", std::to_string(duration()), "90");
    reset(); netNetworkMode = 1; recallOn = 1; resurectEffect__6Player(object); expect("duration recall", std::to_string(duration()), "1");
    reset(); std::memset(object + 12, 0xff, 4); resurectEffect__6Player(object); expect("flags or", std::to_string(flags() == 0xffffffffu), "1");
    // Trace of the fixed part (no shadow points) and the light effect values.
    reset(); resurectEffect__6Player(object);
    expect("fixed trace", trace,
           "alloc188;light:128,128,256,-50,0,0,0,0,240,120,0" "0;find:fx.lmp,spel_cas_curelw.anm;seq:39321,1;pdef:70,83,74,35,31,43,75,55,0,195,0,3,1,8;nm;");
    if (!(lightPos.x == 10.0f && lightPos.y == 20.0f && lightPos.z == 66.0f && near(lightDuration, 1.0f))) { std::printf("FAIL light values\n"); bad = 1; }
    reset(); netNetworkMode = 1; resurectEffect__6Player(object);
    if (!near(lightDuration, 1.5f)) { std::printf("FAIL light duration net %f\n", lightDuration); bad = 1; }
    reset(); recallOn = 1; resurectEffect__6Player(object);
    if (!near(lightDuration, 1.0f / 60.0f)) { std::printf("FAIL light duration recall\n"); bad = 1; }
    // Negative zero x becomes +0 after the + 0.0 step.
    reset(); { float nz[3] = {-0.0f, 5.0f, 1.0f}; std::memcpy(object + 16, nz, 12); } resurectEffect__6Player(object);
    expect("negative zero", std::to_string(std::signbit(lightPos.x)), "0");
    // Block allocation failure skips only the light effect.
    reset(); allocAnswer = nullptr; resurectEffect__6Player(object);
    expect("no block", trace, "alloc188;find:fx.lmp,spel_cas_curelw.anm;seq:39321,1;pdef:70,83,74,35,31,43,75,55,0,195,0,3,1,8;nm;");
    // The particle definition is built once across calls.
    reset(); resurectEffect__6Player(object); resurectEffect__6Player(object); expect("one ctor", std::to_string(ctorCalls), "1");
    // Shadow points: value 5000 gives cube 1.25e11 -> two particles (1.25e11 > 0, 4.4e10 > 0, then negative).
    reset(); shadows = {{{1.0f, 2.0f, 3.0f}, 5000.0f}};
    resurectEffect__6Player(object);
    expect("shadow call", std::to_string(shadowCalls) + "," + std::to_string(particles.size()), "1,2");
    expect("shadow args", trace.substr(trace.find("shadow")), "shadow:10,20,30,77,0,1;");
    // Carry-over of the cube: 3000^3 = 2.7e10 gives one particle and a deficit of 5.39e10; the next point
    // only halves it, and the third brings the sum just above zero for a second particle.
    reset(); shadows = {{{0, 0, 0}, 3000.0f}, {{0, 0, 0}, 3000.0f}};
    resurectEffect__6Player(object);
    expect("carry two", std::to_string(shadowCalls) + "," + std::to_string(particles.size()), "2,1");
    reset(); shadows = {{{0, 0, 0}, 3000.0f}, {{0, 0, 0}, 3000.0f}, {{0, 0, 0}, 3000.0f}};
    resurectEffect__6Player(object);
    expect("carry three", std::to_string(shadowCalls) + "," + std::to_string(particles.size()), "3,2");
    // Zero, small and negative values emit nothing.
    reset(); shadows = {{{0, 0, 0}, 0.0f}, {{0, 0, 0}, -100.0f}, {{0, 0, 0}, 10.0f}};
    resurectEffect__6Player(object); expect("nothing", std::to_string(particles.size()), "0");
    // One particle with chosen random numbers: jitter 40000, 20000, 65536; the first ball try (65535 x3) is rejected, the next accepted; life 49152.
    reset(); shadows = {{{1.0f, 2.0f, 3.0f}, 4000.0f}};
    rands = {40000, 20000, 65536, 65535, 65535, 65535, 49152, 16384, 32768, 49152};  // jitter x y z, rejected ball try, accepted try, lifetime
    resurectEffect__6Player(object);
    expect("count", std::to_string(particles.size()), "1");
    {
        const float scale = 4000.0f * 9.999999747378752e-06f * 35.349998474121094f;
        const float jx = (40000.0f / 65536.0f - 0.5f) * scale + 1.0f, jy = (20000.0f / 65536.0f - 0.5f) * scale + 2.0f, jz = (1.0f - 0.5f) * scale + 3.0f;
        const float ux = 49152.0f / 65536.0f * 2.0f - 1.0f, uy = 16384.0f / 65536.0f * 2.0f - 1.0f, uz = 0.0f;
        const float vx = ux * 40.96299743652344f, vy = uy * 40.96299743652344f, vz = uz * 40.96299743652344f;
        const Particle& p = particles[0];
        const bool ok = near(p.pos.x, jx + vx) && near(p.pos.y, jy + vy) && near(p.pos.z, jz + vz) &&
                        near(p.vel.x, (p.pos.x - jx) * -0.2808988690376282f * 0.5f) && near(p.vel.y, (p.pos.y - jy) * -0.2808988690376282f * 0.5f) &&
                        near(p.vel.z, (p.pos.z - jz) * -0.2808988690376282f * 0.5f) && near(p.life, 49152.0f / 65536.0f * 0.3999999761581421f);
        if (!ok) { std::printf("FAIL particle values\n"); bad = 1; }
        if (!rands.empty()) { std::printf("FAIL random use %zu left\n", rands.size()); bad = 1; }
    }
    std::printf(bad ? "FAIL\n" : "ok: resurect effect\n");
    return bad;
}
