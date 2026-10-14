// Observed __12SpellBlessediP8CreatureP5Skill at 0x002a5f70, 856 bytes in .text. Arguments: the object, the rank, the
// creature and the skill. Returns nothing. The sync random state is advanced as state = state * 0x19660d + 0x3c6ef35f
// (32-bit), and its new value's upper 16 bits are kept. eRand(object) is the image's Spell random step; each use below
// is a separate call.
// Steps, in order:
//  1. Byte +0 = the low byte of the rank. Word +20 = _vt$5Spell. Word +4 = 0, word +12 = creature, word +8 = skill.
//  2. The sync state becomes the new value. Word +16 = the new value shifted right by 16. Word +20 = _vt$12SpellBlessed.
//  3. The 12 bytes at the creature's +16 are copied to the object's +240. A 48-byte block at +192 is initialised: bytes +8
//     and +9 = 0; halfwords +10, +12, +18 and +20 = 75, +14 and +22 = 200, +16 and +24 = 255; floats +28 and +36 = 0.0;
//     floats +32 and +40 = 1.0f / 0.0f computed at run time (the largest float on this FPU); float +44 = the image float
//     0x3f266677 (about 0.65). lightAdd is called with that block.
//  4. Byte +0, word +12 and word +8 are stored again. The skill table (the word at skill+96) entry at byte 88 is called as
//     (skill + adjustment, 1, the signed byte at +0); word +180 = frameNm + 60 * result.
//  5. For i = 0 to 5, an entry of three floats at object + 24 + 12*i is filled: its first float = float(i * 10922); its
//     second = 2 * (float(eRand) * 2^-16 * the creature's float at +28); its third = the creature's float at +32 * 0.5 +
//     float(eRand) * 2^-16 * (the creature's float at +32 * 0.5), reading the creature's +32 once before the call and once
//     after. A vector at object + 96 + 12*i is then drawn by rejection: each component is float(eRand) * 2^-16 * 2 - 1.0;
//     the three are drawn again while 1.0 < x*x + y*y + z*z (summed in that order). Finally x becomes x - 160.0f for even i
//     and x + 160.0f for odd i.
//  6. The 12 bytes at the creature's +16 are copied to the object's +168.
// Inferred: a blessed spell that scatters six sparkles inside a unit sphere around the creature. Unknown: the meanings of
// the fields, constants, and the entries' consumers. Nothing here ran on the EE; checks are the host contract in this
// directory and a private paired run.
#if defined(NORRATH_HOST_ARENA)
// Host checks only: stored 32-bit addresses are arena offsets; the check supplies both conversions.
extern "C" unsigned char* norrath_host_pointer(unsigned address);
extern "C" unsigned norrath_host_address(const void* pointer);
#endif
struct Point3 { float x, y, z; };
struct ListHead { void* first; unsigned unknown[3]; };

extern "C" {
extern unsigned char vt_Spell[] asm("_vt$5Spell");
extern unsigned char vt_Blessed[] asm("_vt$12SpellBlessed");
extern unsigned syncRandState;
extern unsigned frameNm;
void lightAdd__FP5Light(void* light);
int eRand__5Spell(void* spell);
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
}

extern "C" void __12SpellBlessediP8CreatureP5Skill(void* self, int rank, void* creature, void* skill) {
    unsigned char* o = static_cast<unsigned char*>(self);
    unsigned char* c = static_cast<unsigned char*>(creature);
    o[0] = static_cast<unsigned char>(rank);
    store_word(o, 20, address_word(vt_Spell));
    __asm__ volatile("" : : "r"(o) : "memory");
    store_word(o, 4, 0);
    store_word(o, 12, address_word(creature));
    store_word(o, 8, address_word(skill));
    volatile float zero = 0.0f;
    const float largest = 1.0f / zero;
    const unsigned state = syncRandState * 0x19660du + 0x3c6ef35fu;
    syncRandState = state;
    store_word(o, 16, state >> 16);
    store_word(o, 20, address_word(vt_Blessed));
    const WordTriple position = *reinterpret_cast<const WordTriple*>(c + 16);
    unsigned char* light = o + 192;
    light[9] = 0;
    *reinterpret_cast<WordTriple*>(o + 240) = position;
    store_float(light, 40, largest);
    store_float(light, 44, float_bits(0x3f266677u));
    store_half(light, 20, 75);
    store_half(light, 22, 200);
    store_half(light, 24, 255);
    store_float(light, 28, 0.0f);
    store_float(light, 36, 0.0f);
    store_float(light, 32, largest);
    store_half(light, 10, 75);
    store_half(light, 12, 75);
    store_half(light, 14, 200);
    store_half(light, 16, 255);
    store_half(light, 18, 75);
    light[8] = 0;
    lightAdd__FP5Light(light);

    o[0] = static_cast<unsigned char>(rank);
    store_word(o, 12, address_word(creature));
    store_word(o, 8, address_word(skill));
    unsigned char* s = static_cast<unsigned char*>(skill);
    const unsigned char* entry = pointer_word(s, 96) + 88;
    const short adjustment = *reinterpret_cast<const short*>(entry);
    const unsigned function = *reinterpret_cast<const unsigned*>(entry + 4);
    const int result = reinterpret_cast<int (*)(void*, int, int)>(word_pointer(function))(
        s + adjustment, 1, static_cast<signed char>(rank));
    store_word(o, 180, frameNm + 60u * static_cast<unsigned>(result));

    const float scale = float_bits(0x37800000u);
    int angle = 0;
    for (int i = 0; i < 6; ++i, angle += 10922) {
        unsigned char* entry_at = o + 24 + 12 * i;
        unsigned char* vector = o + 96 + 12 * i;
        store_float(entry_at, 0, static_cast<float>(angle));
        float second = static_cast<float>(eRand__5Spell(o)) * scale * load_float(pointer_word(o, 12), 28);
        const float third_base = load_float(pointer_word(o, 12), 32);
        store_float(entry_at, 4, second + second);
        float draw = static_cast<float>(eRand__5Spell(o)) * scale * (load_float(pointer_word(o, 12), 32) * 0.5f);
        store_float(entry_at, 8, third_base * 0.5f + draw);
        float x, y, z, sum;
        do {
            x = static_cast<float>(eRand__5Spell(o)) * scale;
            x = x + x - 1.0f;
            store_float(vector, 0, x);
            y = static_cast<float>(eRand__5Spell(o)) * scale;
            y = y + y - 1.0f;
            store_float(vector, 4, y);
            z = static_cast<float>(eRand__5Spell(o)) * scale;
            z = z + z - 1.0f;
            sum = x * x + y * y;
            sum = sum + z * z;
            store_float(vector, 8, z);
        } while (1.0f < sum);
        store_float(vector, 0, (i & 1) == 0 ? x - 160.0f : x + 160.0f);
    }
    *reinterpret_cast<WordTriple*>(o + 168) = *reinterpret_cast<const WordTriple*>(pointer_word(o, 12) + 16);
}
