// Observed __22SpellRangerCriticalHitiP8CreatureP5Skill at 0x002c7498, 308 bytes in .text. Arguments: the object, the rank,
// the creature and the skill. Returns nothing. The sync random state is advanced as state = state * 0x19660d + 0x3c6ef35f
// (32-bit), and its new value's upper 16 bits are kept.
// Steps, in order:
//  1. Byte +0 = the low byte of the rank. Word +20 = _vt$5Spell. Word +8 = skill, word +12 = creature, word +4 = 0.
//  2. The sync state becomes the new value. Word +16 = the new value shifted right by 16. Word +20 = _vt$22SpellRangerCriticalHit.
//  3. The 12 bytes at the creature's +16 are copied to the object's +76 (through a temporary).
//  4. A 40-byte block at +28 is initialised: byte +9 and byte +8 = 0; halfwords +10 and +18 = 69, +14 and +22 = 32, the rest
//     of +10..+24 = 0; word +44 = 0; float +28 and +36 = 70.0; floats +32 and +40 = a constant read from the image.
//     Then lightAdd is called with that block.
//  5. The skill table (the word at skill+96) entry at byte 88 is called as (skill + adjustment, 2, the signed byte at +0).
//     Its integer result is converted to float and stored at +24.
//  6. P_CreateParticleSystem() is called and its result stored at +88.
// Inferred: a ranger's critical-hit spell that adds a light at its own position and records a particle system.
// Unknown: the meanings of the fields, the constant at image address 0x48310c, and the lightAdd layout. Nothing here
// ran on the EE; the private paired run is described in the notes.
#if defined(NORRATH_HOST_ARENA)
// Host checks only: stored 32-bit addresses are arena offsets; the check supplies both conversions.
extern "C" unsigned char* norrath_host_pointer(unsigned address);
extern "C" unsigned norrath_host_address(const void* pointer);
#endif
extern "C" {
extern unsigned char vt_Spell[] asm("_vt$5Spell");
extern unsigned char vt_RangerCriticalHit[] asm("_vt$22SpellRangerCriticalHit");
extern unsigned syncRandState;
void* P_CreateParticleSystem__Fv();
void lightAdd__FP5Light(void* light);
}

namespace {
struct WordTriple { unsigned words[3]; };

#if defined(NORRATH_HOST_ARENA)
inline unsigned address_word(const void* p) { return norrath_host_address(p); }
inline unsigned char* word_pointer(unsigned a) { return a == 0 ? nullptr : norrath_host_pointer(a); }
#else
inline unsigned address_word(const void* p) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
inline unsigned char* word_pointer(unsigned a) { return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(a)); }
#endif
inline unsigned char* pointer_word(const unsigned char* base, unsigned offset) {
    return word_pointer(*reinterpret_cast<const unsigned*>(base + offset));
}
inline void store_word(unsigned char* base, unsigned offset, unsigned value) {
    *reinterpret_cast<unsigned*>(base + offset) = value;
}
inline void store_half(unsigned char* base, unsigned offset, unsigned short value) {
    *reinterpret_cast<unsigned short*>(base + offset) = value;
}
inline void store_float(unsigned char* base, unsigned offset, float value) {
    *reinterpret_cast<float*>(base + offset) = value;
}
inline float float_bits(unsigned bits) {
    union { unsigned u; float f; } value;
    value.u = bits;
    return value.f;
}
}

extern "C" void __22SpellRangerCriticalHitiP8CreatureP5Skill(void* self, int rank, void* creature, void* skill) {
    unsigned char* o = static_cast<unsigned char*>(self);
    o[0] = static_cast<unsigned char>(rank);
    store_word(o, 20, address_word(vt_Spell));
    __asm__ volatile("" : : "r"(o) : "memory");
    store_word(o, 8, address_word(skill));
    store_word(o, 12, address_word(creature));
    store_word(o, 4, 0);
    const unsigned state = syncRandState * 0x19660du + 0x3c6ef35fu;
    syncRandState = state;
    store_word(o, 16, state >> 16);
    store_word(o, 20, address_word(vt_RangerCriticalHit));
    const WordTriple position = *reinterpret_cast<const WordTriple*>(static_cast<const unsigned char*>(creature) + 16);
    o[37] = 0;
    *reinterpret_cast<WordTriple*>(o + 76) = position;

    unsigned char* light = o + 28;
    store_float(light, 36, float_bits(0x428c0000u));
    store_half(light, 22, 32);
    store_float(light, 28, float_bits(0x428c0000u));
    store_half(light, 14, 32);
    store_float(light, 40, float_bits(0x3955febfu));
    store_float(light, 32, float_bits(0x3955febfu));
    store_half(light, 18, 69);
    store_half(light, 10, 69);
    store_word(light, 44, 0);
    store_half(light, 12, 0);
    store_half(light, 16, 0);
    store_half(light, 20, 0);
    store_half(light, 24, 0);
    light[8] = 0;
    lightAdd__FP5Light(light);

    unsigned char* s = pointer_word(o, 8);
    const unsigned char* entry = pointer_word(s, 96) + 88;
    const short adjustment = *reinterpret_cast<const short*>(entry);
    const unsigned function = *reinterpret_cast<const unsigned*>(entry + 4);
    const int result = reinterpret_cast<int (*)(void*, int, int)>(word_pointer(function))(
        s + adjustment, 2, static_cast<signed char>(o[0]));
    store_float(o, 24, static_cast<float>(result));
    store_word(o, 88, address_word(P_CreateParticleSystem__Fv()));
}
