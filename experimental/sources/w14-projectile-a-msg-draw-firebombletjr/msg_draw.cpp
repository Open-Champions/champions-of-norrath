// Observed msg_draw__13FireBombletJr at 0x0028b468, 216 bytes. In order:
//  1. A four-halfword ambient offset (16, 16, 16, 0 in the image, read as 8 bytes from 0x004860d8 in .sdata)
//     is copied to the stack and passed to lightSetAmbientOffset; lightSetAmbientOnly runs.
//  2. A 48-byte matrix on the stack is set to identity, translated by the floats at object+16, +20, +24, then
//     rotated about axis 2 by the halfword at +56 and about axis 0 by the halfword at +90.
//  3. modelDraw runs with the model at word(word(object+36)+12), the texture at word(word(object+36)+20), the
//     int 0, a copy of the 12 bytes at object+16 as the position, that matrix, a null animation state, the
//     64-bit word at +104, a null draw record and the int 0.
//  4. lightResetOptionalParams runs.
// Unknown: what the rotated fields and the 64-bit word at +104 are. Nothing here ran on the EE.
#include "../w14-projectile-a-common/access.hpp"

extern "C" {
void lightSetAmbientOffset__FPs(short* offsets);
void lightSetAmbientOnly__Fv();
void matIdent__FP8Matrix34(void* matrix);
void matTrans__FP8Matrix34fff(void* matrix, float x, float y, float z);
void matRot__FP8Matrix34is(void* matrix, int axis, short angle);
void modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, Point3 position, void* matrix, void* animation, unsigned long long flags, void* record, int extra);
void lightResetOptionalParams__Fv();

void msg_draw__13FireBombletJr(void* self) {
    using namespace w14pa;
    unsigned char* const o = static_cast<unsigned char*>(self);
    short ambient[4] = {16, 16, 16, 0};
    lightSetAmbientOffset__FPs(ambient);
    lightSetAmbientOnly__Fv();
    float matrix[12];
    matIdent__FP8Matrix34(matrix);
    matTrans__FP8Matrix34fff(matrix, real(o, 16), real(o, 20), real(o, 24));
    matRot__FP8Matrix34is(matrix, 2, half(o, 56));
    matRot__FP8Matrix34is(matrix, 0, half(o, 90));
    unsigned char* const group = from_word(word(o, 36));
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
        from_word(word(group, 12)), from_word(word(group, 20)), 0, *reinterpret_cast<Point3*>(o + 16), matrix, nullptr, dword(o, 104),
        nullptr, 0);
    lightResetOptionalParams__Fv();
}
}
