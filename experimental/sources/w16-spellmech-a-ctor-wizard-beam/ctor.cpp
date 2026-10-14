// Observed __15SpellWizardBeamiP8CreatureP5Skill at 0x002b6ca0, 1076 bytes in .text. Arguments: the object, the rank, the
// creature and the skill. Returns nothing. The sync random state is advanced as state = state * 0x19660d + 0x3c6ef35f
// (32-bit). eRand(object) is the image's Spell random step. hsv(h, s, v) returns a packed colour whose low, middle and high
// bytes are used as three channel values below.
// Steps, in order:
//  1. Word +4 = 0. Byte +0 = the low byte of the rank. Word +12 = creature, word +20 = _vt$5Spell, word +8 = skill. The sync
//     state becomes the new value, word +16 = its upper 16 bits, word +20 = _vt$15SpellWizardBeam.
//  2. h = float(eRand(object)) * 2^-16 (image float 0x37800000). c1 = hsv(h, 0.8f, 1.0f) (image float 0x3f4ccccc). Word +28 =
//     P_StartTrail(1, 200, 2.0f, 5, c1 low, c1 mid, c1 high, 128).
//  3. h = h + 0.5f. c2 = hsv(fmodf(h, 1.0f), 0.9f, 1.0f) (image float 0x3f666666). c3 = hsv(fmodf(h, 1.0f) computed again,
//     0.6f, 1.0f) (image float 0x3f199999). Word +32 = P_StartTrail(1, 100, 2.0f, 5, the three bytes of c2, 128) and word +36 =
//     P_StartTrail(1, 100, 2.0f, 5, the three bytes of c3, 128). Bytes +25 and +26 = 0.
//  4. For i = 0 to 4: the word at object + 852 + 8*i = -100000 and the word at object + 848 + 8*i = P_StartTrail(6, 4000, 2.0f,
//     5, the three bytes of c1, 32).
//  5. The skill table (the word at skill+96) entry at byte 88 is called as (skill + adjustment, 1, the signed byte at +0): floats
//     +840 and +844 = float(2 * result). It is called with 3: float +836 = float(3 * result), copied into the six floats at
//     +812 .. +832. Word +888 = c1. Word +892 = P_StartTrail(1, 300, 2.0f, 5, the three bytes of c1, 128).
//  6. The table entry is called with 2: word +184 = result, limited to at most 6. For i below the stored word +184: word
//     object + 188 + 4*i = P_StartTrail(1, 100, 2.0f, 5, the three bytes of c1, 128) and word object + 212 + 4*i =
//     P_StartTrail(1, 100, 2.0f, 5, the three bytes of c2, 128).
//  7. lumpFindResource("fx.lmp", "spel_ew_wbeam.anm") is passed to soundPlaySequence(lump, a copy of the creature's 12 bytes at
//     +16, 1.0f).
// Inferred: a wizard-beam spell that starts coloured trails; three colours are derived from one random hue. Unknown: the meanings
// of the fields, the saturation constants' roles, and what word +888 and the -100000 words are for. Nothing here ran on the EE;
// checks are the host contract in this directory and a private paired run.
#if defined(NORRATH_HOST_ARENA)
// Host checks only: stored 32-bit addresses are arena offsets; the check supplies both conversions.
extern "C" unsigned char* norrath_host_pointer(unsigned address);
extern "C" unsigned norrath_host_address(const void* pointer);
#endif
struct Point3 { float x, y, z; };
struct ListHead { void* first; unsigned unknown[3]; };

extern "C" {
extern unsigned char vt_Spell[] asm("_vt$5Spell");
extern unsigned char vt_WizardBeam[] asm("_vt$15SpellWizardBeam");
extern unsigned syncRandState;
int eRand__5Spell(void* spell);
unsigned hsv__Ffff(float hue, float saturation, float value);
float fmodf(float x, float y);
int P_StartTrail__Fiifiiiii(int a, int b, float c, int d, int e, int f, int g, int h);
void* lumpFindResource__FPcT0(char* lump, char* name);
int soundPlaySequence__FP15AnimationHeaderG6Point3f(void* header, Point3 point, float volume);
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
static char beam_sound_name[] = "spel_ew_wbeam.anm";
typedef int (*Slot)(void*, int, int);
}

extern "C" void __15SpellWizardBeamiP8CreatureP5Skill(void* self, int rank, void* creature, void* skill) {
    unsigned char* o = static_cast<unsigned char*>(self);
    store_word(o, 4, 0);
    o[0] = static_cast<unsigned char>(rank);
    store_word(o, 12, address_word(creature));
    const unsigned state = syncRandState * 0x19660du + 0x3c6ef35fu;
    store_word(o, 20, address_word(vt_Spell));
    store_word(o, 8, address_word(skill));
    syncRandState = state;
    store_word(o, 16, state >> 16);
    store_word(o, 20, address_word(vt_WizardBeam));
    float hue = static_cast<float>(eRand__5Spell(o)) * float_bits(0x37800000u);
    const unsigned c1 = hsv__Ffff(hue, float_bits(0x3f4cccccu), 1.0f);
    const unsigned c1r = c1 & 0xffu, c1g = (c1 >> 8) & 0xffu, c1b = (c1 >> 16) & 0xffu;
    store_word(o, 28, static_cast<unsigned>(P_StartTrail__Fiifiiiii(1, 200, 2.0f, 5, c1r, c1g, c1b, 128)));
    hue += 0.5f;
    const unsigned c2 = hsv__Ffff(fmodf(hue, 1.0f), float_bits(0x3f666666u), 1.0f);
    const unsigned c3 = hsv__Ffff(fmodf(hue, 1.0f), float_bits(0x3f199999u), 1.0f);
    const unsigned c2r = c2 & 0xffu, c2g = (c2 >> 8) & 0xffu, c2b = (c2 >> 16) & 0xffu;
    store_word(o, 32, static_cast<unsigned>(P_StartTrail__Fiifiiiii(1, 100, 2.0f, 5, c2r, c2g, c2b, 128)));
    store_word(o, 36, static_cast<unsigned>(P_StartTrail__Fiifiiiii(1, 100, 2.0f, 5, c3 & 0xffu, (c3 >> 8) & 0xffu, (c3 >> 16) & 0xffu, 128)));
    o[25] = 0;
    o[26] = 0;
    for (unsigned i = 0; i < 5; ++i) {
        store_word(o, 852 + 8 * i, 0xfffe7960u);
        store_word(o, 848 + 8 * i, static_cast<unsigned>(P_StartTrail__Fiifiiiii(6, 4000, 2.0f, 5, c1r, c1g, c1b, 32)));
    }
    unsigned char* s = pointer_word(o, 8);
    const unsigned char* entry = pointer_word(s, 96) + 88;
    int result = reinterpret_cast<Slot>(word_pointer(load_word(entry, 4)))(s + *reinterpret_cast<const short*>(entry), 1, static_cast<signed char>(o[0]));
    s = pointer_word(o, 8);
    const float doubled = static_cast<float>(result * 2);
    entry = pointer_word(s, 96) + 88;
    store_float(o, 840, doubled);
    store_float(o, 844, doubled);
    result = reinterpret_cast<Slot>(word_pointer(load_word(entry, 4)))(s + *reinterpret_cast<const short*>(entry), 3, static_cast<signed char>(o[0]));
    store_float(o, 836, static_cast<float>(result * 3));
    for (unsigned i = 0; i < 6; ++i) store_float(o, 812 + 4 * i, load_float(o, 836));
    store_word(o, 888, c1);
    store_word(o, 892, static_cast<unsigned>(P_StartTrail__Fiifiiiii(1, 300, 2.0f, 5, c1r, c1g, c1b, 128)));
    s = pointer_word(o, 8);
    entry = pointer_word(s, 96) + 88;
    result = reinterpret_cast<Slot>(word_pointer(load_word(entry, 4)))(s + *reinterpret_cast<const short*>(entry), 2, static_cast<signed char>(o[0]));
    store_word(o, 184, static_cast<unsigned>(result));
    if (result >= 7) store_word(o, 184, 6);
    if (static_cast<int>(load_word(o, 184)) > 0) {
        int i = 0;
        do {
            store_word(o, 188 + 4 * static_cast<unsigned>(i), static_cast<unsigned>(P_StartTrail__Fiifiiiii(1, 100, 2.0f, 5, c1r, c1g, c1b, 128)));
            store_word(o, 212 + 4 * static_cast<unsigned>(i), static_cast<unsigned>(P_StartTrail__Fiifiiiii(1, 100, 2.0f, 5, c2r, c2g, c2b, 128)));
            ++i;
        } while (i < static_cast<int>(load_word(o, 184)));
    }
    void* lump = lumpFindResource__FPcT0(fx_lump_name, beam_sound_name);
    soundPlaySequence__FP15AnimationHeaderG6Point3f(lump, *reinterpret_cast<const Point3*>(pointer_word(o, 12) + 16), 1.0f);
}
