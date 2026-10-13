// Observed msg_draw__13MissileWeapon at 0x00146888, 1228 bytes. Argument: the object o. Nothing is returned.
// Names: pos = floats at o+16..o+24; X = word at +292 (an object), Y = word at +296; M = a 48-byte matrix on the
// stack. Float constants read through the global pointer: K1 = 0.08 (0x3da3d70a at 0x0047fd28), K2 = 0.01
// (0x3c23d70a at 0x0047fd2c) and K3 = 0.08 (0x3da3d70a at 0x0047fd30); none has a symbol. The image also keeps
// two small data tables that are copied to the stack: four halfwords (32, 32, 32, 0) at 0x00483b78 and four
// ints (255, 100, 50, 128) at 0x00462180; they are written out as literals here.
//  1. matMakeTransYawScale(M, pos, halfword at +56, 1.0); the byte modelClippingOn is set to 1. When the halfword
//     at +250 is nonzero, matRot(M, signed byte at +254, halfword at +252). modelSetHalo(null, 0).
//  2. Ambient offsets (32, 32, 32, 0) are used, except (40, 40, 40, 60) when bit 0x100 of levelFlags is set;
//     lightSetAmbientOffset runs on them.
//  3. When X is nonzero: T = the word at X+12, and model = the table entry at offset 128 of T's table (T+72)
//     called on T. When model is non-null: modelGetShadowPos(model, T+288, copy of T's position at T+16, halfword
//     at T+56, the word at o+244, &point, null, &R, float at T+224) fills a 36-byte matrix R and a Point3; then
//     matMul(R, R, o+256), matMakeTransYaw(M, point, 0), matMul(M, R, M), matTrans(M, 0, -24, 0) and
//     matScale(M, 3, 1, 3) are run.
//  4. When X is zero, Y is nonzero and the word at Y+0 is nonzero, fxRunWeapon(Y, word at +444, o+300, copy of
//     pos, halfword at +56, null, null, f, n, 0, 1.0, &M) is called, with f = 0.0 and n = 0 (and the byte at +248
//     cleared) when the byte at +248 is nonzero, else f = 1.0 and n = 1.
//  5. modelDraw(word at +444, word at +448, 1, copy of pos, &M, null, all ones as a 64-bit mask, null, 0);
//     lightResetOptionalParams; modelResetOptionalParams.
//  6. Nothing more when X is nonzero. Otherwise, when the word at +208 (kind) is 2, 5 or 6, ten halo sprites
//     are drawn. The colour (a0..a3) is (255, 100, 50, 128) for kind 2 and (50, 100, 255, 128) for kinds 5 and 6.
//     (The image also zeroes the fourth colour when kind is 0 or 3; that cannot happen here.) D is the floats at
//     +428, +432, +436. For i = 0 .. 9: the point is pos + D * (K3 - i * K2); alpha is the fourth colour value
//     limited to 128; P_FaceSprite(point, 5.0, (float)(5000 - 200 i) + isin(frameNm << 10) * 250.0, colours 0..2,
//     alpha, haloTexture1, a short taken from the low 7 bits of frameNm shifted up by 9, 3).
// Inferred: this is the draw handler of MissileWeapon, from its symbol; the matrix chain places the model on a
// shadow point of the owner T. Unknown: what X, Y, kind and the fields at +244, +248, +250..254, +428..436 are.
// Nothing here ran on the EE.
#include "../w11-actor-a-common/access.hpp"

extern "C" {
extern signed char modelClippingOn;
extern int levelFlags;
extern int frameNm;
extern void* haloTexture1;
void matMakeTransYawScale__FP8Matrix34G6Point3sf(void* matrix, Point3 position, short yaw, float scale);
void matRot__FP8Matrix34is(void* matrix, int axis, short angle);
void modelSetHalo__FP7HaloDefi(void* halo, int a);
void lightSetAmbientOffset__FPs(short* offsets);
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
    void* model, void* animation, Point3 position, short heading, int index, Point3* outPoint, float* outValue, void* matrix, float scale);
void matMul__FP8Matrix33N20(void* destination, void* a, void* b);
void matMakeTransYaw__FP8Matrix34G6Point3s(void* matrix, Point3 position, short yaw);
void matMul__FP8Matrix34P8Matrix33T0(void* destination, void* rotation, void* matrix);
void matTrans__FP8Matrix34fff(void* matrix, float x, float y, float z);
void matScale__FP8Matrix34fff(void* matrix, float x, float y, float z);
void fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
    void* description, void* model, void* state, Point3 position, short heading, void* second, void* animation, float a, int b, int c, float d,
    void* matrix);
void modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, Point3 position, void* matrix, void* animation, unsigned long long flags, void* record, int extra);
void lightResetOptionalParams__Fv();
void modelResetOptionalParams__Fv();
float isin__Fi(int angle);
void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(Point3 position, float a, float b, int c0, int c1, int c2, int alpha, void* texture, short angle, int mode);

void msg_draw__13MissileWeapon(void* self) {
    using namespace w11a;
    unsigned char* const o = static_cast<unsigned char*>(self);
    float matrix[12];
    matMakeTransYawScale__FP8Matrix34G6Point3sf(matrix, *reinterpret_cast<Point3*>(o + 16), half(o, 56), 1.0f);
    modelClippingOn = 1;
    if (half(o, 250) != 0) matRot__FP8Matrix34is(matrix, *reinterpret_cast<signed char*>(o + 254), half(o, 252));
    modelSetHalo__FP7HaloDefi(nullptr, 0);
    short ambient[4] = {32, 32, 32, 0};
    if (levelFlags & 0x100) {
        ambient[0] = 40;
        ambient[1] = 40;
        ambient[2] = 40;
        ambient[3] = 60;
    }
    lightSetAmbientOffset__FPs(ambient);
    if (word(o, 292) != 0) {
        unsigned char* const t = from_word(word(from_word(word(o, 292)), 12));
        void* const model = reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(call_entry(t, 128)));
        if (model != nullptr) {
            float rotation[9];
            Point3 point;
            modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
                model, t + 288, *reinterpret_cast<Point3*>(t + 16), half(t, 56), sword(o, 244), &point, nullptr, rotation, real(t, 224));
            matMul__FP8Matrix33N20(rotation, rotation, o + 256);
            matMakeTransYaw__FP8Matrix34G6Point3s(matrix, point, 0);
            matMul__FP8Matrix34P8Matrix33T0(matrix, rotation, matrix);
            matTrans__FP8Matrix34fff(matrix, 0.0f, bits(0xc1c00000u), 0.0f);
            matScale__FP8Matrix34fff(matrix, bits(0x40400000u), 1.0f, bits(0x40400000u));
        }
    }
    if (word(o, 292) == 0 && word(o, 296) != 0 && word(from_word(word(o, 296)), 0) != 0) {
        void* const description = from_word(word(o, 296));
        if (o[248] != 0) {
            o[248] = 0;
            fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
                description, from_word(word(o, 444)), o + 300, *reinterpret_cast<Point3*>(o + 16), half(o, 56), nullptr, nullptr, 0.0f, 0, 0, 1.0f,
                matrix);
        } else {
            fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
                description, from_word(word(o, 444)), o + 300, *reinterpret_cast<Point3*>(o + 16), half(o, 56), nullptr, nullptr, 1.0f, 1, 0, 1.0f,
                matrix);
        }
    }
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
        from_word(word(o, 444)), from_word(word(o, 448)), 1, *reinterpret_cast<Point3*>(o + 16), matrix, nullptr, ~0ull, nullptr, 0);
    lightResetOptionalParams__Fv();
    modelResetOptionalParams__Fv();
    if (word(o, 292) != 0) return;
    const int kind = sword(o, 208);
    if (kind != 2 && kind != 5 && kind != 6) return;
    int colour[4] = {255, 100, 50, 128};
    if (kind != 2) {
        colour[0] = 50;
        colour[1] = 100;
        colour[2] = 255;
        colour[3] = 128;
    }
    for (int i = 0; i < 10; ++i) {
        const float scale = bits(0x3da3d70au) - static_cast<float>(i) * bits(0x3c23d70au);
        Point3 point;
        point.x = real(o, 16) + real(o, 428) * scale;
        point.y = real(o, 20) + real(o, 432) * scale;
        point.z = real(o, 24) + real(o, 436) * scale;
        const int alpha = colour[3] < 129 ? colour[3] : 128;
        const float wobble = isin__Fi(frameNm << 10);
        P_FaceSprite__FG6Point3ffiiiiP8_texturesi(
            point, 5.0f, static_cast<float>(5000 - 200 * i) + wobble * bits(0x437a0000u), colour[0], colour[1], colour[2], alpha, haloTexture1,
            static_cast<short>(static_cast<int>(static_cast<unsigned>(static_cast<unsigned short>(frameNm)) << 25) >> 16), 3);
    }
}
}
