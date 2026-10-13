// Host contract for __15SpellHolyShieldiP8CreatureP5Skill; not an EE input. The EE image stores 32-bit addresses;
// this host build uses 64-bit pointers, so the skill's table and the call target are read at pointer width and
// the object fields are compared with low(). The particle system and the shaft are host stubs.
#include "spell_holy_shield_ctor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char vt_SpellHolyShield[8];
unsigned syncRandState;
static std::string calls;
static void* blockResult;
static void* lumpResult = reinterpret_cast<void*>(0x7007);
static Point3 shaftPoint;
static int shaftArgs[4];
static int slotResult = -9;
static void* slotSeenThis;
static int slotSeenA, slotSeenB;
static void* particleResult = reinterpret_cast<void*>(0x0badf00d);
static void* soundHeader;
static void* soundObject;
static float soundVolume;

void* P_CreateParticleSystem__Fv(void) { calls += "particle;"; return particleResult; }
void* lumpFindResource__FPcT0(char* lump, char* resource) {
    calls += "lump:"; calls += lump; calls += ":"; calls += resource; calls += ";";
    return lumpResult;
}
int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume) {
    calls += "sound;"; soundHeader = header; soundObject = object; soundVolume = volume; return 0x4242;
}
void* blockAlloc__Fi(int size) { calls += "alloc" + std::to_string(size) + ";"; return blockResult; }
void glowing_shaft_ctor(void* self, Point3 point, int a, int b, int c, int d) {
    calls += "shaft;";
    if (self != blockResult) calls += "shaft-block-wrong;";
    shaftPoint = point; shaftArgs[0] = a; shaftArgs[1] = b; shaftArgs[2] = c; shaftArgs[3] = d;
}
typedef int (*SlotFn)(void*, int, int);
static int slotTarget(void* self, int a, int b) {
    calls += "slot;"; slotSeenThis = self; slotSeenA = a; slotSeenB = b; return slotResult;
}

int main() {
    int bad = 0;
    alignas(16) unsigned char object[128];
    alignas(16) unsigned char skillBuffer[256];
    unsigned char* const skill = skillBuffer + 32;
    alignas(16) unsigned char table[128];
    alignas(16) unsigned char creature[64];
    alignas(16) unsigned char block[92];
    std::memset(skillBuffer, 0, sizeof skillBuffer);
    std::memset(table, 0, sizeof table);
    const unsigned seed = 0x2c6ef35fu;
    const unsigned next = seed * 0x19660du + 0x3c6ef35fu;
    const SlotFn fn = slotTarget;
    const void* tablePtr = table;
    std::memcpy(skill + 96, &tablePtr, sizeof tablePtr);
    const short adjust = 40;
    std::memcpy(table + 88, &adjust, 2);
    std::memcpy(table + 92, &fn, sizeof fn);
    const Point3 at{-2.0f, 0.5f, 7.25f};
    std::memcpy(creature + 16, &at, 12);
    for (int kind : {0x44, -5}) {
        for (void* blk : {static_cast<void*>(nullptr), static_cast<void*>(block)}) {
            calls.clear();
            blockResult = blk;
            std::memset(object, 0xee, sizeof object);
            syncRandState = seed;
            __15SpellHolyShieldiP8CreatureP5Skill(object, kind, creature, skill);

            unsigned w8, w12, w16, w20, w24, w28, w36;
            std::memcpy(&w8, object + 8, 4); std::memcpy(&w12, object + 12, 4); std::memcpy(&w16, object + 16, 4);
            std::memcpy(&w20, object + 20, 4); std::memcpy(&w24, object + 24, 4); std::memcpy(&w28, object + 28, 4);
            std::memcpy(&w36, object + 36, 4);
            if (syncRandState != next || w16 != (next >> 16)) { std::puts("fail: random"); bad = 1; }
            if (object[0] != static_cast<unsigned char>(kind) || w8 != low(skill) || w12 != low(creature)) { std::puts("fail: base"); bad = 1; }
            if (w20 != low(vt_SpellHolyShield) || w24 != 0u || w28 != 0u) { std::puts("fail: table or zero words"); bad = 1; }
            if (*reinterpret_cast<void**>(object + 36) != particleResult) { std::puts("fail: particle handle"); bad = 1; }
            float got; std::memcpy(&got, object + 32, 4);
            if (got != -9.0f) { std::puts("fail: float result"); bad = 1; }
            std::string want = "lump:fx.lmp:spel_he_hshield.anm;sound;alloc92;";
            if (blk != nullptr) want += "shaft;";
            want += "slot;particle;";
            if (calls != want) { std::printf("fail: calls '%s'\n", calls.c_str()); bad = 1; }
            if (soundHeader != lumpResult || soundObject != creature || soundVolume != 1.0f) { std::puts("fail: sound"); bad = 1; }
            if (slotSeenThis != skill + 40 || slotSeenA != 1 || slotSeenB != static_cast<signed char>(object[0])) { std::puts("fail: slot args"); bad = 1; }
            if (blk != nullptr && (shaftArgs[0] != 90 || shaftArgs[1] != 86 || shaftArgs[2] != 126 || shaftArgs[3] != 250)) { std::puts("fail: shaft args"); bad = 1; }
            if (blk != nullptr && (shaftPoint.x != at.x || shaftPoint.y != at.y || shaftPoint.z != at.z)) { std::puts("fail: shaft point"); bad = 1; }
        }
    }
    if (bad == 0) std::puts("ok");
    return bad;
}
