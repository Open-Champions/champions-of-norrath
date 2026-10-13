// Not an EE plan input. Build: c++ -std=c++17 -Wall -Wextra -Werror -ffp-contract=off host_check.cpp
// Callees are stubs that record their arguments; eRand answers from a queue (32768 when empty).
#include "player_block_sparks.cpp"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <deque>
#include <string>
#include <vector>

extern "C" {
alignas(8) unsigned char player[4 * 23984];
int syncRandState;
unsigned char trailParticleSpark[80], blueSpark[80], redSpark[80];
}
alignas(8) static unsigned char object[1400];
static std::string trace;
static std::deque<int> rands;
struct Particle { const void* def; Point3 pos, vel; float life; };
static std::vector<Particle> particles;
static Point3 attach[2];
struct Shadow { Point3 point; float value; };
static std::vector<Shadow> shadows;
static unsigned char animState[600];
static char* sprintfFormat;
extern "C" {
int sprintf(char* out, const char* format, ...) { std::strcpy(out, format); sprintfFormat = out; trace += std::string("sprintf:") + format + ";"; return 0; }
void* lumpFindResource__FPcT0(char* l, char* n) { trace += std::string("find:") + l + "," + n + ";"; return &trace; }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* s, void* o, float v) {
    trace += std::string("seq") + (s == &trace ? "" : "?") + ":" + std::to_string(reinterpret_cast<__UINTPTR_TYPE__>(o)) + "," + std::to_string((int)v) + ";";
}
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(void* m, Point3 p, short yaw, void* st, int index, Point3* out,
                                                                                         Point3* second, float scale) {
    trace += std::string("att") + (m == reinterpret_cast<void*>(0x4321) ? "" : "?") + (st == object + 288 ? "" : "?") + ":" + std::to_string((int)p.x) + "," +
             std::to_string((int)p.y) + "," + std::to_string((int)p.z) + "," + std::to_string(yaw) + "," + std::to_string(index) + (second ? "?" : "") + "," +
             std::to_string((int)scale) + ";";
    *out = attach[index];
}
int modelGetNmShadowPoints__FP12_modelHeader(void*) { return static_cast<int>(shadows.size()); }
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(void* m, void* st, Point3 p, short yaw, int index, Point3* out,
                                                                                         float* value, void* matrix, float scale) {
    trace += std::string("shadow") + (m == reinterpret_cast<void*>(0x4321) ? "" : "?") + (st == animState + 288 ? "" : "?") + ":" + std::to_string((int)p.x) +
             "," + std::to_string(yaw) + "," + std::to_string(index) + (matrix ? "?" : "") + "," + std::to_string((int)scale) + ";";
    *out = shadows[index].point;
    *value = shadows[index].value;
}
float icos__Fi(int a) { trace += "c" + std::to_string(a) + ";"; return static_cast<float>(a) / 100.0f; }
float isin__Fi(int a) { trace += "s" + std::to_string(a) + ";"; return static_cast<float>(a) / 1000.0f; }
int eRand__10GameObject(void* self) {
    if (self != object) trace += "rand?;";
    if (rands.empty()) return 32768;
    int v = rands.front(); rands.pop_front(); return v;
}
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* def, Point3* p, Point3* v, float life) { particles.push_back({def, *p, *v, life}); }
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
    unsigned char* seqObject = animState; std::memcpy(player + 2 * 23984 + 21872, &seqObject, sizeof seqObject);
    attach[0] = {2.0f, 4.0f, 6.0f}; attach[1] = {4.0f, 8.0f, 10.0f};
    rands.clear(); shadows.clear(); particles.clear(); trace.clear(); syncRandState = 0;
}

int main() {
    // Kind 0: sound, two attachments, 50 particles with a cumulative base velocity (queue empty -> unit vector (0,0,0)).
    reset(); sprintfFormat = nullptr;
    playerBlockSparks__6Playeri(object, 0);
    expect("kind 0 trace", trace,
           "sprintf:hit_mtlplate_slash_miss.anm;find:fx.lmp,hit_mtlplate_slash_miss.anm;seq:" + std::to_string(reinterpret_cast<__UINTPTR_TYPE__>(animState)) +
               ",1;att:10,20,30,77,0,1;att:10,20,30,77,1,1;c77;s77;");
    expect("kind 0 count", std::to_string(particles.size()), "50");
    {
        bool ok = true;
        for (const Particle& p : particles)
            ok = ok && p.def == trailParticleSpark && p.pos.x == 3.0f && p.pos.y == 6.0f && p.pos.z == 8.0f && p.life == 0.0f &&
                 near(p.vel.x, 0.77f * 10.0f) && near(p.vel.y, 0.077f * 10.0f) && p.vel.z == 10.0f;
        if (!ok) { std::printf("FAIL kind 0 particles\n"); bad = 1; }
    }
    // Cumulative base: unit vector (0.5, -0.5, 0.0) from rands 49152, 16384, 32768 is scaled by 5 and added each pass.
    reset(); rands = {49152, 16384, 32768};
    playerBlockSparks__6Playeri(object, 0);
    {
        const bool ok = near(particles[0].vel.x, 7.7f + 2.5f) && near(particles[0].vel.y, 0.77f - 2.5f) && near(particles[0].vel.z, 10.0f) &&
                        near(particles[1].vel.x, 7.7f + 2.5f) && near(particles[1].vel.y, 0.77f - 2.5f);
        // the second pass has the default (0,0,0) unit vector, so its base equals the first pass's base
        if (!ok) { std::printf("FAIL cumulative base\n"); bad = 1; }
    }
    // Rejection: (65535 x3) has squared length ~3 > 1 and is redrawn; the accepted vector comes from the next three.
    reset(); rands = {65535, 65535, 65535, 49152, 49152, 49152};
    playerBlockSparks__6Playeri(object, 0);
    expect("rejected try consumed", std::to_string(particles.size()), "50");
    if (!near(particles[0].vel.x, 7.7f + 2.5f) || !near(particles[0].vel.z, 10.0f + 2.5f)) { std::printf("FAIL rejection\n"); bad = 1; }
    // Kind 3 (or any other) with shadow points does nothing but ask for them.
    reset(); shadows = {{{1, 2, 3}, 1.0f}}; playerBlockSparks__6Playeri(object, 3);
    expect("kind 3", std::to_string(particles.size()), "0");
    expect("kind 3 trace has no sound", std::to_string(trace.find("seq") == std::string::npos) + ":" + trace.substr(trace.find("shadow")), "1:shadow:10,77,0,1;");
    // Kind 1: five blue particles per shadow point; syncRandState advances 3 steps per try plus 1 per particle.
    reset(); shadows = {{{1.0f, 2.0f, 3.0f}, 1.0f}, {{4.0f, 5.0f, 6.0f}, 1.0f}}; syncRandState = 12345;
    playerBlockSparks__6Playeri(object, 1);
    expect("kind 1 count", std::to_string(particles.size()), "10");
    {
        // Replay the generator independently.
        unsigned state = 12345;
        auto step = [&]() { state = state * 0x19660du + 0x3c6ef35fu; return state >> 16; };
        bool ok = true;
        for (int pi = 0; pi < 2; ++pi) {
            const Point3 pt = pi == 0 ? Point3{1.0f, 2.0f, 3.0f} : Point3{4.0f, 5.0f, 6.0f};
            for (int n = 0; n < 5; ++n) {
                float ux, uy, uz;
                do {
                    ux = step() / 65536.0f * 2.0f - 1.0f; uy = step() / 65536.0f * 2.0f - 1.0f; uz = step() / 65536.0f * 2.0f - 1.0f;
                } while (ux * ux + uy * uy + uz * uz > 1.0f);
                const float life = step() / 65536.0f * 0.09999999403953552f;
                const Particle& p = particles[pi * 5 + n];
                ok = ok && p.def == blueSpark && p.pos.x == pt.x && p.pos.y == pt.y && p.pos.z == pt.z &&
                     near(p.vel.x, (pt.x - 10.0f) * 0.3489999771118164f + ux * 4.769999980926514f) &&
                     near(p.vel.y, (pt.y - 20.0f) * 0.3489999771118164f + uy * 4.769999980926514f) &&
                     near(p.vel.z, (pt.z - 66.0f) * 0.3489999771118164f + uz * 4.769999980926514f) && near(p.life, life);
            }
        }
        ok = ok && static_cast<unsigned>(syncRandState) == state;
        if (!ok) { std::printf("FAIL kind 1 values\n"); bad = 1; }
    }
    // Kind 2 uses the red definition.
    reset(); shadows = {{{1.0f, 2.0f, 3.0f}, 1.0f}}; syncRandState = 7;
    playerBlockSparks__6Playeri(object, 2);
    expect("kind 2", std::to_string(particles.size()) + std::to_string(particles[0].def == redSpark), "51");
    std::printf(bad ? "FAIL\n" : "ok: player block sparks\n");
    return bad;
}
