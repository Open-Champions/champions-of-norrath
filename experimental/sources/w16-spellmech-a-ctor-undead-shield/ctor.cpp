// Observed __17SpellUndeadShieldiP8CreatureP5Skill at 0x002affa8, 440 bytes in .text. Arguments: the object, the rank, the
// creature and the skill. Returns nothing. The sync random state is advanced as state = state * 0x19660d + 0x3c6ef35f
// (32-bit), and its new value's upper 16 bits are kept.
// Steps, in order:
//  1. Byte +0 = the low byte of the rank. Word +20 = _vt$5Spell. Word +8 = skill, word +12 = creature, word +4 = 0.
//  2. The sync state becomes the new value. Word +16 = the new value shifted right by 16. Word +20 = _vt$17SpellUndeadShield.
//  3. The 12 bytes at the creature's +16 are copied to the object's +80.
//  4. A 48-byte block at +32 is initialised: bytes +8 and +9 of the block = 0; halfwords +10 and +18 = 250, +12 and +20 = 200,
//     +14 and +22 = 100, +16 and +24 = 150; floats +28 and +36 = 0.0; floats +32 and +40 = 1.0f / 0.0f computed at run time
//     (on this FPU that gives the largest float, 0x7fffffff); float +44 = the image float 0x3eccccee (about 0.4).
//     Then lightAdd is called with that block.
//  5. The skill table (the word at skill+96) entry at byte 88 is called as (skill + adjustment, 1, the signed byte at +0).
//     Word +28 = 85 and word +24 = frameNm + 60 * result.
//  6. P_CreateParticleSystem() is called and its result stored at +92.
//  7. lump = lumpFindResource("fx.lmp", "spel_he_unshield.anm"). Word +96 = soundPlaySequence(lump, a copy of the creature's
//     12 bytes at +16, 1.0f). soundMakePersistant(that result).
// Inferred: a spell that adds a light at the creature's position and plays an "unshield" sequence. Unknown: the meanings of
// the fields, the roles of the block's values, and why the two floats are the result of a division by zero. Nothing here
// ran on the EE; checks are the host contract in this directory and a private paired run.
#if defined(NORRATH_HOST_ARENA)
// Host checks only: stored 32-bit addresses are arena offsets; the check supplies both conversions.
extern "C" unsigned char* norrath_host_pointer(unsigned address);
extern "C" unsigned norrath_host_address(const void* pointer);
#endif
struct Point3 { float x, y, z; };

extern "C" {
extern unsigned char vt_Spell[] asm("_vt$5Spell");
extern unsigned char vt_UndeadShield[] asm("_vt$17SpellUndeadShield");
extern unsigned syncRandState;
extern unsigned frameNm;
void* P_CreateParticleSystem__Fv();
void lightAdd__FP5Light(void* light);
void* lumpFindResource__FPcT0(char* lump, char* name);
int soundPlaySequence__FP15AnimationHeaderG6Point3f(void* header, Point3 point, float volume);
void soundMakePersistant__Fi(int sound);
}

namespace {
struct WordTriple { unsigned words[3]; };
static char fx_lump_name[] = "fx.lmp";
static char unshield_name[] = "spel_he_unshield.anm";

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

extern "C" void __17SpellUndeadShieldiP8CreatureP5Skill(void* self, int rank, void* creature, void* skill) {
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
    store_word(o, 20, address_word(vt_UndeadShield));
    const WordTriple position = *reinterpret_cast<const WordTriple*>(static_cast<const unsigned char*>(creature) + 16);
    o[41] = 0;
    *reinterpret_cast<WordTriple*>(o + 80) = position;

    volatile float zero = 0.0f;
    const float largest = 1.0f / zero;
    unsigned char* light = o + 32;
    store_half(light, 20, 200);
    store_half(light, 12, 200);
    store_float(light, 40, largest);
    store_float(light, 44, float_bits(0x3ecccceeu));
    store_half(light, 18, 250);
    store_float(light, 28, 0.0f);
    store_float(light, 36, 0.0f);
    store_float(light, 32, largest);
    store_half(light, 10, 250);
    store_half(light, 22, 100);
    store_half(light, 24, 150);
    store_half(light, 14, 100);
    store_half(light, 16, 150);
    light[8] = 0;
    lightAdd__FP5Light(light);

    unsigned char* s = pointer_word(o, 8);
    const unsigned char* entry = pointer_word(s, 96) + 88;
    const short adjustment = *reinterpret_cast<const short*>(entry);
    const unsigned function = *reinterpret_cast<const unsigned*>(entry + 4);
    const int result = reinterpret_cast<int (*)(void*, int, int)>(word_pointer(function))(
        s + adjustment, 1, static_cast<signed char>(o[0]));
    store_word(o, 28, 85);
    store_word(o, 24, frameNm + 60u * static_cast<unsigned>(result));
    store_word(o, 92, address_word(P_CreateParticleSystem__Fv()));
    void* lump = lumpFindResource__FPcT0(fx_lump_name, unshield_name);
    const Point3 at = *reinterpret_cast<const Point3*>(pointer_word(o, 12) + 16);
    const int sound = soundPlaySequence__FP15AnimationHeaderG6Point3f(lump, at, 1.0f);
    store_word(o, 96, static_cast<unsigned>(sound));
    soundMakePersistant__Fi(sound);
}
