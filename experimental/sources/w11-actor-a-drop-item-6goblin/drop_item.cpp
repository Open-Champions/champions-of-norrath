// Observed dropItem__6Goblini at 0x0022e798, 312 bytes. Arguments: the object and an int kind. Nothing is
// returned. Let R be the data object goblinWeaponResourceMap (0x0042cfc8), an array of 8-byte entries whose
// first word is a string address.
//  1. kind 1: n is the word at object+988 and flags start as 257; the word is cleared.
//     kind 2: n is the word at object+992, flags start as 1 and a marker m is 1; the word is cleared.
//     Any other kind: n is 0. In every case m is 0 unless kind is 2.
//  2. Nothing more happens when n is zero.
//  3. Otherwise flags become flags | 0x2000 when n is 3 and flags | 0x4000 for any other n.
//     modelGetCharAttachmentPos(model = word +12 of the block named by the word at +36, a copy of the position at
//     object+16, (halfword at +56) - 16385 truncated to a short, the animation state at +288, m, first, second,
//     1.0) is called; first starts as a copy of the position and second is not initialised here.
//  4. blockAlloc(584). When it returns a block, the PhProp constructor (the one taking three names) runs on it
//     with first, R[0], R[n], R[n+7], the long 1, the flags, a null description and 1.0.
// Inferred: PhProp is a physics prop and the drop is an item or weapon from the symbol and table names. Unknown:
// what the words at +988 and +992 are, what n values other than 3 mean, and what the flag bits are.
// Nothing here ran on the EE.
#include "../w11-actor-a-common/access.hpp"

extern "C" {
extern unsigned char goblinWeaponResourceMap[];
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
    void* model, Point3 position, short a, void* animation, int b, Point3* first, Point3* second, float f);
void* blockAlloc__Fi(int size);
void __6PhPropG6Point3PcN22liPQ24item11descriptionf(void* self, Point3 position, char* a, char* b, char* c, long n, int flags,
                                                    void* description, float f);

void dropItem__6Goblini(void* self, int kind) {
    using namespace w11a;
    unsigned char* const o = static_cast<unsigned char*>(self);
    int n = 0;
    int flags = 1;
    int marker = 0;
    if (kind == 1) {
        n = sword(o, 988);
        flags = 257;
        word(o, 988) = 0;
    } else if (kind == 2) {
        n = sword(o, 992);
        marker = 1;
        word(o, 992) = 0;
    }
    flags |= n == 3 ? 0x2000 : 0x4000;
    if (n == 0) return;
    Point3 first = *reinterpret_cast<Point3*>(o + 16);
    Point3 second;
    modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
        from_word(word(from_word(word(o, 36)), 12)), *reinterpret_cast<Point3*>(o + 16),
        static_cast<short>(static_cast<unsigned short>(half(o, 56)) - 16385), o + 288, marker, &first, &second, 1.0f);
    void* const prop = blockAlloc__Fi(584);
    if (prop != nullptr) {
        unsigned char* const entry = goblinWeaponResourceMap + static_cast<unsigned>(n) * 8u;
        __6PhPropG6Point3PcN22liPQ24item11descriptionf(
            prop, first, reinterpret_cast<char*>(from_word(word(goblinWeaponResourceMap, 0))), reinterpret_cast<char*>(from_word(word(entry, 0))),
            reinterpret_cast<char*>(from_word(word(entry, 56))), 1, flags, nullptr, 1.0f);
    }
}
}
