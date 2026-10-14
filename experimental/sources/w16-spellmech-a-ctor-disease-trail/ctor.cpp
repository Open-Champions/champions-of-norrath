// Observed __17SpellDiseaseTrailG6Point3iP8CreatureP5Skill at 0x00294308, 708 bytes in .text. Arguments: the object, a point,
// the rank, the creature and the skill (the fifth argument arrives in the eighth-register slot). Returns nothing. The sync
// random state is advanced as state = state * 0x19660d + 0x3c6ef35f (32-bit).
// Steps, in order:
//  1. Byte +0 = the low byte of the rank. Word +12 = creature, word +20 = _vt$5Spell, word +4 = 0, word +8 = skill. The sync
//     state becomes the new value and word +16 = its upper 16 bits. Word +24 (a list head) = 0, word +20 =
//     _vt$17SpellDiseaseTrail. The point is copied to +28. Float +44 = float(int(float(frameNm) + 180.0f)).
//  2. blockAlloc(104). When it returns a block B: the GameObject base constructor is called as (B, a copy of the point, 0, 0);
//     B+12 |= 0x4421f; B+72 = _vt$12DiseaseTrail; B+92 = 0 (a list head); when the creature is non-null,
//     listOnewayLink(B+92, creature, creature). B+100 = skill and B+96 = the signed rank byte. The point is copied to B+16;
//     float B+28 = 33.0f; float B+32 = 36.0f; objectUpdateInGrid(B). The point is copied to B+76. The skill table (the word at
//     skill+96) entry at byte 88 is called as (skill + adjustment, 1, B+96's value); B+88 = frameNm + 60 * result.
//     objectAddToRunList(B).
//  3. When B exists: listOnewayLink(object+24, B, B); then the lump "fx.lmp" resource "spel_de_distrail.anm" is passed to
//     soundPlaySequence(lump, a copy of the creature's 12 bytes at +16, 1.0f).
// Inferred: a disease-trail spell that spawns a trail object bound to the caster. Unknown: the meanings of the fields and
// constants, and what the object at +44 is used for. Nothing here ran on the EE; checks are the host contract in this
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
extern unsigned char vt_DiseaseTrailSpell[] asm("_vt$17SpellDiseaseTrail");
extern unsigned char vt_DiseaseTrail[] asm("_vt$12DiseaseTrail");
extern unsigned syncRandState;
extern int frameNm;
void* blockAlloc__Fi(int size);
void* __10GameObjectG6Point3PQ210GameObject11resourceMapPPc(void* object, Point3 point, void* map, char** arguments);
void listOnewayLink__FP8ListHeadT0Pv(ListHead* list, void* first, void* second);
void objectUpdateInGrid__FP10GameObject(void* object);
void objectAddToRunList__FP10GameObject(void* object);
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
static char trail_sound_name[] = "spel_de_distrail.anm";
}

extern "C" void __17SpellDiseaseTrailG6Point3iP8CreatureP5Skill(void* self, Point3 point, int rank, void* creature, void* skill) {
    unsigned char* o = static_cast<unsigned char*>(self);
    o[0] = static_cast<unsigned char>(rank);
    store_word(o, 12, address_word(creature));
    store_word(o, 20, address_word(vt_Spell));
    __asm__ volatile("" : : "r"(o) : "memory");
    store_word(o, 4, 0);
    store_word(o, 8, address_word(skill));
    const unsigned state = syncRandState * 0x19660du + 0x3c6ef35fu;
    syncRandState = state;
    store_word(o, 16, state >> 16);
    store_word(o, 12, address_word(creature));
    store_word(o, 24, 0);
    store_word(o, 20, address_word(vt_DiseaseTrailSpell));
    o[0] = static_cast<unsigned char>(rank);
    store_word(o, 8, address_word(skill));
    *reinterpret_cast<Point3*>(o + 28) = point;
    store_float(o, 44, static_cast<float>(static_cast<int>(static_cast<float>(frameNm) + 180.0f)));

    unsigned char* b = static_cast<unsigned char*>(blockAlloc__Fi(104));
    if (b != nullptr) {
        const Point3 first = *reinterpret_cast<const Point3*>(o + 28);
        const Point3 second = *reinterpret_cast<const Point3*>(o + 28);
        const signed char rank_byte = static_cast<signed char>(o[0]);
        unsigned char* owner = pointer_word(o, 12);
        unsigned char* spell_skill = pointer_word(o, 8);
        __10GameObjectG6Point3PQ210GameObject11resourceMapPPc(b, first, nullptr, nullptr);
        store_word(b, 12, load_word(b, 12) | 0x4421fu);
        store_word(b, 72, address_word(vt_DiseaseTrail));
        store_word(b, 92, 0);
        if (owner != nullptr) listOnewayLink__FP8ListHeadT0Pv(reinterpret_cast<ListHead*>(b + 92), owner, owner);
        store_word(b, 100, address_word(spell_skill));
        store_word(b, 96, static_cast<unsigned>(static_cast<int>(rank_byte)));
        *reinterpret_cast<Point3*>(b + 16) = first;
        store_float(b, 28, float_bits(0x42040000u));
        store_float(b, 32, float_bits(0x42100000u));
        objectUpdateInGrid__FP10GameObject(b);
        *reinterpret_cast<Point3*>(b + 76) = second;
        unsigned char* s = pointer_word(b, 100);
        const unsigned char* entry = pointer_word(s, 96) + 88;
        const short adjustment = *reinterpret_cast<const short*>(entry);
        const unsigned function = *reinterpret_cast<const unsigned*>(entry + 4);
        const int result = reinterpret_cast<int (*)(void*, int, int)>(word_pointer(function))(
            s + adjustment, 1, static_cast<int>(load_word(b, 96)));
        store_word(b, 88, static_cast<unsigned>(frameNm) + 60u * static_cast<unsigned>(result));
        objectAddToRunList__FP10GameObject(b);
        listOnewayLink__FP8ListHeadT0Pv(reinterpret_cast<ListHead*>(o + 24), b, b);
        void* lump = lumpFindResource__FPcT0(fx_lump_name, trail_sound_name);
        soundPlaySequence__FP15AnimationHeaderG6Point3f(lump, *reinterpret_cast<const Point3*>(pointer_word(o, 12) + 16), 1.0f);
    }
}
