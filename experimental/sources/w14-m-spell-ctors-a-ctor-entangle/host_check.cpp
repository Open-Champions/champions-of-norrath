// Host contract for __13SpellEntangleiP8CreatureP5Skill; not an EE input.
// The EE image stores 32-bit addresses; this host build uses 64-bit pointers, so the skill's secondary
// table pointer and the call target are read at pointer width here. The checks cover the stored fields,
// the random state step, the order of the particle call, the adjusted receiver, the call arguments and
// the stored result. The paired R5900 comparison is the stronger check and is described in the notes.
#include "spell_entangle_ctor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char vt_SpellEntangle[8];
unsigned syncRandState;
static std::string calls;
static void* particleResult = reinterpret_cast<void*>(0x0badf00d);
static int slotResult = 0x5a5a1234;
static void* slotSeenThis;
static int slotSeenA, slotSeenB;
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
    std::memset(object, 0xee, sizeof object);
    std::memset(skillBuffer, 0, sizeof skillBuffer);
    std::memset(table, 0, sizeof table);
    const unsigned seed = 0x12345678u;
    const unsigned next = seed * 0x19660du + 0x3c6ef35fu;

    const SlotFn fn = slotTarget;
    const void* tablePtr = table;
    std::memcpy(skill + 96, &tablePtr, sizeof tablePtr);
    const short adjust = -24;
    std::memcpy(table + 88, &adjust, 2);
    std::memcpy(table + 92, &fn, sizeof fn);

    for (int kind : {0x1a7, 0x80, -1}) {
        calls.clear();
        std::memset(object, 0xee, sizeof object);
        syncRandState = seed;
        __13SpellEntangleiP8CreatureP5Skill(object, kind, creature, skill);

        unsigned w4, w8, w12, w16, w20, w28, w32;
        std::memcpy(&w4, object + 4, 4); std::memcpy(&w8, object + 8, 4); std::memcpy(&w12, object + 12, 4);
        std::memcpy(&w16, object + 16, 4); std::memcpy(&w20, object + 20, 4); std::memcpy(&w28, object + 28, 4);
        std::memcpy(&w32, object + 32, 4);
        if (syncRandState != next || w16 != (next >> 16)) { std::puts("fail: random"); bad = 1; }
        if (object[0] != static_cast<unsigned char>(kind) || w4 != 0u) { std::puts("fail: base fields"); bad = 1; }
        if (w8 != low(skill) || w12 != low(creature)) { std::puts("fail: skill or creature"); bad = 1; }
        if (w20 != low(vt_SpellEntangle) || w28 != 0u) { std::puts("fail: table or zero word"); bad = 1; }
        if (*reinterpret_cast<void**>(object + 36) != particleResult) { std::puts("fail: particle handle"); bad = 1; }
        if (calls != "particle;slot;") { std::printf("fail: calls %s\n", calls.c_str()); bad = 1; }
        if (slotSeenThis != skill - 24 || slotSeenA != 1 || slotSeenB != static_cast<signed char>(object[0])) {
            std::printf("fail: slot arguments for kind %d\n", kind);
            bad = 1;
        }
        if (w32 != static_cast<unsigned>(slotResult)) { std::puts("fail: slot result"); bad = 1; }
    }
    if (bad == 0) std::puts("ok");
    return bad;
}
