// Host contract for __18SpellBlindingLightiP8CreatureT2P5Skill; not an EE input. Includes the source.
// The EE image stores 32-bit addresses; this host build uses 64-bit pointers, so the skill's secondary table
// pointer and the call target are read at pointer width here. The checks cover the stored fields, the random
// state step, the adjusted receiver, the call arguments (including the full int kind) and the frame value.
// The paired R5900 comparison is the stronger check and is described in the notes.
#include "spell_blinding_light_ctor.cpp"
#include <cstdio>
#include <cstring>

unsigned char vt_BlindingLight[8];
unsigned char vt_Spell[8];
unsigned syncRandState;
int frameNm;
static int slotResult;
static void* slotSeenThis;
static int slotSeenA, slotSeenB;
static int slotCalls;

typedef int (*SlotFn)(void*, int, int);
static int slotTarget(void* self, int a, int b) {
    ++slotCalls; slotSeenThis = self; slotSeenA = a; slotSeenB = b; return slotResult;
}

int main() {
    int bad = 0;
    alignas(16) unsigned char object[128];
    alignas(16) unsigned char skillBuffer[256];
    unsigned char* const skill = skillBuffer + 32;
    alignas(16) unsigned char table[128];
    alignas(16) unsigned char creature[32];
    alignas(16) unsigned char creature2[32];
    std::memset(skillBuffer, 0, sizeof skillBuffer);
    std::memset(table, 0, sizeof table);

    const SlotFn fn = slotTarget;
    const void* tablePtr = table;
    std::memcpy(skill + 96, &tablePtr, sizeof tablePtr);
    const short adjust = 40;
    std::memcpy(table + 88, &adjust, 2);
    std::memcpy(table + 92, &fn, sizeof fn);

    const int kinds[] = {0x1a7, 0x80, -1, 0x12345};
    const unsigned seeds[] = {0x12345678u, 0u, 0xffffffffu};
    const int frames[] = {0, 7, -3};
    for (unsigned seed : seeds) for (int kind : kinds) for (int frame : frames) {
        std::memset(object, 0xee, sizeof object);
        syncRandState = seed;
        frameNm = frame;
        slotResult = 1000 + frame;
        slotCalls = 0; slotSeenThis = nullptr; slotSeenA = slotSeenB = 0;
        const unsigned next = seed * 0x19660du + 0x3c6ef35fu;
        __18SpellBlindingLightiP8CreatureT2P5Skill(object, kind, creature, creature2, skill);

        unsigned w4, w8, w12, w16, w20, w24, w28;
        std::memcpy(&w4, object + 4, 4); std::memcpy(&w8, object + 8, 4); std::memcpy(&w12, object + 12, 4);
        std::memcpy(&w16, object + 16, 4); std::memcpy(&w20, object + 20, 4); std::memcpy(&w24, object + 24, 4);
        std::memcpy(&w28, object + 28, 4);
        if (syncRandState != next || w16 != (next >> 16)) { std::puts("fail: random"); bad = 1; }
        if (object[0] != static_cast<unsigned char>(kind) || w4 != 0u) { std::puts("fail: base fields"); bad = 1; }
        if (w8 != low(skill) || w12 != low(creature2) || w28 != low(creature)) { std::puts("fail: skill or creatures"); bad = 1; }
        if (w20 != low(vt_BlindingLight)) { std::puts("fail: table"); bad = 1; }
        if (slotCalls != 1 || slotSeenThis != static_cast<void*>(skill + 40) || slotSeenA != 2
            || slotSeenB != kind) { std::printf("fail: call args kind %d frame %d\n", kind, frame); bad = 1; }
        if (w24 != static_cast<unsigned>(frameNm) + static_cast<unsigned>(slotResult) * 60u) { std::puts("fail: frame value"); bad = 1; }
    }
    if (bad == 0) std::puts("ok");
    return bad;
}
