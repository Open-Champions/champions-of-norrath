// Observed __9SpellRootiP8CreatureP5Skill at 0x002ad058, 164 bytes in .text. Arguments: the object, the rank, the creature
// and the skill. Returns nothing. The sync random state is advanced as state = state * 0x19660d + 0x3c6ef35f (32-bit).
// Steps, in order:
//  1. Byte +0 = the low byte of the rank. Word +20 = _vt$5Spell. Word +12 = creature, word +8 = skill, word +4 = 0.
//  2. The sync state becomes the new value. Word +20 = _vt$9SpellRoot and word +16 = the new state shifted right by 16.
//  3. Word +28 = 0, then word +36 = P_CreateParticleSystem().
//  4. The skill table (the word at skill+96) entry at byte 88 is called as (skill + adjustment, 1, the signed byte at +0).
//     Its integer result is stored at word +32.
// Inferred: a root spell object that records its caster and skill, and takes one particle system and a result from the
// skill. Unknown: the meanings of the fields, the particle system's role, and what the skill entry at byte 88 computes.
// Nothing here ran on the EE; the private paired run is described in the notes.
extern "C" {
extern unsigned char vt_Spell[] asm("_vt$5Spell");
extern unsigned char vt_SpellRoot[] asm("_vt$9SpellRoot");
extern unsigned syncRandState;
void* P_CreateParticleSystem__Fv();
}

namespace {
inline unsigned address_word(const void* p) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
inline unsigned char* pointer_word(const unsigned char* base, unsigned offset) {
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<const unsigned*>(base + offset)));
}
inline void store_word(unsigned char* base, unsigned offset, unsigned value) {
    *reinterpret_cast<unsigned*>(base + offset) = value;
}
}

extern "C" void __9SpellRootiP8CreatureP5Skill(void* self, int rank, void* creature, void* skill) {
    unsigned char* o = static_cast<unsigned char*>(self);
    o[0] = static_cast<unsigned char>(rank);
    store_word(o, 20, address_word(vt_Spell));
    __asm__ volatile("" : : "r"(o) : "memory");
    store_word(o, 12, address_word(creature));
    store_word(o, 8, address_word(skill));
    store_word(o, 4, 0);
    const unsigned state = syncRandState * 0x19660du + 0x3c6ef35fu;
    syncRandState = state;
    store_word(o, 20, address_word(vt_SpellRoot));
    store_word(o, 16, state >> 16);
    store_word(o, 28, 0);
    store_word(o, 36, address_word(P_CreateParticleSystem__Fv()));
    unsigned char* s = pointer_word(o, 8);
    const unsigned char* entry = pointer_word(s, 96) + 88;
    const short adjustment = *reinterpret_cast<const short*>(entry);
    const unsigned function = *reinterpret_cast<const unsigned*>(entry + 4);
    unsigned char* adjusted = s + adjustment;
    const int result = reinterpret_cast<int (*)(void*, int, int)>(static_cast<__UINTPTR_TYPE__>(function))(
        adjusted, 1, static_cast<signed char>(o[0]));
    store_word(o, 32, static_cast<unsigned>(result));
}
