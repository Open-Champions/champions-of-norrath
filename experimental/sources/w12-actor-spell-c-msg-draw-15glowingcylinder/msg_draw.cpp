// Observed msg_draw__15GlowingCylinder at 0x001d8c08, 840 bytes. O is the object; n is the signed word at +76,
// (r, g, b) are the three halfwords at +80, +82 and +84 and P is the position (three floats) at +16.
// Steps in order:
//  1. A scale s and an ambient colour (three halfwords then a zero halfword) are chosen from n:
//       n >= 61:       s = (70 - n) * k1 and the colour is (int)(float(r) * s), (int)(float(g) * s), (int)(float(b) * s);
//       10 <= n < 61:  s = 1.0 and the colour is (r, g, b) unchanged;
//       n < 10:        s = float(n) * k2 and the colour is scaled by s as in the first case.
//     (k1 and k2 are floats the image reads from its global pool at 0x004807dc and 0x004807e0; both store
//     0x3dcccccc, 0.1f; no symbol names them, so the bits are written here.)
//  2. lightSetAmbientColor__FPs is called with the four halfwords, then s is increased by 1.0.
//  3. Three draws of one model share a matrix M. The model is lumpFindResource__FPcT0("fx.lmp", "cylinder.vif"),
//     the texture lumpFindResource__FPcT0("fx.lmp", "cylinder.tex"). Each draw builds M with
//     matMakeTrans(M, P), then matRot(M, 2, angle), then matScale(M, a, a, s), then calls
//     modelDrawEffect(model, texture, 3, M, 2000.0, null, -1). The three (angle, a) pairs are
//       (short)(frameNm << 7), 0.8   -- the constant at 0x004807e4 (bits 0x3f4ccccc);
//       (short)(-145 * frameNm), 1.0;
//       (short)(frameNm * 630 / 4), 1.2   -- the constant at 0x004807e8 (bits 0x3f999999).
//  4. lightResetOptionalParams__Fv is called last.
// The lump lookups happen after the matrix for the first draw is built and before its draw. Unknown: what the
// three halfwords and n mean (a fading glow colour and a countdown are guesses from use), and the true size of
// Matrix34 (64 bytes here from the stack frame, only used through the matrix calls). Nothing here ran on the EE.
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

void msg_draw__15GlowingCylinder(void* self) {
    using namespace w12c;
    unsigned char* const o = static_cast<unsigned char*>(self);
    short color[4];
    float scale = 1.0f;
    const int n = sword(o, 76);
    if (n >= 61) {
        scale = static_cast<float>(70 - n) * bits(0x3dccccccu);
        color[0] = static_cast<short>(static_cast<int>(static_cast<float>(half(o, 80)) * scale));
        color[1] = static_cast<short>(static_cast<int>(static_cast<float>(half(o, 82)) * scale));
        color[2] = static_cast<short>(static_cast<int>(static_cast<float>(half(o, 84)) * scale));
    } else if (n < 10) {
        scale = static_cast<float>(n) * bits(0x3dccccccu);
        color[0] = static_cast<short>(static_cast<int>(static_cast<float>(half(o, 80)) * scale));
        color[1] = static_cast<short>(static_cast<int>(static_cast<float>(half(o, 82)) * scale));
        color[2] = static_cast<short>(static_cast<int>(static_cast<float>(half(o, 84)) * scale));
    } else {
        color[0] = half(o, 80);
        color[1] = half(o, 82);
        color[2] = half(o, 84);
    }
    color[3] = 0;
    lightSetAmbientColor__FPs(color);
    scale = scale + 1.0f;
    Matrix34 m;
    matMakeTrans__FP8Matrix34G6Point3(&m, *reinterpret_cast<Point3*>(o + 16));
    matRot__FP8Matrix34is(&m, 2, static_cast<short>(frameNm << 7));
    matScale__FP8Matrix34fff(&m, bits(0x3f4cccccu), bits(0x3f4cccccu), scale);
    void* const model = lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("cylinder.vif"));
    void* const texture = lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("cylinder.tex"));
    modelDrawEffect__FP12_modelHeaderP8_textureiP8Matrix34fP14AnimationStateUl(model, texture, 3, &m, 2000.0f, nullptr, static_cast<unsigned long>(-1));
    matMakeTrans__FP8Matrix34G6Point3(&m, *reinterpret_cast<Point3*>(o + 16));
    matRot__FP8Matrix34is(&m, 2, static_cast<short>(-145 * frameNm));
    matScale__FP8Matrix34fff(&m, 1.0f, 1.0f, scale);
    modelDrawEffect__FP12_modelHeaderP8_textureiP8Matrix34fP14AnimationStateUl(model, texture, 3, &m, 2000.0f, nullptr, static_cast<unsigned long>(-1));
    matMakeTrans__FP8Matrix34G6Point3(&m, *reinterpret_cast<Point3*>(o + 16));
    matRot__FP8Matrix34is(&m, 2, static_cast<short>(frameNm * 630 / 4));
    matScale__FP8Matrix34fff(&m, bits(0x3f999999u), bits(0x3f999999u), scale);
    modelDrawEffect__FP12_modelHeaderP8_textureiP8Matrix34fP14AnimationStateUl(model, texture, 3, &m, 2000.0f, nullptr, static_cast<unsigned long>(-1));
    lightResetOptionalParams__Fv();
}
}
