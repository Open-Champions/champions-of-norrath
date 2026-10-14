// Observed __12SpellCycloneiP8CreatureP5Skill at 0x00299758, 684 bytes in .text. Arguments: the object, the rank, the
// creature and the skill. Returns nothing. The sync random state is advanced as state = state * 0x19660d + 0x3c6ef35f
// (32-bit), and its new value's upper 16 bits are kept.
// Steps, in order:
//  1. Byte +0 = the low byte of the rank. Word +20 = _vt$5Spell. Word +8 = skill, word +4 = 0, word +12 = creature.
//  2. The sync state becomes the new value. Word +20 = _vt$12SpellCyclone, word +16 = the new value shifted right by 16,
//     word +24 = 0 (a list head).
//  3. The 12 bytes at the creature's +16 are copied to the object's +76. A 48-byte block at +28 is initialised: bytes +8 and
//     +9 = 0; halfwords +10 and +18 = 158, +12 and +20 = 111, +14 and +22 = 67, +16 and +24 = 0; floats +28 and +36 =
//     36.0; floats +32 and +40 = the image float 0x3a4a4587; word +44 = 0. lightAdd is called with that block.
//  4. Word +88 = frameNm. Word at creature+1400 = 6. anm = lumpFindResource(first word of the object at creature+36,
//     "cyclone1.anm"); word +120 = anm.
//  5. The skill table (the word at skill+96) entry at byte 88 is called as (skill + adjustment, 1, the signed byte at +0);
//     its integer result is R. Float +104 = float(int word at anm+4) * 0.016666667f * float(R) (image float 0x3c888888).
//     Byte anm+24 = 1 and float anm+20 = the image float 0x3d4ccccc. animAdd(creature+288, anm, 35).
//  6. Words +92 and +96 = 0. Halfword +100 = eRand(object), halfword +102 = eRand(object). The 12 bytes at the creature's
//     +16 are copied to +108.
//  7. If the creature's word at +12 has bit 0x20000 set, return here. Otherwise playerPlaySound(the word at creature+960,
//     10); lumpFindResource("fx.lmp", "spel_bw_cyclone.anm") is passed to soundPlaySequence(lump, creature, 1.0f).
//  8. A local list is filled by objectGetListInRadius(a copy of the creature's +16 position, the halfword at creature+56,
//     480.0f, the list). For each node in order, listOnewayLink(object+24, target, target) where target is the node's word
//     at +12. Then listDestroy(the list).
// Inferred: a cyclone spell that adds a light, starts an animation and links the creatures near the caster. Unknown: the
// meanings of the fields and constants, and the types of eRand's result. Nothing here ran on the EE; checks are the host
// contract in this directory and a private paired run.
#if defined(NORRATH_HOST_ARENA)
// Host checks only: stored 32-bit addresses are arena offsets; the check supplies both conversions.
extern "C" unsigned char* norrath_host_pointer(unsigned address);
extern "C" unsigned norrath_host_address(const void* pointer);
#endif
struct Point3 { float x, y, z; };
struct ListHead { void* first; unsigned unknown[3]; };

extern "C" {
extern unsigned char vt_Spell[] asm("_vt$5Spell");
extern unsigned char vt_Cyclone[] asm("_vt$12SpellCyclone");
extern unsigned syncRandState;
extern unsigned frameNm;
void lightAdd__FP5Light(void* light);
void* lumpFindResource__FPcT0(char* lump, char* name);
void animAdd__FP14AnimationStateP15AnimationHeaderi(void* state, void* header, int flags);
unsigned eRand__5Spell(void* spell);
void playerPlaySound__Fii(int handle, int id);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sound, void* object, float volume);
void objectGetListInRadius__FG6Point3sfP8ListHead(Point3 point, short angle, float radius, ListHead* list);
void listOnewayLink__FP8ListHeadT0Pv(ListHead* list, void* first, void* second);
void listDestroy__FP8ListHead(ListHead* list);
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
static char cyclone_anim_name[] = "cyclone1.anm";
static char fx_lump_name[] = "fx.lmp";
static char cyclone_sound_name[] = "spel_bw_cyclone.anm";
}

extern "C" void __12SpellCycloneiP8CreatureP5Skill(void* self, int rank, void* creature, void* skill) {
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
    store_word(o, 20, address_word(vt_Cyclone));
    store_word(o, 16, state >> 16);
    store_word(o, 24, 0);
    const WordTriple position = *reinterpret_cast<const WordTriple*>(c + 16);
    unsigned char* light = o + 28;
    light[9] = 0;
    *reinterpret_cast<WordTriple*>(o + 76) = position;
    store_float(light, 36, 36.0f);
    store_float(light, 40, float_bits(0x3a4a4587u));
    store_float(light, 28, 36.0f);
    store_float(light, 32, float_bits(0x3a4a4587u));
    store_half(light, 18, 158);
    store_half(light, 10, 158);
    store_half(light, 20, 111);
    store_half(light, 22, 67);
    store_half(light, 12, 111);
    store_half(light, 14, 67);
    store_word(light, 44, 0);
    store_half(light, 16, 0);
    store_half(light, 24, 0);
    light[8] = 0;
    lightAdd__FP5Light(light);

    store_word(c, 1400, 6);
    char* lump = reinterpret_cast<char*>(word_pointer(load_word(pointer_word(c, 36), 0)));
    store_word(o, 88, frameNm);
    unsigned char* anm = static_cast<unsigned char*>(lumpFindResource__FPcT0(lump, cyclone_anim_name));
    store_word(o, 120, address_word(anm));

    unsigned char* s = pointer_word(o, 8);
    const unsigned char* entry = pointer_word(s, 96) + 88;
    const short adjustment = *reinterpret_cast<const short*>(entry);
    const unsigned function = *reinterpret_cast<const unsigned*>(entry + 4);
    const int result = reinterpret_cast<int (*)(void*, int, int)>(word_pointer(function))(
        s + adjustment, 1, static_cast<signed char>(o[0]));
    unsigned char* anm_again = pointer_word(o, 120);
    store_float(o, 104, static_cast<float>(*reinterpret_cast<const int*>(anm_again + 4)) * float_bits(0x3c888888u) *
                        static_cast<float>(result));
    anm_again[24] = 1;
    store_float(anm_again, 20, float_bits(0x3d4cccccu));
    animAdd__FP14AnimationStateP15AnimationHeaderi(c + 288, pointer_word(o, 120), 35);
    store_word(o, 92, 0);
    store_word(o, 96, 0);
    store_half(o, 100, static_cast<unsigned short>(eRand__5Spell(o)));
    store_half(o, 102, static_cast<unsigned short>(eRand__5Spell(o)));
    *reinterpret_cast<WordTriple*>(o + 108) = position;

    unsigned char* owner = pointer_word(o, 12);
    if ((load_word(owner, 12) & 0x20000u) != 0) return;
    playerPlaySound__Fii(static_cast<int>(load_word(c, 960)), 10);
    void* sound = lumpFindResource__FPcT0(fx_lump_name, cyclone_sound_name);
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(sound, creature, 1.0f);

    ListHead list;
    list.first = nullptr;
    const Point3 at = *reinterpret_cast<const Point3*>(c + 16);
    objectGetListInRadius__FG6Point3sfP8ListHead(at, *reinterpret_cast<const short*>(c + 56), 480.0f, &list);
    for (unsigned char* node = static_cast<unsigned char*>(list.first); node != nullptr; node = pointer_word(node, 0)) {
        void* target = pointer_word(node, 12);
        listOnewayLink__FP8ListHeadT0Pv(reinterpret_cast<ListHead*>(o + 24), target, target);
    }
    listDestroy__FP8ListHead(&list);
}
