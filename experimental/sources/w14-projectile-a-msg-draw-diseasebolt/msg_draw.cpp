// Provenance: the body is the reviewed w13-spell-msg-a unit w13-spell-msg-a-msg-draw-11diseasebolt, re-verified in this package against the original with a fresh paired run and given a host
// contract (the earlier unit has none and was left out of the plan). Select one of the two units, not both.
// Observed msg_draw__11DiseaseBolt at 0x00293628, 268 bytes. O is the object and P the point (three floats) at O+16.
// Steps in order:
//  1. A 64-byte local matrix is built with matMakeTransYawScale__FP8Matrix34G6Point3sf from a copy of P, the signed
//     halfword at O+56 plus 16612 (truncated to 16 bits) and the scale 1.0.
//  2. lightSetAmbientColor__FPs is called with four halfwords: 7, 17, 7, 0.
//  3. The model is lumpFindResource__FPcT0("fx.lmp", "skull.vif") and the texture lumpFindResource__FPcT0("fx.lmp",
//     "skull.tex"), looked up in that order.
//  4. modelDraw runs with the model, the texture, mode 3, a second copy of P, the matrix, a null animation, all-ones
//     flags, a null draw record and a final 0.
//  5. lightResetOptionalParams__Fv is called last.
// Inferred: the argument roles from the mangled callee names. Unknown: what the ambient colour and the 16612 turn
// mean. Nothing here ran on the EE.
#include "../w12-actor-spell-c-common/access.hpp"

struct alignas(16) Matrix34 { float f[16]; };

extern "C" {
void matMakeTransYawScale__FP8Matrix34G6Point3sf(Matrix34* m, Point3 p, short yaw, float scale);
void lightSetAmbientColor__FPs(short* color);
void lightResetOptionalParams__Fv();
void* lumpFindResource__FPcT0(char* lump, char* name);
int modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, Point3 p, Matrix34* m, void* animation, unsigned long flags,
    void* drawRecord, int last);

void msg_draw__11DiseaseBolt(void* self) {
    using namespace w12c;
    unsigned char* const o = static_cast<unsigned char*>(self);
    Matrix34 m;
    Point3 first = *reinterpret_cast<Point3*>(o + 16);
    matMakeTransYawScale__FP8Matrix34G6Point3sf(&m, first, static_cast<short>(half(o, 56) + 16612), 1.0f);
    short color[4];
    color[0] = 7;
    color[1] = 17;
    color[2] = 7;
    color[3] = 0;
    lightSetAmbientColor__FPs(color);
    void* const model = lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("skull.vif"));
    void* const texture = lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("skull.tex"));
    Point3 second = *reinterpret_cast<Point3*>(o + 16);
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
        model, texture, 3, second, &m, nullptr, static_cast<unsigned long>(-1), nullptr, 0);
    lightResetOptionalParams__Fv();
}
}
