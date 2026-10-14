// Host contract for run__19SpellHornOmenEffectG6Point3s; not an EE input. 32-bit words are offsets from a host base
// and the callees log. The authored function was also compared with the original in a private R5900 run (37
// scenarios over the remaining time around the 140-frame stage change, the end of the effect and zero shadow
// points, with the owner's colour halfwords compared; every one of its 513 words reached).
#define W12_SPELL_C_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w12_spell_c_host_base;
static std::string calls;
unsigned w12_spell_c_host_call(unsigned function_word, void* adjusted_object, int argument_count, unsigned long a1, unsigned long) {
    if (function_word == 0x128) { calls += "s128;"; return 0x77; }
    calls += "end(" + std::to_string(static_cast<unsigned char*>(adjusted_object) - w12_spell_c_host_base) + "," + std::to_string(argument_count) + "," + std::to_string(a1) + ");";
    return 0;
}
extern "C" { int frameNm; unsigned char hornomen_run_particle_8d8650[80]; }
#include "run.cpp"

static int pointCount, particles, shadowCalls;
static int rollValue;
extern "C" {
int eRand__5Spell(void*) { return rollValue; }
void matMakeTransYaw__FP8Matrix34G6Point3s(Matrix34*, Point3 p, short yaw) { calls += "mat(" + std::to_string(static_cast<int>(p.x)) + "," + std::to_string(yaw) + ");"; }
void P_SetParticleSystemMatrix__FiP8Matrix34(int s, Matrix34*) { calls += "setm(" + std::to_string(s) + ");"; }
void P_OpenParticleSystem__Fi(int s) { calls += "open(" + std::to_string(s) + ");"; }
void P_CloseParticleSystem__Fi(int s) { calls += "close(" + std::to_string(s) + ");"; }
int modelGetNmShadowPoints__FP12_modelHeader(void*) { return pointCount; }
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(void*, void*, Point3, short, int index, Point3* point, float*, void*, float) {
    ++shadowCalls; point->x = static_cast<float>(index); point->y = 2.0f; point->z = 3.0f;
}
void P_AddParticle__FP11ParticleDefP6Point3T1f(void*, Point3* p, Point3* v, float) {
    ++particles;
    if (particles == 1) calls += "particle(" + std::to_string(p->x) + ":" + std::to_string(p->y) + ":" + std::to_string(p->z) + "," + std::to_string(v->x) + ":" + std::to_string(v->y) + ":" + std::to_string(v->z) + ");";
}
}

static unsigned char arena[0x2000] __attribute__((aligned(16)));
static unsigned char* const effect = arena + 0x100;
static unsigned char* const owner = arena + 0x400;
static unsigned char* const ownerTable = arena + 0x800;
static unsigned char* const effectTable = arena + 0x900;
static void put32(unsigned char* p, unsigned v) { std::memcpy(p, &v, 4); }
static void putf(unsigned char* p, float v) { std::memcpy(p, &v, 4); }
static short get16(unsigned char* p) { short v; std::memcpy(&v, p, 2); return v; }
static void reset(int remaining, int points) {
    w12_spell_c_host_base = arena;
    std::memset(arena, 0, sizeof arena);
    calls.clear(); pointCount = points; particles = 0; shadowCalls = 0; rollValue = 32768;
    frameNm = 1000;
    put32(effect + 24, static_cast<unsigned>(1000 + remaining)); put32(effect + 12, static_cast<unsigned>(owner - arena)); put32(effect + 28, 5); put32(effect + 20, static_cast<unsigned>(effectTable - arena));
    short adjust = -4; std::memcpy(effectTable + 8, &adjust, 2); put32(effectTable + 12, 0x1);
    put32(owner + 72, static_cast<unsigned>(ownerTable - arena)); std::memcpy(ownerTable + 128, &adjust, 2); put32(ownerTable + 132, 0x128);
    putf(owner + 16, 7); putf(owner + 20, 8); putf(owner + 24, 9);
    short yaw = 123; std::memcpy(owner + 56, &yaw, 2);
}

int main() {
    int bad = 0;
    reset(0, 3);
    run__19SpellHornOmenEffectG6Point3s(effect, Point3{0, 0, 0}, 0);
    if (calls != "end(" + std::to_string(effect - arena - 4) + ",2,3);") { std::puts(calls.c_str()); std::puts("fail: end of effect"); bad = 1; }
    // remaining 100: elapsed 500 (late stage): constant colours, 150 particles, rolls of 32768 give the centre of the ball
    reset(100, 3);
    run__19SpellHornOmenEffectG6Point3s(effect, Point3{0, 0, 0}, 0);
    if (calls.find("mat(7,123);setm(5);open(5);") == std::string::npos) { std::puts(calls.c_str()); std::puts("fail: opening"); bad = 1; }
    if (particles != 150 || shadowCalls != 150 || calls.find("close(5);") == std::string::npos) { std::printf("fail: late stage %d particles\n", particles); bad = 1; }
    if (get16(owner + 140) != 225 || get16(owner + 142) != 20 || get16(owner + 144) != 0 || get16(owner + 146) != 100 || get16(owner + 148) != 225 || get16(owner + 150) != 20 ||
        get16(owner + 152) != 0 || get16(owner + 154) != 100) { std::puts("fail: late colours"); bad = 1; }
    // remaining 570: elapsed 30 (early stage): s = 30/140 -> colours truncated; particles = while float(i + 1) < s * 150 = 32.14
    reset(570, 3);
    run__19SpellHornOmenEffectG6Point3s(effect, Point3{0, 0, 0}, 0);
    if (particles != 33) { std::printf("fail: early particle count %d\n", particles); bad = 1; }
    if (get16(owner + 140) != 48 || get16(owner + 142) != 4 || get16(owner + 146) != 21 || get16(owner + 148) != 48 || get16(owner + 150) != 4 || get16(owner + 154) != 21) {
        std::printf("fail: early colours %d %d %d\n", get16(owner + 140), get16(owner + 142), get16(owner + 146)); bad = 1;
    }
    // no shadow points: no particles, but the system is opened and closed
    reset(100, 0);
    run__19SpellHornOmenEffectG6Point3s(effect, Point3{0, 0, 0}, 0);
    if (particles != 0 || calls.find("open(5);") == std::string::npos || calls.find("close(5);") == std::string::npos) { std::puts("fail: no points"); bad = 1; }
    // the first particle: index 32768 % 3 = 2 -> point (2, 2, 3) moved by (0, 0, 0) (ball centre)
    reset(100, 3);
    run__19SpellHornOmenEffectG6Point3s(effect, Point3{0, 0, 0}, 0);
    if (calls.find("particle(2.000000:2.000000:3.000000,0.000000:0.000000:0.000000);") == std::string::npos) { std::puts(calls.c_str()); std::puts("fail: first particle"); bad = 1; }
    return bad;
}
