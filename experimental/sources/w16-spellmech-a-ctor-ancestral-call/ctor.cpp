// Observed __18SpellAncestralCalliP8CreatureP5Skill at 0x002a7620, 844 bytes in .text. Arguments: the object, the rank, the
// creature and the skill. Returns nothing. The sync random state is advanced as state = state * 0x19660d + 0x3c6ef35f
// (32-bit).
// Steps, in order:
//  1. Byte +0 = the low byte of the rank. Word +20 = _vt$5Spell. Word +8 = skill, word +4 = 0, word +12 = creature. The sync
//     state becomes the new value. Word +20 = _vt$18SpellAncestralCall, word +16 = its upper 16 bits, word +24 = creature,
//     word +28 = 0.
//  2. lumpFindResource("fx.lmp", "spel_bw_ancestral.anm") is passed to soundPlaySequence(lump, creature, 1.0f).
//  3. A byte K is read from the image's player table: the entry index is the creature's word at +960, entries are 23984 bytes
//     apart, and K is the signed byte at offset 23582 in the entry. A name is chosen by K and written with sprintf (no
//     arguments): 0 gives "ohw_warcry.anm"; 5 and any value not listed give "nw_warcry.anm"; 2 gives "dha_warcry.anm"; 1
//     gives "dhs_warcry.anm"; 3 gives "l_warcry.anm"; 7 gives "dw_warcry.anm".
//  4. animAddOneShot(creature+288, lumpFindResource(the first word of the object at creature+36, the chosen name), c, c, 34)
//     with c the image float 0x3dcccccc (about 0.1).
//  5. When the creature's word at +12 has bit 0x20000 set, return here.
//  6. blockAlloc(188); when it returns a block, a LightEffect is constructed in it from (the point creature+16 with 36.0f
//     added to z and 0.0f added to x, then twelve floats 0, 155, 255, -96, 0, 0, 0, 0, 180, 144, 0, 5, the creature, 0). The
//     twelve floats' meanings are not known.
//  7. The word at creature+1400 = 6; word +124 = 0; word +40 = frameNm; words +52, +48 and +44 = 0. The skill table (the
//     word at skill+96) entry at byte 88 is called as (skill + adjustment, 2, the signed byte at +0): float +32 = float(result).
//     It is called again with 3: float +36 = float(12 * result). Word +128 = P_CreateParticleSystem().
// Inferred: an ancestral-call (war cry) spell that selects an animation by the caster's class. Unknown: the meanings of K and
// the LightEffect parameters. Nothing here ran on the EE; checks are the host contract in this directory and a private
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
extern unsigned char vt_AncestralCall[] asm("_vt$18SpellAncestralCall");
extern unsigned char player[] asm("player");
extern unsigned syncRandState;
extern unsigned frameNm;
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sound, void* object, float volume);
int sprintf(char* buffer, const char* format, ...);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float first, float second, int id);
void* blockAlloc__Fi(int size);
void __11LightEffectG6Point3ffffffffffffP10GameObjecti(void* self, Point3 point, float a, float b, float c, float d, float e, float f,
                                                      float g, float h, float i, float j, float k, float l, void* object, int flag);
void* P_CreateParticleSystem__Fv();
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
static char ancestral_sound_name[] = "spel_bw_ancestral.anm";
static char name_ohw[] = "ohw_warcry.anm";
static char name_nw[] = "nw_warcry.anm";
static char name_dha[] = "dha_warcry.anm";
static char name_dhs[] = "dhs_warcry.anm";
static char name_l[] = "l_warcry.anm";
static char name_dw[] = "dw_warcry.anm";
}

extern "C" void __18SpellAncestralCalliP8CreatureP5Skill(void* self, int rank, void* creature, void* skill) {
    unsigned char* o = static_cast<unsigned char*>(self);
    unsigned char* c = static_cast<unsigned char*>(creature);
    o[0] = static_cast<unsigned char>(rank);
    store_word(o, 20, address_word(vt_Spell));
    __asm__ volatile("" : : "r"(o) : "memory");
    store_word(o, 8, address_word(skill));
    store_word(o, 4, 0);
    store_word(o, 12, address_word(creature));
    const unsigned state = syncRandState * 0x19660du + 0x3c6ef35fu;
    syncRandState = state;
    store_word(o, 20, address_word(vt_AncestralCall));
    store_word(o, 16, state >> 16);
    store_word(o, 24, address_word(creature));
    store_word(o, 28, 0);
    void* sound = lumpFindResource__FPcT0(fx_lump_name, ancestral_sound_name);
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(sound, creature, 1.0f);

    char buffer[64];
    const signed char kind = static_cast<signed char>(player[load_word(pointer_word(o, 24), 960) * 23984u + 23582u]);
    if (kind == 0) sprintf(buffer, name_ohw);
    else if (kind == 5) sprintf(buffer, name_nw);
    else if (kind == 2) sprintf(buffer, name_dha);
    else if (kind == 1) sprintf(buffer, name_dhs);
    else if (kind == 3) sprintf(buffer, name_l);
    else if (kind == 7) sprintf(buffer, name_dw);
    else sprintf(buffer, name_nw);
    unsigned char* owner = pointer_word(o, 24);
    void* animation = lumpFindResource__FPcT0(reinterpret_cast<char*>(word_pointer(load_word(pointer_word(owner, 36), 0))), buffer);
    animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(owner + 288, animation, float_bits(0x3dccccccu), float_bits(0x3dccccccu), 34);

    if ((load_word(pointer_word(o, 12), 12) & 0x20000u) != 0) return;
    unsigned char* block = static_cast<unsigned char*>(blockAlloc__Fi(188));
    if (block != nullptr) {
        const unsigned char* at = pointer_word(o, 24) + 16;
        Point3 point;
        point.x = load_float(at, 0) + 0.0f;
        point.y = load_float(at, 4);
        point.z = load_float(at, 8) + 36.0f;
        __11LightEffectG6Point3ffffffffffffP10GameObjecti(block, point, 0.0f, 155.0f, 255.0f, -96.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                                          180.0f, 144.0f, 0.0f, 5.0f, pointer_word(o, 24), 0);
    }
    store_word(pointer_word(o, 24), 1400, 6);
    store_word(o, 124, 0);
    store_word(o, 40, frameNm);
    for (int i = 0; i < 3; ++i) store_word(o, 52 - 4 * static_cast<unsigned>(i), 0);
    unsigned char* s = pointer_word(o, 8);
    const unsigned char* entry = pointer_word(s, 96) + 88;
    const short adjustment = *reinterpret_cast<const short*>(entry);
    const unsigned function = *reinterpret_cast<const unsigned*>(entry + 4);
    typedef int (*Slot)(void*, int, int);
    const int first = reinterpret_cast<Slot>(word_pointer(function))(s + adjustment, 2, static_cast<signed char>(o[0]));
    s = pointer_word(o, 8);
    store_float(o, 32, static_cast<float>(first));
    entry = pointer_word(s, 96) + 88;
    const int second = reinterpret_cast<Slot>(word_pointer(load_word(entry, 4)))(s + *reinterpret_cast<const short*>(entry), 3, static_cast<signed char>(o[0]));
    store_float(o, 36, static_cast<float>(second * 12));
    store_word(o, 128, address_word(P_CreateParticleSystem__Fv()));
}
