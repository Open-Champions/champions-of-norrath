// Observed __13SpellEntangleiP8CreatureP5Skill at 0x002c6f98, 164 bytes (private executable symbol values).
// Constructor of a spell object: arguments are the object, an int whose low byte is stored (K), a Creature
// pointer C and a Skill pointer S. Nothing is returned.
//
// Observed layout, the same Spell base as the other constructors of this family:
//   +0 byte K, +4 word 0, +8 word S, +12 word C,
//   +16 word  the high half of the sync random state after one LCG step (state * 0x19660d + 0x3c6ef35f),
//   +20 word  the _vt$13SpellEntangle table address (the base table is stored first and then replaced),
//   +28 word 0, +32 word (result of the call below), +36 word (result of P_CreateParticleSystem).
// The order of the stores is kept: the random state is stepped before the particle system is created.
// Then the skill's secondary table is read: X = *(S + 96), with a halfword adjustment at X + 88 and a
// function address at X + 92. The call receives S + adjustment, 1, and K read back from +0 as a sign-extended
// byte. Its int result is stored at +32.
// Inferred: a spell that creates a particle system and asks the skill for one value. Unknown: the names of
// the skill's secondary table and of the call target, and the meaning of the value at +32.
// Nothing here ran on the EE; the paired-run comparison is described in docs/agents/w14-m-spell-ctors-a.md.

extern "C" {
extern unsigned char vt_SpellEntangle[] __asm__("_vt$13SpellEntangle");
extern unsigned syncRandState;
void* P_CreateParticleSystem__Fv(void);
}

namespace {
inline unsigned low(const void* p) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
}

extern "C" void __13SpellEntangleiP8CreatureP5Skill(void* self, int kind, void* creature, void* skill) {
    unsigned char* o = static_cast<unsigned char*>(self);
    o[0] = static_cast<unsigned char>(kind);
    *reinterpret_cast<unsigned*>(o + 4) = 0u;
    *reinterpret_cast<unsigned*>(o + 8) = low(skill);
    *reinterpret_cast<unsigned*>(o + 12) = low(creature);
    syncRandState = syncRandState * 0x19660du + 0x3c6ef35fu;
    *reinterpret_cast<unsigned*>(o + 16) = syncRandState >> 16;
    *reinterpret_cast<unsigned*>(o + 20) = low(vt_SpellEntangle);
    *reinterpret_cast<unsigned*>(o + 28) = 0u;
    *reinterpret_cast<void**>(o + 36) = P_CreateParticleSystem__Fv();

    unsigned char* s = static_cast<unsigned char*>(skill);
    unsigned char* table = *reinterpret_cast<unsigned char**>(s + 96);
    const short adjustment = *reinterpret_cast<short*>(table + 88);
    typedef int (*Slot)(void* self, int a, int b);
    const Slot slot = *reinterpret_cast<Slot*>(table + 92);
    const int signed_kind = static_cast<signed char>(o[0]);
    *reinterpret_cast<int*>(o + 32) = slot(s + adjustment, 1, signed_kind);
}
