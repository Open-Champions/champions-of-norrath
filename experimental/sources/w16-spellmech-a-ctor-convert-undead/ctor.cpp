// Observed __18SpellConvertUndeadiP8CreatureP5Skill at 0x00298e10, 508 bytes in .text. Arguments: the object, the rank, the
// creature and the skill. Returns nothing. The sync random state is advanced as state = state * 0x19660d + 0x3c6ef35f (32-bit).
// Steps, in order:
//  1. Byte +0 = the low byte of the rank. Word +8 = skill, word +20 = _vt$5Spell, word +4 = 0, word +12 = creature.
//  2. The sync state becomes the new value. Word +20 = _vt$18SpellConvertUndead, word +24 = 0 (the list head at +24 is
//     emptied), a local list head's first word = 0, word +16 = the new value shifted right by 16, and word +108 = the
//     image word frameNm.
//  3. The skill table (the word at skill+96) entry at byte 88 is called as (skill + adjustment, 0, the signed byte at +0).
//     Its integer result is the bound N of step 6. Halfword +112 = 0 and word +116 = 0.
//  4. lump = lumpFindResource("fx.lmp", "spel_he_convert.anm"). soundPlaySequence(lump, creature, 1.0f).
//  5. A local point = the creature's 12 bytes at +16. objectGetSortedCreatureList(point, the halfword at creature+56,
//     329.0f, 120.0f, the local list, null) fills the list.
//  6. Walk the list from its first node while the first node exists and N, read as a signed int, is positive.
//     Each node's target is the word at node+12. A target is handled when its word at +12 has bit 0x01000000 clear, isBoss
//     is zero for it, its signed byte at +172 is 16, and its float at +876 is at most zero. A handled target is linked by
//     listOnewayLink(self+24, target, target), the count is increased, and P_CreateParticleSystem() is stored at the next
//     word from self+28. After each node the walk stops at a null next node or once the count is no longer below N.
//  7. listDestroy(the local list).
// Inferred: a cleric's convert-undead spell that links nearby undead targets to the caster with particle effects. Unknown:
// the meanings of the fields and the sound lump. Nothing here ran on the EE; the private paired run is described in the notes.
struct Point3 { float x, y, z; };
struct ListHead { void* first; unsigned unknown[3]; };

#if defined(NORRATH_HOST_ARENA)
// Host checks only: stored 32-bit addresses are arena offsets; the check supplies both conversions.
extern "C" unsigned char* norrath_host_pointer(unsigned address);
extern "C" unsigned norrath_host_address(const void* pointer);
#endif
extern "C" {
extern unsigned char vt_Spell[] asm("_vt$5Spell");
extern unsigned char vt_ConvertUndead[] asm("_vt$18SpellConvertUndead");
extern unsigned syncRandState;
extern unsigned frameNm;
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sound, void* object, float volume);
void objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature(Point3 point, short angle, float first, float second, ListHead* list, void* exclude);
void listOnewayLink__FP8ListHeadT0Pv(ListHead* list, void* first, void* second) asm("listOnewayLink__FP8ListHeadT0Pv");
int isBoss__FP10GameObject(void* object);
void* P_CreateParticleSystem__Fv();
void listDestroy__FP8ListHead(ListHead* list);
}

namespace {
static char fx_lump_name[] = "fx.lmp";
static char convert_name[] = "spel_he_convert.anm";

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
inline float float_bits(unsigned bits) {
    union { unsigned u; float f; } value;
    value.u = bits;
    return value.f;
}
inline float load_float(const unsigned char* base, unsigned offset) {
    return *reinterpret_cast<const float*>(base + offset);
}
}

extern "C" void __18SpellConvertUndeadiP8CreatureP5Skill(void* self, int rank, void* creature, void* skill) {
    unsigned char* o = static_cast<unsigned char*>(self);
    unsigned char* c = static_cast<unsigned char*>(creature);
    o[0] = static_cast<unsigned char>(rank);
    store_word(o, 8, address_word(skill));
    store_word(o, 20, address_word(vt_Spell));
    __asm__ volatile("" : : "r"(o) : "memory");
    store_word(o, 4, 0);
    store_word(o, 12, address_word(creature));
    const unsigned state = syncRandState * 0x19660du + 0x3c6ef35fu;
    syncRandState = state;
    store_word(o, 20, address_word(vt_ConvertUndead));
    store_word(o, 24, 0);
    ListHead list;
    list.first = nullptr;
    store_word(o, 16, state >> 16);
    store_word(o, 108, frameNm);

    unsigned char* skill_object = static_cast<unsigned char*>(skill);
    const unsigned char* entry = pointer_word(skill_object, 96) + 88;
    const short adjustment = *reinterpret_cast<const short*>(entry);
    const unsigned function = *reinterpret_cast<const unsigned*>(entry + 4);
    const int bound = reinterpret_cast<int (*)(void*, int, int)>(word_pointer(function))(
        skill_object + adjustment, 0, static_cast<signed char>(o[0]));
    store_half(o, 112, 0);
    store_word(o, 116, 0);

    void* lump = lumpFindResource__FPcT0(fx_lump_name, convert_name);
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lump, pointer_word(o, 12), 1.0f);

    const Point3 point = *reinterpret_cast<const Point3*>(c + 16);
    const short angle = *reinterpret_cast<const short*>(c + 56);
    objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature(point, angle, float_bits(0x43a48000u), float_bits(0x42f00000u), &list, nullptr);

    unsigned char* node = static_cast<unsigned char*>(list.first);
    if (node != nullptr && bound > 0) {
        unsigned char* list_head = o + 24;
        unsigned char* particles = o + 28;
        int count = 0;
        do {
            unsigned char* target = pointer_word(node, 12);
            const bool flagged = (load_word(target, 12) & 0x01000000u) != 0;
            if (!flagged && isBoss__FP10GameObject(target) == 0 && static_cast<signed char>(target[172]) == 16 &&
                load_float(target, 876) <= 0.0f) {
                listOnewayLink__FP8ListHeadT0Pv(reinterpret_cast<ListHead*>(list_head), target, target);
                ++count;
                store_word(particles, 0, address_word(P_CreateParticleSystem__Fv()));
                particles += 4;
            }
            node = pointer_word(node, 0);
            if (node == nullptr) break;
        } while (count < bound);
    }
    listDestroy__FP8ListHead(&list);
}
