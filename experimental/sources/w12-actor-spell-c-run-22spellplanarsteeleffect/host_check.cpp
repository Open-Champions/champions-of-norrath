// Host contract for run__22SpellPlanarSteelEffectG6Point3s; not an EE input. 32-bit words are offsets from a host
// base and the callees log. The authored function was also compared with the original in a private R5900 run (42
// scenarios: the end of the effect and its edge, the countdown crossing zero, the done flag, several frames and
// byte values, with the owner's colour halfwords compared; 653 of 654 original words reached, the missing one being
// padding).
#define W12_SPELL_C_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w12_spell_c_host_base;
static std::string calls;
unsigned w12_spell_c_host_call(unsigned function_word, void* adjusted_object, int argument_count, unsigned long a1, unsigned long a2) {
    calls += "end(" + std::to_string(function_word) + "," + std::to_string(static_cast<unsigned char*>(adjusted_object) - w12_spell_c_host_base) + "," +
             std::to_string(argument_count) + "," + std::to_string(a1) + "," + std::to_string(a2) + ");";
    return 0;
}
extern "C" {
int frameNm;
void* world = reinterpret_cast<void*>(0x55);
void* haloTexture5 = reinterpret_cast<void*>(0x66);
unsigned char planarsteel_run_particle_8d8510[80];
unsigned char planarsteel_run_particle_8d8560[80];
unsigned char planarsteel_run_particle_8d85b0[80];
unsigned char planarsteel_run_particle_8d8600[80];
}
#include "run.cpp"

static int rollValue;
static int p1, p2, p3, p5, faces, lights, sounds, heights;
static std::string num(float v) { char b[32]; std::snprintf(b, sizeof b, "%g", v); return b; }
extern "C" {
int eRand__5Spell(void*) { return rollValue; }
float icos__Fi(int) { return 0.5f; }
float isin__Fi(int) { return 0.25f; }
void lightUpdatePosition__FP5Light(void*) { ++lights; }
void* lumpFindResource__FPcT0(char* a, char* b) { calls += std::string("lump(") + a + "," + b + ");"; return reinterpret_cast<void*>(0x77); }
void soundPlaySequence__FP15AnimationHeaderG6Point3f(void*, Point3 p, float v) { ++sounds; calls += "sound(" + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + "," + num(v) + ");"; }
void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(Point3 p, float a, float b, int c1, int c2, int c3, int c4, void* t, short sp, int fl) {
    ++faces;
    calls += "face(" + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + "," + num(a) + "," + num(b) + "," + std::to_string(c1) + "," + std::to_string(c2) + "," + std::to_string(c3) + "," +
             std::to_string(c4) + "," + std::to_string(t == haloTexture5) + "," + std::to_string(sp) + "," + std::to_string(fl) + ");";
}
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3* p, Point3* v, float) {
    if (d == planarsteel_run_particle_8d85b0) ++p3; else if (d == planarsteel_run_particle_8d8560) ++p2; else if (d == planarsteel_run_particle_8d8600) ++p5; else ++p1;
    if (d == planarsteel_run_particle_8d8600 && p5 == 1) calls += "p5(" + num(p->x) + ":" + num(p->y) + ":" + num(p->z) + "," + num(v->x) + ":" + num(v->y) + ":" + num(v->z) + ");";
}
float worldFindPointHeight__FP12_worldHeaderG6Point3(void*, Point3 p) { ++heights; calls += "height(" + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + ");"; return 3.0f; }
}

static unsigned char arena[0x2000] __attribute__((aligned(16)));
static unsigned char* const effect = arena + 0x100;
static unsigned char* const owner = arena + 0x600;
static unsigned char* const effectTable = arena + 0x900;
static void put32(unsigned char* p, unsigned v) { std::memcpy(p, &v, 4); }
static void putf(unsigned char* p, float v) { std::memcpy(p, &v, 4); }
static float getf(unsigned char* p) { float v; std::memcpy(&v, p, 4); return v; }
static unsigned get32(unsigned char* p) { unsigned v; std::memcpy(&v, p, 4); return v; }
static short get16(unsigned char* p) { short v; std::memcpy(&v, p, 2); return v; }
static void reset(int frame, int end, short timer, unsigned done) {
    w12_spell_c_host_base = arena;
    std::memset(arena, 0, sizeof arena);
    calls.clear(); rollValue = 32768; p1 = p2 = p3 = p5 = faces = lights = sounds = heights = 0;
    frameNm = frame;
    put32(effect + 36, static_cast<unsigned>(end)); put32(effect + 12, static_cast<unsigned>(owner - arena)); put32(effect + 20, static_cast<unsigned>(effectTable - arena));
    std::memcpy(effect + 28, &timer, 2); put32(effect + 32, done); effect[0] = 4;
    short adjust = -4; std::memcpy(effectTable + 8, &adjust, 2); put32(effectTable + 12, 0x1);
    putf(owner + 16, 10); putf(owner + 20, 20); putf(owner + 24, 30);
}

int main() {
    int bad = 0;
    const Point3 p{1.0f, 2.0f, 3.0f};
    reset(100, 100, 5, 0);   // not below the end frame
    run__22SpellPlanarSteelEffectG6Point3s(effect, p, 0);
    if (calls != "end(1," + std::to_string(effect - arena - 4) + ",2,3,0);" || faces != 0) { std::puts(calls.c_str()); std::puts("fail: end of effect"); bad = 1; }
    // running: frame 0 gives isin(0) = 0.25 and the stub trig is constant; timer 5 -> 4 (not negative): no burst
    reset(0, 10, 5, 0);
    run__22SpellPlanarSteelEffectG6Point3s(effect, p, 0);
    if (lights != 1 || get32(effect + 96) != get32(owner + 16) || get32(effect + 100) != get32(owner + 20) || get32(effect + 104) != get32(owner + 24)) { std::puts("fail: light position"); bad = 1; }
    // g = 4 + 66 = 70; level = 70 + 0.25 * 10 = 72.5; inverse = 1 / 72.5^2
    if (getf(effect + 76) != 72.5f || getf(effect + 84) != 72.5f || getf(effect + 80) != 1.0f / (72.5f * 72.5f) || getf(effect + 88) != 1.0f / (72.5f * 72.5f)) { std::puts("fail: level"); bad = 1; }
    // colours: 0.25 * 49 = 12.25 -> 212, 87; 0.25 * 5 = 1.25 -> 16
    if (get16(owner + 152) != 212 || get16(owner + 144) != 212 || get16(owner + 154) != 16 || get16(owner + 146) != 16 || get16(owner + 150) != 87 || get16(owner + 140) != 87 ||
        get16(owner + 148) != 87 || get16(owner + 142) != 87) { std::puts("fail: colours"); bad = 1; }
    if (get16(effect + 28) != 4 || p3 != 0 || sounds != 0) { std::puts("fail: no burst yet"); bad = 1; }
    if (faces != 2 || p1 != 22 || p2 != 8 || p5 != 10 || heights != 10) { std::printf("fail: counts %d %d %d %d %d\n", faces, p1, p2, p5, heights); bad = 1; }
    // the first sprite: angle -1500 -> pos (10 + 0.5 * 40, 20 + 0.25 * 40, 30 + 4) = (30, 30, 34); c = (int)(0.25 * 40 + 50) = 60, d = 110
    if (calls.find("face(30:30:34,0,4000,60,60,110,60,1,0,2);") == std::string::npos) { std::puts(calls.c_str()); std::puts("fail: first sprite"); bad = 1; }
    // the first fall particle: point (0.5 * r + 1, 0.25 * r + 2, 200 + 8 + (-100) + 3) with r = 32768 / 65536 * 40 = 20
    if (calls.find("p5(11:7:271,0:0:-80);") == std::string::npos) { std::puts(calls.c_str()); std::puts("fail: first fall particle"); bad = 1; }
    // the burst: timer 0 -> -1, done flag zero: sixty particles, the sound, the done flag
    reset(0, 10, 0, 0);
    run__22SpellPlanarSteelEffectG6Point3s(effect, p, 0);
    if (p3 != 60 || sounds != 1 || get32(effect + 32) != 1 || calls.find("lump(fx.lmp,spel_fire_hit.anm);sound(10:20:30,1);") == std::string::npos) { std::puts(calls.c_str()); std::puts("fail: burst"); bad = 1; }
    reset(0, 10, 0, 1);
    run__22SpellPlanarSteelEffectG6Point3s(effect, p, 0);
    if (p3 != 0 || sounds != 0) { std::puts("fail: burst repeated"); bad = 1; }
    return bad;
}
