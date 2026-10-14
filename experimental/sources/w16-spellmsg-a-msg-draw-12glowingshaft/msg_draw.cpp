// Observed msg_draw__12GlowingShaft at 0x001e15b0, 868 bytes. O is the object; a is the signed word at +80, n the signed word at +76,
// (r, g, b) are the three halfwords at +84, +86 and +88 and P is the position (three singles) at +16. Pool constants with no symbols (bits):
// k1 = 0x3d088888 and k2 = 0x3d088888 (about 1/30, two entries with the same bits), c = 0x403dc28f (about 2.965), x = 0x3f918936 (about 1.137),
// y = 0x3f8a9fbe (about 1.083), d = 0x43b9d1a9 (about 371.64), e = 0x3e4ccccc (about 0.2), f = 0x3ecccccc (about 0.4). Steps in order:
//  1. A scale s and an ambient colour (three halfwords then a zero halfword) are chosen:
//       a - 30 < n:  s = ((float)a - (float)n) * k1 and the colour is (int)((float)r * s), (int)((float)g * s), (int)((float)b * s);
//       else n < 30: s = (float)n * k2 and the colour is scaled by s as above;
//       otherwise:   s = 1.0 and the colour is (r, g, b) unchanged.
//  2. lightSetAmbientColor__FPs is called with the four halfwords. The model is lumpFindResource__FPcT0("fx.lmp", "cylinder.vif") and the
//     texture lumpFindResource__FPcT0("fx.lmp", "cylinder.tex"), looked up in that order.
//  3. With S = (x, y, -(s + c)) and T = (P.x + 0.0, P.y, P.z + (S.z + d)), three draws of the model share a matrix M. Each builds M with
//     matMakeTrans__FP8Matrix34G6Point3(M, T), then matRot__FP8Matrix34is(M, 2, angle), then matScale__FP8Matrix34fff(M, ...), then calls
//     modelDrawEffect__FP12_modelHeaderP8_textureiP8Matrix34fP14AnimationStateUl(model, texture, 3, M, 2000.0, null, -1). The three
//     (angle, scale) pairs are ((short)(frameNm << 7), S), ((short)(-145 * frameNm), S + e on every component) and
//     ((short)(frameNm * 630 / 4), S + f on every component).
//  4. lightResetOptionalParams__Fv is called last.
// Inferred: the draw of a glowing shaft that fades in and out over its countdown, three spinning copies of the cylinder model of different
// size. Unknown: what the words at +76 and +80 and the three halfwords mean beyond their use here. Nothing here ran on the EE.
#include "../w12-actor-spell-c-common/access.hpp"

struct alignas(16) Matrix34 { float f[16]; };

extern "C" {
extern int frameNm;
void lightSetAmbientColor__FPs(short* color);
void lightResetOptionalParams__Fv();
void matMakeTrans__FP8Matrix34G6Point3(Matrix34* m, Point3 p);
void matRot__FP8Matrix34is(Matrix34* m, int axis, short angle);
void matScale__FP8Matrix34fff(Matrix34* m, float x, float y, float z);
void* lumpFindResource__FPcT0(char* lump, char* name);
void modelDrawEffect__FP12_modelHeaderP8_textureiP8Matrix34fP14AnimationStateUl(void* model, void* texture, int a, Matrix34* m, float f, void* animation, unsigned long flags);

void msg_draw__12GlowingShaft(void* self) {
    using namespace w12c;
    unsigned char* const o = static_cast<unsigned char*>(self);
    short color[4];
    float s = 1.0f;
    const int a = sword(o, 80);
    const int n = sword(o, 76);
    if (a - 30 < n) {
        s = (static_cast<float>(a) - static_cast<float>(n)) * bits(0x3d088888u);
        color[0] = static_cast<short>(static_cast<int>(static_cast<float>(half(o, 84)) * s));
        color[1] = static_cast<short>(static_cast<int>(static_cast<float>(half(o, 86)) * s));
        color[2] = static_cast<short>(static_cast<int>(static_cast<float>(half(o, 88)) * s));
    } else if (n < 30) {
        s = static_cast<float>(n) * bits(0x3d088888u);
        color[0] = static_cast<short>(static_cast<int>(static_cast<float>(half(o, 84)) * s));
        color[1] = static_cast<short>(static_cast<int>(static_cast<float>(half(o, 86)) * s));
        color[2] = static_cast<short>(static_cast<int>(static_cast<float>(half(o, 88)) * s));
    } else {
        color[0] = half(o, 84);
        color[1] = half(o, 86);
        color[2] = half(o, 88);
    }
    color[3] = 0;
    lightSetAmbientColor__FPs(color);
    void* const model = lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("cylinder.vif"));
    void* const texture = lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("cylinder.tex"));
    float zero = 0.0f;
    __asm__("" : "+f"(zero));
    const float sz = -(s + bits(0x403dc28fu));
    // The pool constants must be added at run time (the compiler would fold them with different rounding).
    float sx = bits(0x3f918936u);
    float sy = bits(0x3f8a9fbeu);
    __asm__("" : "+f"(sx), "+f"(sy));
    const float tz = sz + bits(0x43b9d1a9u);
    const Point3 at = {real(o, 16) + zero, real(o, 20), real(o, 24) + tz};
    Matrix34 m;
    matMakeTrans__FP8Matrix34G6Point3(&m, at);
    matRot__FP8Matrix34is(&m, 2, static_cast<short>(frameNm << 7));
    matScale__FP8Matrix34fff(&m, sx, sy, sz);
    modelDrawEffect__FP12_modelHeaderP8_textureiP8Matrix34fP14AnimationStateUl(model, texture, 3, &m, 2000.0f, nullptr, static_cast<unsigned long>(-1));
    matMakeTrans__FP8Matrix34G6Point3(&m, at);
    matRot__FP8Matrix34is(&m, 2, static_cast<short>(-145 * frameNm));
    matScale__FP8Matrix34fff(&m, sx + bits(0x3e4ccccc), sy + bits(0x3e4ccccc), sz + bits(0x3e4ccccc));
    modelDrawEffect__FP12_modelHeaderP8_textureiP8Matrix34fP14AnimationStateUl(model, texture, 3, &m, 2000.0f, nullptr, static_cast<unsigned long>(-1));
    matMakeTrans__FP8Matrix34G6Point3(&m, at);
    matRot__FP8Matrix34is(&m, 2, static_cast<short>(frameNm * 630 / 4));
    matScale__FP8Matrix34fff(&m, sx + bits(0x3ecccccc), sy + bits(0x3ecccccc), sz + bits(0x3ecccccc));
    modelDrawEffect__FP12_modelHeaderP8_textureiP8Matrix34fP14AnimationStateUl(model, texture, 3, &m, 2000.0f, nullptr, static_cast<unsigned long>(-1));
    lightResetOptionalParams__Fv();
}
}
