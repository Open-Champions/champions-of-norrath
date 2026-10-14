// Observed msg_draw__18FrostStormIceShard at 0x001e1918, 292 bytes. O is the object. Nothing happens when the word at O+76 is
// nonzero. Otherwise, in order:
//  1. lightSetAmbientOffset__FPs is called with four halfwords: 7, 7, 43, 128 (the image copies them from a constant table).
//  2. A 64-byte local matrix is made identity (matIdent__FP8Matrix34), translated by the singles at O+16, +20, +24
//     (matTrans__FP8Matrix34fff), rotated on axis 1 by -16383 (matRot__FP8Matrix34is) and scaled by (h + h, h, h) with
//     h = (the single at O+80) * 0.5 (matScale__FP8Matrix34fff).
//  3. The model is lumpFindResource__FPcT0("fx.lmp", "iceshard.vif") and the texture lumpFindResource__FPcT0("fx.lmp",
//     "iceshard.tex"), looked up in that order.
//  4. modelDraw runs with the model, the texture, mode 3, a copy of the point at O+16, the matrix, a null animation, all-ones
//     flags, a null draw record and a final 0.
//  5. lightResetOptionalParams__Fv is called last.
// Inferred: the argument roles from the mangled callee names; a stretched ice-shard model at the object's position. Unknown: what
// the word at O+76 and the single at O+80 mean. Nothing here ran on the EE.
#include "../w12-actor-spell-c-common/access.hpp"

struct alignas(16) Matrix34 { float f[16]; };

extern "C" {
void matIdent__FP8Matrix34(Matrix34* m);
void matTrans__FP8Matrix34fff(Matrix34* m, float x, float y, float z);
void matRot__FP8Matrix34is(Matrix34* m, int axis, short angle);
void matScale__FP8Matrix34fff(Matrix34* m, float x, float y, float z);
void lightSetAmbientOffset__FPs(short* offset);
void lightResetOptionalParams__Fv();
void* lumpFindResource__FPcT0(char* lump, char* name);
int modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, Point3 p, Matrix34* m, void* animation, unsigned long flags,
    void* drawRecord, int last);

void msg_draw__18FrostStormIceShard(void* self) {
    using namespace w12c;
    unsigned char* const o = static_cast<unsigned char*>(self);
    if (word(o, 76) != 0u) return;
    short offset[4];
    offset[0] = 7;
    offset[1] = 7;
    offset[2] = 43;
    offset[3] = 128;
    lightSetAmbientOffset__FPs(offset);
    Matrix34 m;
    matIdent__FP8Matrix34(&m);
    matTrans__FP8Matrix34fff(&m, real(o, 16), real(o, 20), real(o, 24));
    matRot__FP8Matrix34is(&m, 1, -16383);
    const float h = real(o, 80) * 0.5f;
    matScale__FP8Matrix34fff(&m, h + h, h, h);
    void* const model = lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("iceshard.vif"));
    void* const texture = lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("iceshard.tex"));
    Point3 at = *reinterpret_cast<Point3*>(o + 16);
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
        model, texture, 3, at, &m, nullptr, static_cast<unsigned long>(-1), nullptr, 0);
    lightResetOptionalParams__Fv();
}
}
