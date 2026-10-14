// Observed __16SpellFireWeaponsiP8CreatureP5Skill at 0x002a9eb8, 1124 bytes in .text. Arguments: the object, the rank, the
// creature and the skill. Returns nothing. The sync random state is advanced as state = state * 0x19660d + 0x3c6ef35f
// (32-bit). "Skill call n" below is the skill table (the word at skill+96) entry at byte 88 called as
// (skill + adjustment, n, the signed byte at object+0), returning an int.
// Steps, in order:
//  1. Byte +0 = the low byte of the rank. Word +20 = _vt$5Spell. Words +12, +8 = creature, skill; word +4 = 0. The sync state
//     becomes the new value, word +16 = its upper 16 bits, word +20 = _vt$16SpellFireWeapons.
//  2. The creature's 12 bytes at +16 are copied to +80. A 48-byte block at +32 is initialised: bytes +8 and +9 = 0; halfwords
//     +10 and +18 = 255, +12 and +20 = 128, +14, +16, +22 and +24 = 0; floats +28 and +36 = 0.0; floats +32 and +40 = 1.0f / 0.0f
//     computed at run time (the largest float on this FPU); float +44 = the image float 0x3f266655. lightAdd is called with
//     that block.
//  3. Byte +0, words +12 and +8 are stored again. Skill call 1 gives R1; word +24 = frameNm + 60 * R1. Skill call 2 gives R2;
//     word +92 = frameNm and word +28 = 12 * R2.
//  4. Using the image's player table: the entry index is the creature's word at +960, entries are 23984 bytes apart. A = the
//     word at entry offset 21960 + 4 * (the signed byte at entry offset 23572); B = the word at entry offset 21968. Skill call
//     0 gives R0. Skill call 1 is called again and 60 times its result is V.
//  5. For each of A and then B (skipping a null one), with the image float 0.2f (0x3e4ccccc): the object has four 8-byte records
//     starting at +36 whose first halfword is a type (-1 means empty). If a record of type 21 exists and its second halfword
//     (+2) is non-zero, nothing is done. Otherwise F = float(R0), F = F + F * 0.2f, G = F + 0.5f for F >= 0 or F - 0.5f
//     otherwise, truncated to an int. The first record whose type is -1 or 21 gets type 21, halfword +2 = R0, +4 = G, +6 = V
//     (low 16 bits of each); when no record qualifies nothing is stored.
//  6. lumpFindResource("fx.lmp", "spel_ew_heatwpn.anm") is passed to soundPlaySequence(lump, a copy of the creature's 12 bytes at
//     +16, 1.0f).
// Inferred: a fire-weapons spell (the cold-weapons spell's counterpart) tagging the player's weapon records with a type-21
// effect and adding a light. Unknown: the meanings of the records' fields and the constants. Nothing here ran on the EE; checks
// are the host contract in this directory and a private paired run.
#if defined(NORRATH_HOST_ARENA)
// Host checks only: stored 32-bit addresses are arena offsets; the check supplies both conversions.
extern "C" unsigned char* norrath_host_pointer(unsigned address);
extern "C" unsigned norrath_host_address(const void* pointer);
#endif
struct Point3 { float x, y, z; };
struct ListHead { void* first; unsigned unknown[3]; };

extern "C" {
extern unsigned char vt_Spell[] asm("_vt$5Spell");
extern unsigned char vt_FireWeapons[] asm("_vt$16SpellFireWeapons");
extern unsigned char player[] asm("player");
extern unsigned syncRandState;
extern int frameNm;
void lightAdd__FP5Light(void* light);
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
static char fire_sound_name[] = "spel_ew_heatwpn.anm";
typedef int (*Slot)(void*, int, int);

inline short load_short(const unsigned char* base, unsigned offset) {
    return *reinterpret_cast<const short*>(base + offset);
}
// Applies the type-21 record update described in the header to one weapon block.
void tag_weapon(unsigned char* weapon, int r0, short v, float scale) {
    unsigned existing = 0;
    for (unsigned j = 0; j < 4; ++j) {
        if (load_short(weapon, 36 + 8 * j) == 21) {
            existing = static_cast<unsigned short>(load_short(weapon, 38 + 8 * j));
            break;
        }
    }
    if (existing != 0) return;
    float f = static_cast<float>(r0);
    f = f + f * scale;
    f = 0.0f <= f ? f + 0.5f : f - 0.5f;
    const int g = static_cast<int>(f);
    for (unsigned j = 0; j < 4; ++j) {
        const short type = load_short(weapon, 36 + 8 * j);
        if (type == -1 || type == 21) {
            store_half(weapon, 36 + 8 * j, 21);
            store_half(weapon, 38 + 8 * j, static_cast<unsigned short>(r0));
            store_half(weapon, 40 + 8 * j, static_cast<unsigned short>(g));
            store_half(weapon, 42 + 8 * j, static_cast<unsigned short>(v));
            break;
        }
    }
}
}

extern "C" void __16SpellFireWeaponsiP8CreatureP5Skill(void* self, int rank, void* creature, void* skill) {
    unsigned char* o = static_cast<unsigned char*>(self);
    volatile float zero = 0.0f;
    const float largest = 1.0f / zero;
    o[0] = static_cast<unsigned char>(rank);
    store_word(o, 20, address_word(vt_Spell));
    store_word(o, 12, address_word(creature));
    store_word(o, 8, address_word(skill));
    const unsigned state = syncRandState * 0x19660du + 0x3c6ef35fu;
    store_word(o, 4, 0);
    syncRandState = state;
    store_word(o, 16, state >> 16);
    store_word(o, 20, address_word(vt_FireWeapons));
    unsigned char* light = o + 32;
    light[9] = 0;
    *reinterpret_cast<WordTriple*>(o + 80) = *reinterpret_cast<const WordTriple*>(static_cast<const unsigned char*>(creature) + 16);
    store_float(light, 40, largest);
    store_float(light, 44, float_bits(0x3f266655u));
    store_float(light, 28, 0.0f);
    store_float(light, 36, 0.0f);
    store_float(light, 32, largest);
    store_half(light, 18, 255);
    store_half(light, 10, 255);
    store_half(light, 20, 128);
    store_half(light, 12, 128);
    store_half(light, 14, 0);
    store_half(light, 16, 0);
    store_half(light, 22, 0);
    store_half(light, 24, 0);
    light[8] = 0;
    lightAdd__FP5Light(light);

    o[0] = static_cast<unsigned char>(rank);
    store_word(o, 12, address_word(creature));
    store_word(o, 8, address_word(skill));
    unsigned char* s = static_cast<unsigned char*>(skill);
    const unsigned char* entry = pointer_word(s, 96) + 88;
    int result = reinterpret_cast<Slot>(word_pointer(load_word(entry, 4)))(s + *reinterpret_cast<const short*>(entry), 1, static_cast<signed char>(rank));
    store_word(o, 24, static_cast<unsigned>(frameNm) + 60u * static_cast<unsigned>(result));
    s = pointer_word(o, 8);
    entry = pointer_word(s, 96) + 88;
    result = reinterpret_cast<Slot>(word_pointer(load_word(entry, 4)))(s + *reinterpret_cast<const short*>(entry), 2, static_cast<signed char>(o[0]));
    unsigned char* owner = pointer_word(o, 12);
    store_word(o, 92, static_cast<unsigned>(frameNm));
    store_word(o, 28, 12u * static_cast<unsigned>(result));
    s = pointer_word(o, 8);
    unsigned char* table = player + load_word(owner, 960) * 23984u;
    const unsigned char* slot = pointer_word(s, 96) + 88;
    const signed char which = static_cast<signed char>(table[23572]);
    unsigned char* second = pointer_word(table, 21968);
    unsigned char* first = pointer_word(table + 4 * static_cast<int>(which), 21960);
    const int r0 = reinterpret_cast<Slot>(word_pointer(load_word(slot, 4)))(s + *reinterpret_cast<const short*>(slot), 0, static_cast<signed char>(o[0]));
    s = pointer_word(o, 8);
    slot = pointer_word(s, 96) + 88;
    result = reinterpret_cast<Slot>(word_pointer(load_word(slot, 4)))(s + *reinterpret_cast<const short*>(slot), 1, static_cast<signed char>(o[0]));
    const short v = static_cast<short>(60 * result);
    if (first != nullptr) tag_weapon(first, r0, v, float_bits(0x3e4cccccu));
    if (second != nullptr) tag_weapon(second, r0, v, float_bits(0x3e4cccccu));
    void* lump = lumpFindResource__FPcT0(fx_lump_name, fire_sound_name);
    soundPlaySequence__FP15AnimationHeaderG6Point3f(lump, *reinterpret_cast<const Point3*>(pointer_word(o, 12) + 16), 1.0f);
}
