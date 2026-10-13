// Observed __6PhPropG6Point3sP12_modelHeaderP14AnimationStatePcN25liPQ24item11descriptionf at
// 0x148d08, 864 bytes. Arguments: the object, a position P (by value), a short yaw Y, a model
// header, an animation state, three char pointers A, B, C, a long N, an int F (flags), an item
// description (may be null) and a float S. Returns nothing. Steps, in order:
//  1. The Prop constructor runs with (object, P, A, B, C, N, F | 4). Word +72 = _vt$6PhProp and the
//     description-area defaults are set as in the matrix-placement constructor (halfwords at
//     +364, +372, +380, +388 = -1, bytes +360/+361 = -1, eight halfwords at +344..+358 = 0).
//  2. Bit 0x4000 is set in the word at +12 and float +576 = S. A non-null description is copied (72
//     bytes) to +328, byte +568 = 1, bit 0x100000 set and 0x800 cleared in the word at +12.
//  3. Word +556 = -1, byte +570 = 0, word +564 = 0; when the 64-bit word at +232 is non-zero, word
//     +556 = the index of its highest set bit.
//  4. modelGetPrimaryBone(model, word +556) -> bone; animGetBoneMatrix(animation, bone, M) fills a
//     local matrix M. A second matrix T = matMakeTransYawScale(point (0,0,0), (short)(Y + 16384),
//     float +576) is built and M = M * T (matMul(M, M, T)).
//  5. The position at +16 becomes P + matTransform(position - P, M).
//  6. initStuff__6PhPropi(object, F) runs, then the flag-gated swish sound and bit 0x2000 in the word
//     at +12, exactly as in the matrix-placement constructor.
// Unknown: what the bone, the yaw offset of 16384 (a quarter turn if the angle is 16 bits) and the
// description area mean. Nothing here ran on the EE.
#include "../w11-ctor-a-common/phprop.hpp"

extern "C" {
int modelGetPrimaryBone__FP12_modelHeaderi(void* model, int index);
void animGetBoneMatrix__FP14AnimationStateiP8Matrix34(void* animation, int bone, Matrix34* out);
void matMakeTransYawScale__FP8Matrix34G6Point3sf(Matrix34* out, Point3 translation, short yaw, float scale);
void matMul__FP8Matrix34N20(Matrix34* out, Matrix34* a, Matrix34* b);
void __6PhPropG6Point3sP12_modelHeaderP14AnimationStatePcN25liPQ24item11descriptionf(void* self, Point3 position, short yaw, void* model, void* animation,
                                                                                    char* a, char* b, char* c, long n, int flags, void* description,
                                                                                    float scale);
}

extern "C" void __6PhPropG6Point3sP12_modelHeaderP14AnimationStatePcN25liPQ24item11descriptionf(void* self, Point3 position, short yaw, void* model, void* animation,
                                                                                               char* a, char* b, char* c, long n, int flags, void* description,
                                                                                               float scale) {
    using namespace w11;
    prop_construct(self, position, a, b, c, n, flags | 4);
    phprop_defaults(self);
    u32(self, 12) |= 0x4000u;
    f32(self, 576) = scale;
    phprop_description(self, description);
    phprop_layer(self);
    Matrix34 bone_matrix;
    Matrix34 turn;
    animGetBoneMatrix__FP14AnimationStateiP8Matrix34(animation, modelGetPrimaryBone__FP12_modelHeaderi(model, i32(self, 556)), &bone_matrix);
    matMakeTransYawScale__FP8Matrix34G6Point3sf(&turn, Point3{0.0f, 0.0f, 0.0f}, static_cast<short>(yaw + 16384), f32(self, 576));
    matMul__FP8Matrix34N20(&bone_matrix, &bone_matrix, &turn);
    phprop_place(self, position, &bone_matrix);
    initStuff__6PhPropi(self, flags);
    phprop_finish(self, flags);
}
