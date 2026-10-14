// Observed __18SpellBlindingLightiP8CreatureT2P5Skill at 0x002ba028, 184 bytes (private executable symbol values).
// Constructor of a spell object: arguments are the object, an int K, two Creature pointers (the first is C1, the
// second C2) and a Skill pointer S. Nothing is returned.
//
// Observed layout, the same Spell base as the other constructors of this family:
//   +0 byte K, +4 word 0, +8 word S, +12 word C2,
//   +16 word the high half of the sync random state after one LCG step (state * 0x19660d + 0x3c6ef35f),
//   +20 word the _vt$18SpellBlindingLight table address (the base table is stored first and then replaced),
//   +24 word frameNm + 60 * (result of the call below), +28 word C1.
// The skill's secondary table is read: X = *(S + 96), with a halfword adjustment at X + 88 and a function address
// at X + 92. The call receives S + adjustment, 2, and the full int K (not truncated to a byte).
// Inferred: a spell of the same family that stores a frame-based value derived from the call's int result, as
// the hate-effect constructor does. Unknown: the names of the skill's secondary table and of the call target,
// and what the frame value at +24 is used for. Nothing here ran on the EE; the paired-run comparison is in
// docs/agents/w15-m-spell-ctors-a.md.

extern "C" {
extern unsigned char vt_BlindingLight[] __asm__("_vt$18SpellBlindingLight");
extern unsigned char vt_Spell[] __asm__("_vt$5Spell");
extern unsigned syncRandState;
extern int frameNm;
}

namespace {
inline unsigned low(const void* p) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
}

extern "C" void __18SpellBlindingLightiP8CreatureT2P5Skill(void* self, int kind, void* creature, void* second, void* skill) {
    unsigned char* o = static_cast<unsigned char*>(self);
    o[0] = static_cast<unsigned char>(kind);
    *reinterpret_cast<unsigned*>(o + 4) = 0u;
    *reinterpret_cast<unsigned*>(o + 8) = low(skill);
    *reinterpret_cast<unsigned*>(o + 12) = low(second);
    *reinterpret_cast<unsigned*>(o + 20) = low(vt_Spell);
    syncRandState = syncRandState * 0x19660du + 0x3c6ef35fu;
    *reinterpret_cast<unsigned*>(o + 16) = syncRandState >> 16;
    *reinterpret_cast<unsigned*>(o + 20) = low(vt_BlindingLight);

    unsigned char* s = static_cast<unsigned char*>(skill);
    unsigned char* table = *reinterpret_cast<unsigned char**>(s + 96);
    const short adjustment = *reinterpret_cast<short*>(table + 88);
    typedef int (*Slot)(void* self, int a, int b);
    const Slot slot = *reinterpret_cast<Slot*>(table + 92);
    const int result = slot(s + adjustment, 2, kind);

    *reinterpret_cast<unsigned*>(o + 24) = static_cast<unsigned>(frameNm) + static_cast<unsigned>(result) * 60u;
    *reinterpret_cast<unsigned*>(o + 28) = low(creature);
}
