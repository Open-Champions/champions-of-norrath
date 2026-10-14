// Observed __15SpellFireFlaresiP8CreatureG6Point3P5Skill at 0x002a3ad8, 1032 bytes in .text. Arguments: the object, the
// rank, the creature, a point and the skill (the fifth argument arrives in the eighth-register slot). Returns nothing. The
// sync random state is advanced as state = state * 0x19660d + 0x3c6ef35f (32-bit).
// Steps, in order:
//  1. Byte +0 = the low byte of the rank. Word +20 = _vt$5Spell. Word +4 = 0, word +12 = creature, word +8 = skill. The sync
//     state becomes the new value, word +24 (a list head) = 0, word +16 = its upper 16 bits, word +20 = _vt$15SpellFireFlares.
//  2. The point is copied to +92. A 48-byte block at +44 is initialised: bytes +8 and +9 = 0; halfwords +10 and +18 = 255, +12
//     and +20 = 128, +16 and +24 = -16, +14 and +22 = 0; floats +28 and +36 = 0.0; floats +32 and +40 = 1.0f / 0.0f computed at
//     run time (the largest float on this FPU); float +44 = the image float 0x3f09fbd6. lightAdd is called with that block.
//  3. Word +28 = creature. Word +104 = 2 * rank + 65. Let q = (2 * rank + 65) / 12 (signed). Word +32 = rank, word +36 = skill,
//     word +112 = 0, word +108 = 4500 / q. The skill table (the word at skill+96) entry at byte 88 is called as
//     (skill + adjustment, 2, rank) and word +40 = frameNm + 60 * result. Float +100 += the image float 0x37b88ca3;
//     lightUpdatePosition is called with the block at +44.
//  4. The same table entry is called again with 3 giving N, limited to at most 16. When N > 0, the step is the signed
//     halfword of 0xfffe / N. For N iterations, with an angle starting at 0 and growing by the step each time: X = icos(angle)
//     * float(word +112), Y = isin(angle) * float(word +112), Z = 0, each plus the corresponding component of the point. A
//     block of 108 bytes is allocated; when it exists, the GameObject base constructor is called as (block, a copy of
//     (X, Y, Z), 0, 0); block+12 |= 0x421d; block+72 = _vt$9FireFlare; words +76, +84 and +80 = the object's words +28, +36 and
//     +32; the copy of (X, Y, Z) is stored at block+16; float block+28 = 24.0f; halfword block+92 = the angle truncated to
//     16 bits; word block+88 = 0; halfword block+94 = 0; objectUpdateInGrid(block); objectAddToRunList(block). When the block
//     exists, listOnewayLink(object+24, block, block).
//  5. The lump "fx.lmp" resource "spel_ew_flares.anm" is passed to soundPlaySequence(lump, a copy of the creature's 12 bytes at
//     +16, 1.0f); word +116 = its result; soundMakePersistant(that result).
// Inferred: a fire-flares spell that scatters up to sixteen flare objects around a point. Unknown: the meanings of the fields
// and constants. Word +112 is stored as zero before it is used as a multiplier, so every flare is created at the point itself
// (this is read from the instructions and the paired comparison, not guessed). The division by zero when q is 0 is not traced:
// this source divides plainly. Nothing here ran on the EE; checks are the host contract in this directory and a private
// paired run.
#if defined(NORRATH_HOST_ARENA)
// Host checks only: stored 32-bit addresses are arena offsets; the check supplies both conversions.
extern "C" unsigned char* norrath_host_pointer(unsigned address);
extern "C" unsigned norrath_host_address(const void* pointer);
#endif
struct Point3 { float x, y, z; };
struct ListHead { void* first; unsigned unknown[3]; };

extern "C" {
extern unsigned char vt_Spell[] asm("_vt$5Spell");
extern unsigned char vt_FireFlares[] asm("_vt$15SpellFireFlares");
extern unsigned char vt_FireFlare[] asm("_vt$9FireFlare");
extern unsigned syncRandState;
extern unsigned frameNm;
void lightAdd__FP5Light(void* light);
void lightUpdatePosition__FP5Light(void* light);
float icos__Fi(int angle);
float isin__Fi(int angle);
void* blockAlloc__Fi(int size);
void* __10GameObjectG6Point3PQ210GameObject11resourceMapPPc(void* object, Point3 point, void* map, char** arguments);
void objectUpdateInGrid__FP10GameObject(void* object);
void objectAddToRunList__FP10GameObject(void* object);
void listOnewayLink__FP8ListHeadT0Pv(ListHead* list, void* first, void* second);
void* lumpFindResource__FPcT0(char* lump, char* name);
int soundPlaySequence__FP15AnimationHeaderG6Point3f(void* header, Point3 point, float volume);
void soundMakePersistant__Fi(int sound);
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
static char flares_sound_name[] = "spel_ew_flares.anm";
}

extern "C" void __15SpellFireFlaresiP8CreatureG6Point3P5Skill(void* self, int rank, void* creature, Point3 point, void* skill) {
    unsigned char* o = static_cast<unsigned char*>(self);
    volatile float zero = 0.0f;
    const float largest = 1.0f / zero;
    o[0] = static_cast<unsigned char>(rank);
    store_word(o, 20, address_word(vt_Spell));
    __asm__ volatile("" : : "r"(o) : "memory");
    store_word(o, 4, 0);
    store_word(o, 12, address_word(creature));
    store_word(o, 8, address_word(skill));
    const unsigned state = syncRandState * 0x19660du + 0x3c6ef35fu;
    syncRandState = state;
    store_word(o, 24, 0);
    store_word(o, 16, state >> 16);
    store_word(o, 20, address_word(vt_FireFlares));
    unsigned char* light = o + 44;
    light[9] = 0;
    *reinterpret_cast<Point3*>(o + 92) = point;
    store_float(light, 40, largest);
    store_float(light, 44, float_bits(0x3f09fbd6u));
    store_float(light, 28, 0.0f);
    store_float(light, 36, 0.0f);
    store_float(light, 32, largest);
    store_half(light, 18, 255);
    store_half(light, 20, 128);
    store_half(light, 24, 0xfff0);
    store_half(light, 10, 255);
    store_half(light, 12, 128);
    store_half(light, 16, 0xfff0);
    store_half(light, 14, 0);
    store_half(light, 22, 0);
    light[8] = 0;
    lightAdd__FP5Light(light);

    store_word(o, 28, address_word(creature));
    const int scaled = rank * 2 + 65;
    store_word(o, 104, static_cast<unsigned>(scaled));
    const int quotient = scaled / 12;
    unsigned char* s = static_cast<unsigned char*>(skill);
    const unsigned char* entry = pointer_word(s, 96) + 88;
    typedef int (*Slot)(void*, int, int);
    store_word(o, 32, static_cast<unsigned>(rank));
    store_word(o, 36, address_word(skill));
    store_word(o, 112, 0);
    store_word(o, 108, static_cast<unsigned>(4500 / quotient));
    const int result = reinterpret_cast<Slot>(word_pointer(load_word(entry, 4)))(s + *reinterpret_cast<const short*>(entry), 2, rank);
    store_word(o, 40, frameNm + 60u * static_cast<unsigned>(result));
    store_float(o, 100, load_float(o, 100) + float_bits(0x37b88ca3u));
    lightUpdatePosition__FP5Light(light);

    s = pointer_word(o, 36);
    entry = pointer_word(s, 96) + 88;
    int count = reinterpret_cast<Slot>(word_pointer(load_word(entry, 4)))(s + *reinterpret_cast<const short*>(entry), 3,
                                                                          static_cast<int>(load_word(o, 32)));
    if (count > 16) count = 16;
    if (count > 0) {
        const int step = static_cast<short>(0xfffe / count);
        int angle = 0;
        for (int i = count; i != 0; --i) {
            const float x = icos__Fi(angle) * static_cast<float>(static_cast<int>(load_word(o, 112)));
            const float y = isin__Fi(angle) * static_cast<float>(static_cast<int>(load_word(o, 112)));
            Point3 at;
            at.x = x + point.x;
            at.y = y + point.y;
            at.z = 0.0f + point.z;
            unsigned char* block = static_cast<unsigned char*>(blockAlloc__Fi(108));
            const unsigned short angle_half = static_cast<unsigned short>(angle);
            angle += step;
            if (block != nullptr) {
                const Point3 copy = at;
                const unsigned rank_word = load_word(o, 32);
                const unsigned creature_word = load_word(o, 28);
                const unsigned skill_word = load_word(o, 36);
                __10GameObjectG6Point3PQ210GameObject11resourceMapPPc(block, copy, nullptr, nullptr);
                store_word(block, 72, address_word(vt_FireFlare));
                store_word(block, 12, load_word(block, 12) | 0x421du);
                store_word(block, 76, creature_word);
                store_word(block, 84, skill_word);
                store_word(block, 80, rank_word);
                *reinterpret_cast<Point3*>(block + 16) = copy;
                store_float(block, 28, float_bits(0x41c00000u));
                store_half(block, 92, angle_half);
                store_word(block, 88, 0);
                store_half(block, 94, 0);
                objectUpdateInGrid__FP10GameObject(block);
                objectAddToRunList__FP10GameObject(block);
                listOnewayLink__FP8ListHeadT0Pv(reinterpret_cast<ListHead*>(o + 24), block, block);
            }
        }
    }
    void* lump = lumpFindResource__FPcT0(fx_lump_name, flares_sound_name);
    const int sound = soundPlaySequence__FP15AnimationHeaderG6Point3f(lump, *reinterpret_cast<const Point3*>(pointer_word(o, 12) + 16), 1.0f);
    store_word(o, 116, static_cast<unsigned>(sound));
    soundMakePersistant__Fi(sound);
}
