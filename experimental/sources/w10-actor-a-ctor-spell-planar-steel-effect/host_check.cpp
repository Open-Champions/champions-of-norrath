// Host contract for __22SpellPlanarSteelEffectiP8CreatureP5Skill; not an EE input.
// The object and Creature are byte buffers, so offsets are the real ones; pointer words hold the
// low 32 bits of host addresses. The 1.0f/0.0f words are 0x7fffffff on the EE (R5900 largest value)
// and IEEE infinity on a host, so only that pair depends on the build.
#include "spell_planar_steel_effect_ctor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char vt_Spell[48];
unsigned char vt_SpellPlanarSteelEffect[48];
unsigned syncRandState;
int frameNm;
static unsigned char obj[160] __attribute__((aligned(16)));
static unsigned char cre[64] __attribute__((aligned(16)));
static std::string calls;
static unsigned char lightSeen[64];
static unsigned tableAtLight;
static std::string lumpA, lumpB;
static Point3 soundPoint;
static float soundGain;
void lightAdd__FP5Light(void* light) {
    calls += "light;";
    if (light != obj + 48) calls += "BADLIGHT;";
    std::memcpy(lightSeen, light, 64);
    std::memcpy(&tableAtLight, obj + 20, 4);
}
void* lumpFindResource__FPcT0(char* a, char* b) { calls += "lump;"; lumpA = a; lumpB = b; return reinterpret_cast<void*>(0x77); }
int soundPlaySequence__FP15AnimationHeaderG6Point3f(void* h, Point3 p, float g) {
    calls += "sound;"; soundPoint = p; soundGain = g;
    return h == reinterpret_cast<void*>(0x77) ? 1 : 0;
}
template <class T> static T get(const unsigned char* b, int off) { T v; std::memcpy(&v, b + off, sizeof v); return v; }
static unsigned lo(const void* p) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
int main() {
    int bad = 0;
#define CHECK(c, m) do { if (!(c)) { std::printf("fail: %s\n", m); bad = 1; } } while (0)
    std::memset(obj, 0xee, sizeof obj);
    float p3[3] = {1.5f, -2.0f, 3.25f}; std::memcpy(cre + 16, p3, 12);
    float scale = 100.0f; std::memcpy(cre + 32, &scale, 4);
    syncRandState = 0x12345678u; frameNm = 1000;
    __22SpellPlanarSteelEffectiP8CreatureP5Skill(obj, 0x1a7, cre, reinterpret_cast<void*>(0x40));
    unsigned next = 0x12345678u * 1664525u + 1013904223u;
    CHECK(calls == "light;lump;sound;", "call order");
    CHECK(syncRandState == next && get<unsigned>(obj, 16) == (next >> 16), "random step");
    CHECK(obj[0] == 0xa7 && get<unsigned>(obj, 4) == 0, "byte and zero word");
    CHECK(get<unsigned>(obj, 8) == 0x40 && get<unsigned>(obj, 12) == lo(cre), "skill and creature words");
    CHECK(get<unsigned>(obj, 20) == lo(vt_SpellPlanarSteelEffect) && tableAtLight == lo(vt_SpellPlanarSteelEffect), "derived table set before lightAdd");
    CHECK(get<short>(obj, 28) == 300 && get<unsigned>(obj, 32) == 0 && get<unsigned>(obj, 36) == 1240u, "post-light fields");
    CHECK(get<unsigned short>(obj, 24) == 0 && get<unsigned short>(obj, 26) == 32767, "countdown halfwords");
    CHECK(std::memcmp(obj + 96, p3, 12) == 0, "position copy");
    const unsigned short expectHalves[8] = {75, 75, 200, 255, 75, 75, 200, 255};
    CHECK(std::memcmp(lightSeen + 10, expectHalves, 16) == 0 && lightSeen[8] == 0 && lightSeen[9] == 0, "light colour halves and bytes");
    // lightSeen is the 64 bytes at obj+48, i.e. light record offsets 0..63 of the object copy.
    CHECK(get<unsigned>(lightSeen, 28) == 0 && get<unsigned>(lightSeen, 36) == 0, "light zero floats");
#if defined(__mips__)
    const unsigned fmax = 0x7fffffffu;
#else
    const unsigned fmax = 0x7f800000u;
#endif
    CHECK(get<unsigned>(lightSeen, 32) == fmax && get<unsigned>(lightSeen, 40) == fmax, "light 1/0 floats");
    CHECK(get<float>(lightSeen, 44) == 0.65f, "light 0.65f");
    CHECK(lumpA == "fx.lmp" && lumpB == "spel_gatescroll.anm", "resource names");
    CHECK(soundPoint.x == 1.5f && soundPoint.y == -2.0f && soundPoint.z == 3.25f && soundGain == 1.0f, "sound call arguments");
    // truncation toward zero for the +28 halfword
    scale = -7.9f; std::memcpy(cre + 32, &scale, 4); calls.clear();
    __22SpellPlanarSteelEffectiP8CreatureP5Skill(obj, 1, cre, nullptr);
    CHECK(get<short>(obj, 28) == -23, "float * 3 truncates toward zero");
    return bad;
}
