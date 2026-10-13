// Observed msg_draw__5Wheel at 0x002772f8, 324 bytes. O is the object. Names: B = the block at word O+396, i = the signed
// word at O+384, f = the float at B + 12i + 1236. Steps in order:
//  1. A local 48-byte matrix M (twelve floats, three per row, the last row the translation) is filled: row 0 is the three
//     floats at O+400, O+404, O+408 when 0.0 < f and their negations otherwise; rows 1 and 2 are the six floats at
//     O+412 .. O+432; row 3 is the position O+16, O+20, O+24.
//  2. matRot__FP8Matrix34is(M, 0, (short)(int)(float(O+368) * 0.0055555552 * 32767.0)), the two factors being the floats
//     0x3bb60b60 and 0x46fffe00 read through the global pointer (about 1/180 and 32767).
//  3. modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi is called with the model
//     (word at +76), the texture (word at +80), mode 1, the position, M, no animation state, the 64-bit value at +352
//     as the flags, no draw record and a final 0; then modelResetOptionalParams__Fv.
// Inferred: an angle of 0..180 mapped to a 16-bit rotation about axis 0 (the numbers only). Unknown: what the rows are
// (the name Matrix34 is that of the image's declaration) and what the float f selects. Nothing here ran on the EE.
#include "../w13-spell-msg-b-common/access.hpp"

struct Matrix34 { float m[12]; };

extern "C" {
void matRot__FP8Matrix34is(Matrix34* matrix, int axis, short angle);
void modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, Point3 position, Matrix34* matrix, void* animation,
    unsigned long long flags, void* record, int extra);
void modelResetOptionalParams__Fv();

void msg_draw__5Wheel(void* self) {
    using namespace w13smb;
    unsigned char* const o = static_cast<unsigned char*>(self);
    unsigned char* const block = from_word(word(o, 396));
    const float f = real(block + sword(o, 384) * 12, 1236);
    Matrix34 matrix;
    if (0.0f < f) {
        matrix.m[0] = real(o, 400);
        matrix.m[1] = real(o, 404);
        matrix.m[2] = real(o, 408);
    } else {
        matrix.m[0] = -real(o, 400);
        matrix.m[1] = -real(o, 404);
        matrix.m[2] = -real(o, 408);
    }
    matrix.m[3] = real(o, 412);
    matrix.m[4] = real(o, 416);
    matrix.m[5] = real(o, 420);
    matrix.m[6] = real(o, 424);
    matrix.m[7] = real(o, 428);
    matrix.m[8] = real(o, 432);
    matrix.m[9] = real(o, 16);
    matrix.m[10] = real(o, 20);
    matrix.m[11] = real(o, 24);
    matRot__FP8Matrix34is(&matrix, 0, static_cast<short>(static_cast<int>(real(o, 368) * bits(0x3bb60b60u) * bits(0x46fffe00u))));
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
        from_word(word(o, 76)), from_word(word(o, 80)), 1, *reinterpret_cast<Point3*>(o + 16), &matrix, nullptr, dword(o, 352), nullptr, 0);
    modelResetOptionalParams__Fv();
}
}
