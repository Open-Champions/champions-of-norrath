// Host contract for msg_run__14FireStormFlare; not an EE input. 32-bit words are offsets from a host base; stubs record calls and
// every random draw returns 32768, so each ball is the zero vector. The authored function was also compared with the original in a
// private R5900 run (120 scenarios with random flags, end frames, sizes, ring radii, heights and rolls, including rejected balls;
// every original word covered except two alignment no-ops).
#define W12_SPELL_C_HOST 1
#include "msg_run.cpp"
#include <cstring>
#include <string>
unsigned char* w12_spell_c_host_base;
int frameNm;
void* world;
unsigned char firestormParticle1[80], firestormParticle2[80], firestormParticle3[80];
static std::string trace;
static float height, last_motion_sum;
static int rolls;
unsigned w12_spell_c_host_call(unsigned fn, void*, int count, unsigned long a1, unsigned long) {
    trace += "call" + std::to_string(fn) + "," + std::to_string(count) + "," + std::to_string(a1) + ";";
    return 0;
}
extern "C" {
int eRand__10GameObject(void*) { ++rolls; return 32768; }
float icos__Fi(int) { return 1.0f; }
float isin__Fi(int) { return 0.5f; }
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3* p, Point3* v, float age) {
    trace += d == firestormParticle1 ? "p1" : d == firestormParticle2 ? "p2" : d == firestormParticle3 ? "p3" : "p?";
    trace += "(" + std::to_string(p->x) + "," + std::to_string(p->y) + "," + std::to_string(p->z) + ";" + std::to_string(v->x + v->y + v->z) +
             ";" + std::to_string(age) + ");";
}
float worldFindHeight__FP12_worldHeaderG6Point3f(void*, Point3, float) { trace += "h;"; return height; }
void* lumpFindResource__FPcT0(const char* a, const char* b) { trace += std::string("lump:") + a + ":" + b + ";"; return nullptr; }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void*, void*, float v) { trace += "snd" + std::to_string(v) + ";"; }
void floorAdd__FG6Point3sfii(Point3 p, short a, float s, int b, int c) {
    trace += "floor(" + std::to_string(p.z) + "," + std::to_string(a) + "," + std::to_string(s) + "," + std::to_string(b) + "," + std::to_string(c) + ");";
}
}
static unsigned char arena[0x1000] __attribute__((aligned(16)));
static void put32(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static void putf(unsigned off, float v) { std::memcpy(arena + off, &v, 4); }
static float getf(unsigned off) { float v; std::memcpy(&v, arena + off, 4); return v; }
static unsigned get32(unsigned off) { unsigned v; std::memcpy(&v, arena + off, 4); return v; }
enum { O = 0x100, T = 0x800 };
static void reset(float size) {
    std::memset(arena, 0, sizeof arena);
    trace.clear(); rolls = 0;
    putf(O + 16, 1.0f); putf(O + 20, 2.0f); putf(O + 24, 30.0f);
    putf(O + 96, 0.5f); putf(O + 100, 0.25f); putf(O + 104, 1.0f);
    putf(O + 80, size); putf(O + 28, 2.0f);
    put32(O + 108, 100);
    put32(O + 72, T);
    const short adjust = 16;
    std::memcpy(arena + T + 8, &adjust, 2);
    put32(T + 8 + 4, 0x4242);
    frameNm = 50; height = 0.0f;
}
int main() {
    w12_spell_c_host_base = arena;
    // live phase, no particles (size 0): gravity and move; high above the ground, so nothing else
    reset(0.0f);
    msg_run__14FireStormFlare(arena + O);
    if (getf(O + 104) != 1.0f + __builtin_bit_cast(float, 0xbecccccc) || getf(O + 16) != 1.5f || getf(O + 20) != 2.25f ||
        getf(O + 24) != 30.0f + getf(O + 104) || trace != "h;" || get32(O + 88) != 0 || rolls != 0) return 1;
    // live phase near the ground: flag set, sound looked up and played
    reset(0.0f); height = 20.0f;
    msg_run__14FireStormFlare(arena + O);
    if (trace != "h;lump:fx.lmp:spel_ew_flares_hit.anm;snd1.000000;" || get32(O + 88) != 1) return 2;
    // live phase with size 0.1: ceil(3.0), ceil(1.8), ceil(0.4) iterations of the three loops, two balls each
    reset(0.1f);
    msg_run__14FireStormFlare(arena + O);
    {
        std::string want;
        std::string t = trace;
        int p1 = 0, p2 = 0, p3 = 0;
        for (size_t i = 0; (i = t.find('p', i)) != std::string::npos; ++i) (t[i + 1] == '1' ? p1 : t[i + 1] == '2' ? p2 : p3)++;
        if (p1 != 3 || p2 != 2 || p3 != 1 || rolls != 2 * 3 * 3 + 2 * 2 * 3 + 2 * 1 * 3) return 3;
    }
    // ended (frameNm past the end frame) with R large: no ring, an end step: no floor roll for 32768 % 100 = 68, delete call
    reset(1.0f); frameNm = 100; arena[O + 92] = 0xf4; arena[O + 93] = 0x01;  // R = 500 > (2 * 3) * 1
    msg_run__14FireStormFlare(arena + O);
    if (trace != "call16962,1,3;" || rolls != 1) return 4;
    // ended with R small: it grows by 7 and 85 rings of two particles each are added around pos (balls are zero): the first point is (cos * 8 + 1, sin * 8 + 2, 30)
    reset(1.0f); frameNm = 100; arena[O + 92] = 1; arena[O + 93] = 0;
    msg_run__14FireStormFlare(arena + O);
    if (arena[O + 92] != 8 || arena[O + 93] != 0 || rolls != 85 * 7) return 5;
    if (trace.compare(0, 50, "p1(9.000000,6.000000,30.000000;0.000000;0.000000);") != 0) return 6;
    // flag set also ends the live phase
    reset(1.0f); put32(O + 88, 1); arena[O + 92] = 0xf4; arena[O + 93] = 0x01;
    msg_run__14FireStormFlare(arena + O);
    return trace == "call16962,1,3;" ? 0 : 7;
}
