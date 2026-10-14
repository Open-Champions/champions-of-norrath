// Observed __12SpellHealingiP8CreatureP5Skill at 0x002ae9d8, 352 bytes (private executable symbol values).
// Constructor of the spell object: arguments are the object, an int whose low byte is stored (K), a Creature
// pointer C and a Skill pointer S. Nothing is returned.
//
// Observed layout, the same Spell base as the other constructors of this family:
//   +0 byte K, +4 word 0, +8 word S, +12 word C,
//   +16 word  the high half of the sync random state after one LCG step (state * 0x19660d + 0x3c6ef35f),
//   +20 word  the _vt$12SpellHealing table (the base table is stored first and then replaced),
//   +24 word C, +28 word 0, +32 float (see below), +36 word the result of P_CreateParticleSystem.
//   The random word at +16 is stored in the delay slot of the lump call, so the lump result itself is not kept.
// Then: lumpFindResource__FPcT0("fx.lmp", "spel_he_heal.anm") gives a header, and
// soundPlaySequence__FP15AnimationHeaderP10GameObjectf is called with the header, C and volume 1.0f. Its result
// is not stored.
// If blockAlloc__Fi(92) returns a block, a copy of the point at C + 16 is made, and the block is passed to
// __12GlowingShaftG6Point3iiii with the point, 90, 250, 200 and 100. The block is not stored. Then the particle
// system is created, unconditionally.
// Then the skill's secondary table is read: X = *(S + 96), a halfword adjustment at X + 88 and a function address
// at X + 92. The call receives S + adjustment, 0, and K read back from +0 as a sign-extended byte. Its int result
// is converted to float (f2).
// The index is the word at C + 960 times 23984 (a three-operand R5900 mult, whose low word is also kept in its
// destination register). The float at +32 is f0 * f2, where f0 is the float at 0x004086b0 + index times K_a, and
// f2 is the converted call result times K_b. K_a is the float constant at 0x00482c60 (bits 0x3c886600) and K_b the
// float constant at 0x00482af0 (bits 0x3c23d70a).
// Inferred: a healing effect whose strength is scaled by a per-creature table entry, with a particle system.
// Unknown: the names of the
// table, the creature field at +960, the skill table and the shaft's meaning. Nothing here ran on the EE.
struct Point3 { float x, y, z; };

extern "C" {
extern unsigned char vt_SpellHealing[] __asm__("_vt$12SpellHealing");
extern unsigned syncRandState;
extern unsigned char player[] __asm__("player");
void* lumpFindResource__FPcT0(char* lump, char* resource);
int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
void* blockAlloc__Fi(int size);
void* P_CreateParticleSystem__Fv(void);
void glowing_shaft_ctor(void* self, Point3 point, int a, int b, int c, int d) __asm__("__12GlowingShaftG6Point3iiii");
}

namespace {
inline unsigned low(const void* p) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
inline float floatFromBits(unsigned bits) {
    union { unsigned u; float f; } v;
    v.u = bits;
    return v.f;
}
}

extern "C" void __12SpellHealingiP8CreatureP5Skill(void* self, int kind, void* creature, void* skill) {
    unsigned char* o = static_cast<unsigned char*>(self);
    unsigned char* c = static_cast<unsigned char*>(creature);
    unsigned char* s = static_cast<unsigned char*>(skill);
    o[0] = static_cast<unsigned char>(kind);
    *reinterpret_cast<unsigned*>(o + 4) = 0u;
    *reinterpret_cast<unsigned*>(o + 8) = low(skill);
    *reinterpret_cast<unsigned*>(o + 12) = low(creature);
    syncRandState = syncRandState * 0x19660du + 0x3c6ef35fu;
    *reinterpret_cast<unsigned*>(o + 16) = syncRandState >> 16;
    *reinterpret_cast<unsigned*>(o + 20) = low(vt_SpellHealing);
    *reinterpret_cast<unsigned*>(o + 24) = low(creature);

    void* header = lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("spel_he_heal.anm"));
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(header, creature, 1.0f);

    if (void* block = blockAlloc__Fi(92)) {
        Point3 point;
        const unsigned* from = reinterpret_cast<const unsigned*>(c + 16);
        unsigned* to = reinterpret_cast<unsigned*>(&point);
        to[0] = from[0];
        to[1] = from[1];
        to[2] = from[2];
        glowing_shaft_ctor(block, point, 90, 250, 200, 100);
    }

    *reinterpret_cast<void**>(o + 36) = P_CreateParticleSystem__Fv();
    *reinterpret_cast<unsigned*>(o + 28) = 0u;
    typedef int (*Slot)(void* self, int a, int b);
    unsigned char* table = *reinterpret_cast<unsigned char**>(s + 96);
    const short adjustment = *reinterpret_cast<short*>(table + 88);
    const Slot slot = *reinterpret_cast<Slot*>(table + 92);
    const int signed_kind = static_cast<signed char>(o[0]);
    const float fromSkill = static_cast<float>(slot(s + adjustment, 0, signed_kind));

    const int index = static_cast<int>(*reinterpret_cast<unsigned*>(c + 960) * 23984u);
    const float ka = floatFromBits(0x3c886600u);
    const float kb = floatFromBits(0x3c23d70au);
    const float tableValue = *reinterpret_cast<float*>(player + 0xb0 + index);
    *reinterpret_cast<float*>(o + 32) = (tableValue * ka) * (fromSkill * kb);
}
