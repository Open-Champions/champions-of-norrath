// Observed __16SpellCriticalHitiP8CreatureP5Skill at 0x002ad3f8, 304 bytes (private executable symbol values).
// Constructor of the spell object: arguments are the object, an int whose low byte is stored (K), a Creature
// pointer C and a Skill pointer S. Nothing is returned.
//
// Observed layout, the same Spell base as the other constructors of this family:
//   +0 byte K, +4 word 0, +8 word S, +12 word C,
//   +16 word  the high half of the sync random state after one LCG step (state * 0x19660d + 0x3c6ef35f),
//   +20 word  the _vt$16SpellCriticalHit table (the base table is stored first and then replaced),
//   +24 float the result of the call below, converted from int,
//   +76 Point3 copied from C + 16, +88 word the result of P_CreateParticleSystem.
// A light record is initialised in place at +28 and handed to lightAdd__FP5Light (it ends where the copied point begins):
//   +8 and +9 bytes 0; halfwords +10 = 255, +12 = 0, +14 = 0, +16 = 0, +18 = 255, +20 = 0, +22 = 0, +24 = 0;
//   floats +28 = 120.0, +32 = the float constant at 0x00482c20 (bits 0x3891a2b3), +36 = 120.0, +40 = the same
//   constant, word +44 = 0.
// The skill's secondary table is then read: X = *(S + 96), a halfword adjustment at X + 88 and a function address
// at X + 92. The call receives S + adjustment, 2, and K read back from +0 as a sign-extended byte.
// Inferred: a flash and a particle system for a critical hit. Unknown: the meaning of the light fields, of the
// skill's secondary table, and of the stored float. Nothing here ran on the EE.

extern "C" {
extern unsigned char vt_SpellCriticalHit[] __asm__("_vt$16SpellCriticalHit");
extern unsigned syncRandState;
void lightAdd__FP5Light(void* light);
void* P_CreateParticleSystem__Fv(void);
}

namespace {
inline unsigned low(const void* p) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
inline float floatFromBits(unsigned bits) {
    union { unsigned u; float f; } v;
    v.u = bits;
    return v.f;
}
}

extern "C" void __16SpellCriticalHitiP8CreatureP5Skill(void* self, int kind, void* creature, void* skill) {
    unsigned char* o = static_cast<unsigned char*>(self);
    o[0] = static_cast<unsigned char>(kind);
    *reinterpret_cast<unsigned*>(o + 4) = 0u;
    *reinterpret_cast<unsigned*>(o + 8) = low(skill);
    *reinterpret_cast<unsigned*>(o + 12) = low(creature);
    syncRandState = syncRandState * 0x19660du + 0x3c6ef35fu;
    *reinterpret_cast<unsigned*>(o + 16) = syncRandState >> 16;
    *reinterpret_cast<unsigned*>(o + 20) = low(vt_SpellCriticalHit);
    const unsigned* at = reinterpret_cast<const unsigned*>(static_cast<unsigned char*>(creature) + 16);
    unsigned* copy = reinterpret_cast<unsigned*>(o + 76);
    copy[0] = at[0];
    copy[1] = at[1];
    copy[2] = at[2];

    unsigned char* light = o + 28;
    light[8] = 0;
    light[9] = 0;
    *reinterpret_cast<short*>(light + 10) = 255;
    *reinterpret_cast<short*>(light + 12) = 0;
    *reinterpret_cast<short*>(light + 14) = 0;
    *reinterpret_cast<short*>(light + 16) = 0;
    *reinterpret_cast<short*>(light + 18) = 255;
    *reinterpret_cast<short*>(light + 20) = 0;
    *reinterpret_cast<short*>(light + 22) = 0;
    *reinterpret_cast<short*>(light + 24) = 0;
    const float constant = floatFromBits(0x3891a2b3u);
    *reinterpret_cast<float*>(light + 28) = 120.0f;
    *reinterpret_cast<float*>(light + 32) = constant;
    *reinterpret_cast<float*>(light + 36) = 120.0f;
    *reinterpret_cast<float*>(light + 40) = constant;
    *reinterpret_cast<unsigned*>(light + 44) = 0u;
    lightAdd__FP5Light(light);

    unsigned char* s = static_cast<unsigned char*>(skill);
    unsigned char* table = *reinterpret_cast<unsigned char**>(s + 96);
    const short adjustment = *reinterpret_cast<short*>(table + 88);
    typedef int (*Slot)(void* self, int a, int b);
    const Slot slot = *reinterpret_cast<Slot*>(table + 92);
    const int signed_kind = static_cast<signed char>(o[0]);
    *reinterpret_cast<float*>(o + 24) = static_cast<float>(slot(s + adjustment, 2, signed_kind));
    *reinterpret_cast<unsigned*>(o + 88) = low(P_CreateParticleSystem__Fv());
}
