// Observed dropWeapon__5Mummy at 0x002398a0, 228 bytes. Let R be the block at the word object+36.
// 1. modelGetCharAttachmentPos(model = word +12 of R, copy of the point at +16, (halfword +56) - 16384
//    truncated to a short, animation state at +288, 0, point A, point B, 1.0) fills A and B.
// 2. blockAlloc(584). When it returns a block, the PhProp constructor runs on it with: A, the word
//    at R+0, the word at R+8*k, the word at R+8*k+8 (k is the word at object+964), the long 1, the
//    int 16641 (0x4101), a null description and 1.0.
// 3. The word at +964 is cleared, whether or not a block was obtained.
// Point B is filled but not read afterwards. What R's words are is unknown (they are used as three
// character-pointer arguments). Nothing here ran on the EE.
#include "../w10-actor-j-common/access.hpp"

extern "C" {
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
    void* model, Point3 position, short a, void* animation, int b, Point3* first, Point3* second, float f);
void* blockAlloc__Fi(int size);
void __6PhPropG6Point3PcN22liPQ24item11descriptionf(void* self, Point3 position, char* a, char* b, char* c, long n, int flags,
                                                    void* description, float f);

void dropWeapon__5Mummy(void* self) {
    using namespace w10j;
    unsigned char* const o = static_cast<unsigned char*>(self);
    Point3 first, second;
    unsigned char* const block = from_word(word(o, 36));
    modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
        from_word(word(block, 12)), *reinterpret_cast<Point3*>(o + 16), static_cast<short>(half(o, 56) - 16384), o + 288, 0,
        &first, &second, 1.0f);
    void* const prop = blockAlloc__Fi(584);
    if (prop != nullptr) {
        unsigned char* const entry = block + word(o, 964) * 8u;
        __6PhPropG6Point3PcN22liPQ24item11descriptionf(
            prop, first, reinterpret_cast<char*>(from_word(word(block, 0))), reinterpret_cast<char*>(from_word(word(entry, 0))),
            reinterpret_cast<char*>(from_word(word(entry, 8))), 1, 16641, nullptr, 1.0f);
    }
    word(o, 964) = 0;
}
}
