// Host contract for __16SpellCriticalHitiP8CreatureP5Skill; not an EE input. The EE image stores 32-bit
// addresses; this host build uses 64-bit pointers, so the skill's table and the call target are read at pointer
// width, and the object fields are compared with low().
#include "spell_critical_hit_ctor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char vt_SpellCriticalHit[8];
unsigned syncRandState;
static std::string calls;
static unsigned char* objectBase;
static unsigned char lightSeen[48];
static void* particleResult = reinterpret_cast<void*>(0x0badf00d);
static int slotResult = 0x00000005;
static void* slotSeenThis;
static int slotSeenA, slotSeenB;
void lightAdd__FP5Light(void* light) {
    calls += "light;";
    if (static_cast<unsigned char*>(light) != objectBase + 28) calls += "light-address-wrong;";
    std::memcpy(lightSeen, light, 48);
}
void* P_CreateParticleSystem__Fv(void) { calls += "particle;"; return particleResult; }
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
    alignas(16) unsigned char creature[32];
    std::memset(skillBuffer, 0, sizeof skillBuffer);
    std::memset(table, 0, sizeof table);
    const unsigned seed = 0x2468ace0u;
    const unsigned next = seed * 0x19660du + 0x3c6ef35fu;
    const SlotFn fn = slotTarget;
    const void* tablePtr = table;
    std::memcpy(skill + 96, &tablePtr, sizeof tablePtr);
    const short adjust = -24;
    std::memcpy(table + 88, &adjust, 2);
    std::memcpy(table + 92, &fn, sizeof fn);
    for (int i = 0; i < 12; ++i) creature[16 + i] = static_cast<unsigned char>(0x40 + i);
    objectBase = object;
    for (int kind : {0x1a7, 0x80, -1}) {
        calls.clear();
        std::memset(object, 0xee, sizeof object);
        syncRandState = seed;
        __16SpellCriticalHitiP8CreatureP5Skill(object, kind, creature, skill);
        unsigned w8, w12, w16, w20, w76, w80, w84, w88, w24;
        std::memcpy(&w8, object + 8, 4); std::memcpy(&w12, object + 12, 4); std::memcpy(&w16, object + 16, 4);
        std::memcpy(&w20, object + 20, 4); std::memcpy(&w76, object + 76, 4); std::memcpy(&w80, object + 80, 4);
        std::memcpy(&w84, object + 84, 4); std::memcpy(&w88, object + 88, 4); std::memcpy(&w24, object + 24, 4);
        if (syncRandState != next || w16 != (next >> 16)) { std::puts("fail: random"); bad = 1; }
        if (object[0] != static_cast<unsigned char>(kind) || w8 != low(skill) || w12 != low(creature)) { std::puts("fail: base"); bad = 1; }
        if (w20 != low(vt_SpellCriticalHit)) { std::puts("fail: table"); bad = 1; }
        unsigned c0, c1, c2; std::memcpy(&c0, creature + 16, 4); std::memcpy(&c1, creature + 20, 4); std::memcpy(&c2, creature + 24, 4);
        if (w76 != c0 || w80 != c1 || w84 != c2) { std::puts("fail: copied point"); bad = 1; }
        if (w88 != low(particleResult)) { std::puts("fail: particle handle"); bad = 1; }
        float f24; std::memcpy(&f24, object + 24, 4);
        if (f24 != 5.0f) { std::puts("fail: float result"); bad = 1; }
        if (lightSeen[8] != 0 || lightSeen[9] != 0 || lightSeen[10] != 255 || lightSeen[18] != 255) { std::puts("fail: light halves"); bad = 1; }
        float l28, l32, l36, l40; unsigned l44;
        std::memcpy(&l28, lightSeen + 28, 4); std::memcpy(&l32, lightSeen + 32, 4); std::memcpy(&l36, lightSeen + 36, 4);
        std::memcpy(&l40, lightSeen + 40, 4); std::memcpy(&l44, lightSeen + 44, 4);
        if (l28 != 120.0f || l36 != 120.0f || l44 != 0u || l32 != l40) { std::puts("fail: light floats"); bad = 1; }
        if (calls != "light;slot;particle;") { std::printf("fail: calls %s\n", calls.c_str()); bad = 1; }
        if (slotSeenThis != skill - 24 || slotSeenA != 2 || slotSeenB != static_cast<signed char>(object[0])) {
            std::printf("fail: slot arguments for kind %d\n", kind); bad = 1;
        }
    }
    if (bad == 0) std::puts("ok");
    return bad;
}
