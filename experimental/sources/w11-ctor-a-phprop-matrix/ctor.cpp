// Observed __6PhPropG6Point3P8Matrix34PcN23liPQ24item11descriptionf at 0x1489c0, 836 bytes.
// Arguments: the object, a position P (by value), a Matrix34 M, three char pointers A, B, C, a
// long N, an int F (flags), an item description (may be null) and a float S. Returns nothing.
// Steps, in order:
//  1. The Prop constructor runs with (object, P, A, B, C, N, F | 4). Word +72 = _vt$6PhProp.
//     Halfwords at +364, +372, +380, +388 = -1, bytes +360/+361 = -1 and eight halfwords at +344..
//     +358 = 0 (defaults for the description area). Float +576 = S.
//  2. The 48 bytes of M are copied to +272 and bit 0x4000 is set in the word at +12.
//  3. A non-null description is copied (72 bytes) to +328, byte +568 = 1, bit 0x100000 set and
//     0x800 cleared in the word at +12.
//  4. Word +556 = -1, byte +570 = 0, word +564 = 0; when the 64-bit word at +232 is non-zero,
//     word +556 = the index of its highest set bit.
//  5. The position at +16 becomes P + matTransform(position - P, matrix at +272).
//  6. initStuff__6PhPropi(object, F) runs.
//  7. When byte +568 is non-zero and F has any of the bits 0x19, word +68 steps as an LCG (times
//     0x19660d plus 0x3c6ef35f) and the sound "game_swish_chest<n>.anm", n = (word +68 >> 16)
//     % 3 + 1, from "fx.lmp" plays on the object at volume 1.0.
//  8. Bit 0x2000 is set in the word at +12.
// Unknown: what the description area, the matrix and the words at +232, +556, +564 mean. Nothing
// here ran on the EE.
#include "../w11-ctor-a-common/phprop.hpp"

extern "C" void __6PhPropG6Point3P8Matrix34PcN23liPQ24item11descriptionf(void* self, Point3 position, Matrix34* matrix, char* a, char* b, char* c,
                                                                        long n, int flags, void* description, float scale);

extern "C" void __6PhPropG6Point3P8Matrix34PcN23liPQ24item11descriptionf(void* self, Point3 position, Matrix34* matrix, char* a, char* b, char* c,
                                                                        long n, int flags, void* description, float scale) {
    using namespace w11;
    prop_construct(self, position, a, b, c, n, flags | 4);
    phprop_defaults(self);
    f32(self, 576) = scale;
    *reinterpret_cast<Matrix34*>(static_cast<unsigned char*>(self) + 272) = *matrix;
    u32(self, 12) |= 0x4000u;
    phprop_description(self, description);
    phprop_layer(self);
    phprop_place(self, position, reinterpret_cast<Matrix34*>(static_cast<unsigned char*>(self) + 272));
    initStuff__6PhPropi(self, flags);
    phprop_finish(self, flags);
}
