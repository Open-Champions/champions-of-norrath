// Observed __11SpellChargeiP8CreatureP5Skill at 0x00290e78, 632 bytes in .text. Arguments: the object, the rank, the
// creature and the skill. Returns nothing. The sync random state is advanced as state = state * 0x19660d + 0x3c6ef35f
// (32-bit), and its new value's upper 16 bits are kept.
// Steps, in order:
//  1. Byte +0 = the low byte of the rank. Word +20 = _vt$5Spell. Word +12 = creature, word +8 = skill, word +4 = 0.
//  2. The sync state becomes the new value. Word +16 = the new value shifted right by 16. Word +20 = _vt$11SpellCharge.
//  3. The 12 bytes at the creature's +16 are copied to the object's +124. A 48-byte block at +76 is initialised: bytes +8 and
//     +9 = 0; halfwords +10 and +18 = 108, +12 and +20 = 102, +14 and +22 = 208, +16 and +24 = 0; floats +28 and +36 =
//     103.0; floats +32 and +40 = the image float 0x38c5ad3b; word +44 = 0. lightAdd is called with that block.
//  4. lumpFindResource("fx.lmp", "spel_bw_bull.anm") is passed to soundPlaySequence(lump, creature, 1.0f).
//  5. Word +68 = frameNm, word +72 = 0. X = icos(halfword at creature+56) * 10.0f, Y = isin(same) * 10.0f.
//  6. For i = 0 to 9: h = P_StartTrail(1, 800, f, 4, 64, 64, 128, 32) with f the image float 0x3f4ccccc; the word at
//     object+28+4*i = h; Z = float(i) * 6.0f (image float 0x40bfffff) + 12.0f; then
//     P_ExtendTrail(h, the creature's position at +16 plus (X, Y, Z)), each component added in float.
// Inferred: a charge spell that lays ten trails ahead of the caster along its facing. Unknown: the meanings of the fields,
// the trail parameters and the image float 0x3f4ccccc's role. Nothing here ran on the EE; checks are the host contract in
// this directory and a private paired run.
#if defined(NORRATH_HOST_ARENA)
// Host checks only: stored 32-bit addresses are arena offsets; the check supplies both conversions.
extern "C" unsigned char* norrath_host_pointer(unsigned address);
extern "C" unsigned norrath_host_address(const void* pointer);
#endif
struct Point3 { float x, y, z; };
struct ListHead { void* first; unsigned unknown[3]; };

extern "C" {
extern unsigned char vt_Spell[] asm("_vt$5Spell");
extern unsigned char vt_Charge[] asm("_vt$11SpellCharge");
extern unsigned syncRandState;
extern unsigned frameNm;
void lightAdd__FP5Light(void* light);
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sound, void* object, float volume);
float icos__Fi(int angle);
float isin__Fi(int angle);
int P_StartTrail__Fiifiiiii(int a, int b, float c, int d, int e, int f, int g, int h);
void P_ExtendTrail__FiG6Point3(int handle, Point3 point);
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
inline unsigned load_word(const unsigned char* base, unsigned offset) {
    return *reinterpret_cast<const unsigned*>(base + offset);
}
inline unsigned char* pointer_word(const unsigned char* base, unsigned offset) {
    return word_pointer(load_word(base, offset));
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
inline float load_float(const unsigned char* base, unsigned offset) {
    return *reinterpret_cast<const float*>(base + offset);
}
inline float float_bits(unsigned bits) {
    union { unsigned u; float f; } value;
    value.u = bits;
    return value.f;
}
static char fx_lump_name[] = "fx.lmp";
static char charge_sound_name[] = "spel_bw_bull.anm";
}

extern "C" void __11SpellChargeiP8CreatureP5Skill(void* self, int rank, void* creature, void* skill) {
    unsigned char* o = static_cast<unsigned char*>(self);
    unsigned char* c = static_cast<unsigned char*>(creature);
    o[0] = static_cast<unsigned char>(rank);
    store_word(o, 20, address_word(vt_Spell));
    __asm__ volatile("" : : "r"(o) : "memory");
    store_word(o, 12, address_word(creature));
    store_word(o, 8, address_word(skill));
    store_word(o, 4, 0);
    const unsigned state = syncRandState * 0x19660du + 0x3c6ef35fu;
    syncRandState = state;
    store_word(o, 16, state >> 16);
    store_word(o, 20, address_word(vt_Charge));
    const WordTriple position = *reinterpret_cast<const WordTriple*>(c + 16);
    unsigned char* light = o + 76;
    light[9] = 0;
    *reinterpret_cast<WordTriple*>(o + 124) = position;
    store_float(light, 36, 103.0f);
    store_half(light, 18, 108);
    store_half(light, 20, 102);
    store_half(light, 22, 208);
    store_float(light, 28, 103.0f);
    store_half(light, 10, 108);
    store_float(light, 40, float_bits(0x38c5ad3bu));
    store_float(light, 32, float_bits(0x38c5ad3bu));
    store_word(light, 44, 0);
    store_half(light, 12, 102);
    store_half(light, 14, 208);
    store_half(light, 16, 0);
    store_half(light, 24, 0);
    light[8] = 0;
    lightAdd__FP5Light(light);

    void* sound = lumpFindResource__FPcT0(fx_lump_name, charge_sound_name);
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(sound, pointer_word(o, 12), 1.0f);
    store_word(o, 68, frameNm);
    store_word(o, 72, 0);
    const float x = icos__Fi(*reinterpret_cast<const short*>(pointer_word(o, 12) + 56)) * 10.0f;
    const float y = isin__Fi(*reinterpret_cast<const short*>(pointer_word(o, 12) + 56)) * 10.0f;
    for (int i = 0; i < 10; ++i) {
        const int handle = P_StartTrail__Fiifiiiii(1, 800, float_bits(0x3f4cccccu), 4, 64, 64, 128, 32);
        store_word(o, 28 + 4 * static_cast<unsigned>(i), static_cast<unsigned>(handle));
        const float z = static_cast<float>(i) * float_bits(0x40bfffffu) + 12.0f;
        const unsigned char* at = pointer_word(o, 12) + 16;
        Point3 next;
        next.x = load_float(at, 0) + x;
        next.y = load_float(at, 4) + y;
        next.z = load_float(at, 8) + z;
        P_ExtendTrail__FiG6Point3(handle, next);
    }
}
