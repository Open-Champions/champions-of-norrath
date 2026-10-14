// Host contract for __18SpellAncestralCalliP8CreatureP5Skill; not an EE input. Stored 32-bit addresses are offsets into an
// arena (NORRATH_HOST_ARENA). Checks the stored fields, the class-byte driven animation name for every listed class value and
// for unlisted ones, the early return on the creature flag, the light-effect arguments, the failed allocation path and the
// two skill-table calls with their float conversions.
#define NORRATH_HOST_ARENA 1
#include "ctor.cpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

alignas(16) static unsigned char arena[0x4000];
static const unsigned base = 0x100000u;
unsigned char vt_Spell[8];
unsigned char vt_AncestralCall[8];
unsigned char player[4 * 23984];
unsigned syncRandState;
unsigned frameNm;
static const unsigned fnAddress = 0x7f0000u, spellAddress = 0x7f0010u, ancestralAddress = 0x7f0020u;
static std::string calls; static int allocSize; static bool failAlloc;
static std::vector<std::string> lumpNames, lumpArgs; static std::vector<std::string> formats;
static void* soundHeader; static void* soundObject; static float soundVolume;
static unsigned char* animState; static void* animHeader; static float animA, animB; static int animId;
struct Light { unsigned char* self; Point3 point; float f[12]; void* object; int flag; };
static Light light;
static int slotCalls; static unsigned char* slotSeenThis; static int slotSeenA[2], slotSeenB[2]; static int slotResults[2];

static int slotTarget(void* self, int a, int b) {
    calls += "slot;"; slotSeenThis = static_cast<unsigned char*>(self);
    slotSeenA[slotCalls] = a; slotSeenB[slotCalls] = b; return slotResults[slotCalls++];
}
extern "C" void* lumpFindResource__FPcT0(char* lump, char* name) {
    calls += "lump;"; lumpNames.push_back(lump); lumpArgs.push_back(name); return arena + 0x3000 + 0x100 * lumpNames.size();
}
extern "C" void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sound, void* object, float volume) {
    calls += "sound;"; soundHeader = sound; soundObject = object; soundVolume = volume;
}
extern "C" int sprintf(char* buffer, const char* format, ...) { calls += "fmt;"; formats.push_back(format); std::strcpy(buffer, format); return 0; }
extern "C" void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float a, float b, int id) {
    calls += "anim;"; animState = static_cast<unsigned char*>(state); animHeader = header; animA = a; animB = b; animId = id;
}
extern "C" void* blockAlloc__Fi(int size) { calls += "alloc;"; allocSize = size; return failAlloc ? nullptr : arena + 0x3800; }
extern "C" void __11LightEffectG6Point3ffffffffffffP10GameObjecti(void* self, Point3 point, float a, float b, float c, float d, float e, float f,
                                                                float g, float h, float i, float j, float k, float l, void* object, int flag) {
    calls += "light;"; light.self = static_cast<unsigned char*>(self); light.point = point;
    const float v[12] = {a, b, c, d, e, f, g, h, i, j, k, l}; std::memcpy(light.f, v, sizeof v); light.object = object; light.flag = flag;
}
extern "C" void* P_CreateParticleSystem__Fv() { calls += "particle;"; return arena + 0x3c00; }
extern "C" unsigned char* norrath_host_pointer(unsigned a) {
    if (a == fnAddress) return reinterpret_cast<unsigned char*>(slotTarget);
    return arena + (a - base);
}
extern "C" unsigned norrath_host_address(const void* p) {
    const unsigned char* q = static_cast<const unsigned char*>(p);
    if (q >= arena && q < arena + sizeof arena) return base + static_cast<unsigned>(q - arena);
    if (q == vt_Spell) return spellAddress;
    if (q == vt_AncestralCall) return ancestralAddress;
    return 0xdeadbeefu;
}
static unsigned word(const unsigned char* p) { unsigned v; std::memcpy(&v, p, 4); return v; }
static float flt(const unsigned char* p) { float v; std::memcpy(&v, p, 4); return v; }
static unsigned addr(const unsigned char* p) { return norrath_host_address(p); }

int main() {
    int bad = 0;
    unsigned char* const object = arena + 0x100;
    unsigned char* const creature = arena + 0x400;   // 1408+ bytes
    unsigned char* const skill = arena + 0xa00;
    unsigned char* const table = arena + 0xb00;
    unsigned char* const owner = arena + 0xc00;
    unsigned char* const names = arena + 0xd00;
    const int kinds[] = {0, 5, 2, 1, 3, 7, 4, -128, -1, 6};
    const char* const expected[] = {"ohw_warcry.anm", "nw_warcry.anm", "dha_warcry.anm", "dhs_warcry.anm", "l_warcry.anm", "dw_warcry.anm",
                                    "nw_warcry.anm", "nw_warcry.anm", "nw_warcry.anm", "nw_warcry.anm"};
    const unsigned seeds[] = {0x12345678u, 0xffffffffu};
    const short adjusts[] = {-24, 64};
    const int results[][2] = {{0, 0}, {5, -3}, {-7, 100}};
    const unsigned ownerFlags[] = {0u, 0x20000u, 0xfffdffffu, 0xffffffffu};
    const unsigned indexes[] = {0u, 3u};
    for (unsigned seed : seeds) for (int ki = 0; ki < 10; ++ki) for (short adj : adjusts) for (const auto& res : results)
    for (unsigned flags : ownerFlags) for (unsigned index : indexes) for (int fail = 0; fail < 2; ++fail) {
        std::memset(arena, 0x5a, sizeof arena);
        std::memset(player, 0x11, sizeof player);
        player[index * 23984u + 23582u] = static_cast<unsigned char>(kinds[ki]);
        for (int i = 0; i < 12; ++i) creature[16 + i] = 0;
        const float cx = 10.5f, cy = -3.0f, cz = 77.25f;
        std::memcpy(creature + 16, &cx, 4); std::memcpy(creature + 20, &cy, 4); std::memcpy(creature + 24, &cz, 4);
        const unsigned t = addr(table), ow = addr(owner), nm = addr(names);
        std::memcpy(skill + 96, &t, 4); std::memcpy(table + 88, &adj, 2); std::memcpy(table + 92, &fnAddress, 4);
        std::memcpy(creature + 36, &ow, 4); std::memcpy(owner, &nm, 4); std::memcpy(names, "fx.lmp", 7);
        std::memcpy(creature + 960, &index, 4); std::memcpy(creature + 12, &flags, 4);
        calls.clear(); lumpNames.clear(); lumpArgs.clear(); formats.clear(); failAlloc = fail != 0; allocSize = 0; slotCalls = 0;
        slotResults[0] = res[0]; slotResults[1] = res[1]; animState = nullptr; light.self = nullptr; slotSeenThis = nullptr;
        syncRandState = seed; frameNm = 0x4321u + seed % 17;
        const unsigned next = seed * 0x19660du + 0x3c6ef35fu;
        __18SpellAncestralCalliP8CreatureP5Skill(object, 0x1a7, creature, skill);

        if (syncRandState != next || word(object + 16) != (next >> 16)) { std::puts("fail: random"); bad = 1; }
        if (object[0] != 0xa7 || word(object + 4) != 0u || word(object + 28) != 0u) { std::puts("fail: base"); bad = 1; }
        if (word(object + 8) != addr(skill) || word(object + 12) != addr(creature) || word(object + 24) != addr(creature) || word(object + 20) != ancestralAddress) { std::puts("fail: fields"); bad = 1; }
        if (lumpNames.size() < 2 || lumpNames[0] != "fx.lmp" || lumpArgs[0] != "spel_bw_ancestral.anm" || soundHeader != arena + 0x3100 || soundObject != creature || soundVolume != 1.0f) { std::puts("fail: first sound"); bad = 1; continue; }
        if (formats.size() != 1 || formats[0] != expected[ki]) { std::printf("fail: name for kind %d\n", kinds[ki]); bad = 1; continue; }
        if (lumpNames[1] != "fx.lmp" || lumpArgs[1] != expected[ki] || animState != creature + 288 || animHeader != arena + 0x3200 ||
            animA != animB || word(reinterpret_cast<const unsigned char*>(&animA)) != 0x3dccccccu || animId != 34) { std::puts("fail: animation"); bad = 1; }
        if (flags & 0x20000u) {
            if (calls != "lump;sound;fmt;lump;anim;") { std::printf("fail: early calls %s\n", calls.c_str()); bad = 1; }
            continue;
        }
        const std::string want = std::string("lump;sound;fmt;lump;anim;alloc;") + (fail ? "" : "light;") + "slot;slot;particle;";
        if (calls != want) { std::printf("fail: calls %s\n", calls.c_str()); bad = 1; continue; }
        if (allocSize != 188) { std::puts("fail: alloc size"); bad = 1; }
        if (!fail) {
            const float wantF[12] = {0.0f, 155.0f, 255.0f, -96.0f, 0.0f, 0.0f, 0.0f, 0.0f, 180.0f, 144.0f, 0.0f, 5.0f};
            if (light.self != arena + 0x3800 || light.point.x != cx + 0.0f || light.point.y != cy || light.point.z != cz + 36.0f ||
                std::memcmp(light.f, wantF, sizeof wantF) != 0 || light.object != creature || light.flag != 0) { std::puts("fail: light effect"); bad = 1; }
        }
        if (word(creature + 1400) != 6u || word(object + 124) != 0u || word(object + 40) != frameNm || word(object + 52) != 0u || word(object + 48) != 0u || word(object + 44) != 0u) { std::puts("fail: state words"); bad = 1; }
        if (slotSeenThis != skill + adj || slotSeenA[0] != 2 || slotSeenA[1] != 3 || slotSeenB[0] != static_cast<signed char>(0xa7) || slotSeenB[1] != static_cast<signed char>(0xa7)) { std::puts("fail: slot args"); bad = 1; }
        if (flt(object + 32) != static_cast<float>(res[0]) || flt(object + 36) != static_cast<float>(res[1] * 12)) { std::puts("fail: floats"); bad = 1; }
        if (word(object + 128) != addr(arena + 0x3c00)) { std::puts("fail: particle"); bad = 1; }
    }
    if (bad == 0) std::puts("ok");
    return bad;
}
