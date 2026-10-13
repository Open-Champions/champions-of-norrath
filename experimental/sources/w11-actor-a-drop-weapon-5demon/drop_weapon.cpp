// Observed dropWeapon__5Demon at 0x001eb258, 236 bytes. Let R be the block at the word object+36 and k the
// word at object+960.
// 1. When k is non-zero:
//    a. modelGetCharAttachmentPos(model = word +12 of R, copy of the point at +16, (halfword +56) - 16385
//       truncated to a short, animation state at +288, 0, point A, point B, 1.0) fills A and B.
//    b. blockAlloc(584). When it returns a block, the PhProp constructor runs on it with: A, the word at
//       R+0, the word at R+8*k, the word at R+8*k+8, the long 1, the int 16385 (0x4001), a null description
//       and 1.0.
// 2. The word at +960 is cleared in every case, including k == 0.
// Point B is filled but not read afterwards. What R's words are is unknown (they are used as three
// character-pointer arguments). The routine has the same shape as the Mummy one, which has no k guard,
// reads +964 and passes 16641. Nothing here ran on the EE.
#include "../w11-actor-a-common/access.hpp"

extern "C" {
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
    void* model, Point3 position, short a, void* animation, int b, Point3* first, Point3* second, float f);
void* blockAlloc__Fi(int size);
void __6PhPropG6Point3PcN22liPQ24item11descriptionf(void* self, Point3 position, char* a, char* b, char* c, long n, int flags,
                                                    void* description, float f);

void dropWeapon__5Demon(void* self) {
    using namespace w11a;
    unsigned char* const o = static_cast<unsigned char*>(self);
    if (word(o, 960) != 0) {
        Point3 first, second;
        unsigned char* const block = from_word(word(o, 36));
        modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
            from_word(word(block, 12)), *reinterpret_cast<Point3*>(o + 16), static_cast<short>(half(o, 56) - 16385), o + 288, 0,
            &first, &second, 1.0f);
        void* const prop = blockAlloc__Fi(584);
        if (prop != nullptr) {
            unsigned char* const entry = block + word(o, 960) * 8u;
            __6PhPropG6Point3PcN22liPQ24item11descriptionf(
                prop, first, reinterpret_cast<char*>(from_word(word(block, 0))), reinterpret_cast<char*>(from_word(word(entry, 0))),
                reinterpret_cast<char*>(from_word(word(entry, 8))), 1, 16385, nullptr, 1.0f);
        }
    }
    word(o, 960) = 0;
}
}
