// Observed msg_draw__12ShockOfFrost at 0x002bdef0, 280 bytes. O is the object and P the point (three floats) at O+16.
// Steps in order:
//  1. A 64-byte local matrix is built with matMakeTransYaw__FP8Matrix34G6Point3s from a copy of P and the signed
//     halfword at O+56 minus 32767 (truncated to 16 bits).
//  2. matRot__FP8Matrix34is rotates it on axis 0 by the signed halfword at O+152.
//  3. lightSetAmbientColor__FPs is called with four halfwords: 7, 7, 42, 128.
//  4. The model is lumpFindResource__FPcT0("fx.lmp", "iceshard.vif") and the texture lumpFindResource__FPcT0("fx.lmp",
//     "iceshard.tex"), looked up in that order.
//  5. modelDraw runs with the model, the texture, mode 3, a second copy of P, the matrix, a null animation, all-ones
//     flags, a null draw record and a final 0.
//  6. lightResetOptionalParams__Fv is called last.
// Inferred: the argument roles from the mangled callee names. Unknown: what the halfword at O+152 and the ambient
// colour mean. Nothing here ran on the EE.
#include "../w12-actor-spell-c-common/access.hpp"

struct alignas(16) Matrix34 { float f[16]; };

extern "C" {
void matMakeTransYaw__FP8Matrix34G6Point3s(Matrix34* m, Point3 p, short yaw);
void matRot__FP8Matrix34is(Matrix34* m, int axis, short angle);
void lightSetAmbientColor__FPs(short* color);
void lightResetOptionalParams__Fv();
void* lumpFindResource__FPcT0(char* lump, char* name);
int modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, Point3 p, Matrix34* m, void* animation, unsigned long flags,
    void* drawRecord, int last);

void msg_draw__12ShockOfFrost(void* self) {
    using namespace w12c;
    unsigned char* const o = static_cast<unsigned char*>(self);
    Matrix34 m;
    Point3 first = *reinterpret_cast<Point3*>(o + 16);
    matMakeTransYaw__FP8Matrix34G6Point3s(&m, first, static_cast<short>(half(o, 56) - 32767));
    matRot__FP8Matrix34is(&m, 0, half(o, 152));
    short color[4];
    color[0] = 7;
    color[1] = 7;
    color[2] = 42;
    color[3] = 128;
    lightSetAmbientColor__FPs(color);
    void* const model = lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("iceshard.vif"));
    void* const texture = lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("iceshard.tex"));
    Point3 second = *reinterpret_cast<Point3*>(o + 16);
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
        model, texture, 3, second, &m, nullptr, static_cast<unsigned long>(-1), nullptr, 0);
    lightResetOptionalParams__Fv();
}
}
