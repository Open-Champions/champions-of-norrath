// Observed __9SpellBashiP8CreatureP5Skill at 0x002a52c0, 384 bytes in .text. Arguments: the object, the rank, the creature
// and the skill. Returns nothing. The sync random state is advanced as state = state * 0x19660d + 0x3c6ef35f (32-bit).
// Steps, in order:
//  1. Byte +0 = the low byte of the rank. Word +20 = _vt$5Spell. Word +8 = skill, word +4 = 0, word +12 = creature.
//  2. The sync state becomes the new value. Word +20 = _vt$9SpellBash and the word at creature+1400 = 6.
//     Word +16 = the new value shifted right by 16.
//  3. anm = lumpFindResource(lump, "bash.anm"), where lump is the first word of the object whose pointer is at creature+36.
//  4. animAddOneShot(creature+288, anm, c, c, 2066) where c is the image float 0x3dcccccc (about 0.1). Then byte anm+32 = 2
//     and float anm+28 = 0.592f (image float 0x3f178d60).
//  5. Words +28, +24, +32 and +36 of the object = 0. A local list head's first word = 0.
//  6. A local point = the creature's 12 bytes at +16. objectGetSortedCreatureList(point, the halfword at creature+56,
//     100.0f, 90.0f, the list, null) fills the list. The two floats are 0x42c80000 and 0x42b40000.
//  7. Let first be the list's first node. Word +36 = the first node's target (its word at +12) when that target's word at
//     +12 has bit 0x01000000 clear, or when some later node's target has that bit clear. Otherwise +36 is left alone.
//  8. listDestroy(the list).
// Inferred: a bash spell that animates the creature and records a target from a nearby list. Unknown: the meanings of
// the fields and the lump name source, the constants' roles, and why step 7 stores the first node's target rather than
// the node that passed the test. Nothing here ran on the EE; the private paired run is described in the notes.
struct Point3 { float x, y, z; };
struct ListHead { void* first; unsigned unknown[3]; };

extern "C" {
extern unsigned char vt_Spell[] asm("_vt$5Spell");
extern unsigned char vt_SpellBash[] asm("_vt$9SpellBash");
extern unsigned syncRandState;
void* lumpFindResource__FPcT0(char* lump, char* name);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float first, float second, int id);
void objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature(Point3 point, short angle, float first, float second, ListHead* list, void* exclude);
void listDestroy__FP8ListHead(ListHead* list);
}

namespace {
static char bash_name[] = "bash.anm";
const unsigned target_flag = 0x01000000u;

inline unsigned address_word(const void* p) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
inline unsigned load_word(const unsigned char* base, unsigned offset) {
    return *reinterpret_cast<const unsigned*>(base + offset);
}
inline unsigned char* pointer_word(const unsigned char* base, unsigned offset) {
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(load_word(base, offset)));
}
inline void store_word(unsigned char* base, unsigned offset, unsigned value) {
    *reinterpret_cast<unsigned*>(base + offset) = value;
}
inline void store_float(unsigned char* base, unsigned offset, float value) {
    *reinterpret_cast<float*>(base + offset) = value;
}
inline float float_bits(unsigned bits) {
    union { unsigned u; float f; } value;
    value.u = bits;
    return value.f;
}
// True when the target object's word at +12 has the flag bit clear.
inline bool target_clear(const unsigned char* node) {
    return (load_word(pointer_word(node, 12), 12) & target_flag) == 0;
}
}

extern "C" void __9SpellBashiP8CreatureP5Skill(void* self, int rank, void* creature, void* skill) {
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
    store_word(o, 20, address_word(vt_SpellBash));
    store_word(c, 1400, 6);
    store_word(o, 16, state >> 16);
    char* lump = reinterpret_cast<char*>(static_cast<__UINTPTR_TYPE__>(load_word(pointer_word(c, 36), 0)));
    unsigned char* anm = static_cast<unsigned char*>(lumpFindResource__FPcT0(lump, bash_name));
    animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(c + 288, anm, float_bits(0x3dccccccu), float_bits(0x3dccccccu), 2066);
    anm[32] = 2;
    store_float(anm, 28, float_bits(0x3f178d60u));
    store_word(o, 28, 0);
    store_word(o, 24, 0);
    store_word(o, 32, 0);
    store_word(o, 36, 0);

    ListHead list;
    list.first = nullptr;
    const Point3 point = *reinterpret_cast<const Point3*>(c + 16);
    const short angle = *reinterpret_cast<const short*>(c + 56);
    objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature(point, angle, float_bits(0x42c80000u), float_bits(0x42b40000u), &list, nullptr);

    unsigned char* first = static_cast<unsigned char*>(list.first);
    if (first != nullptr) {
        bool store = target_clear(first);
        for (unsigned char* node = pointer_word(first, 0); node != nullptr && !store; node = pointer_word(node, 0)) {
            if (target_clear(node)) store = true;
        }
        if (store) store_word(o, 36, load_word(first, 12));
    }
    listDestroy__FP8ListHead(&list);
}
