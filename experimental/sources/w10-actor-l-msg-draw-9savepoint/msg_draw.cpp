#include "../w10-actor-l-common/access.hpp"
// Observed msg_draw__9Savepoint at 0x002156f8, 536 bytes. Let w be the word at object+128 and
// "group" the table whose address is the word at object+36 (model = group word +12, texture =
// group word +20), "state" the animation member at object+76 and "light" the member at +136.
//  1. Ambient offset: when w is nonzero, t = min(w * 4 - 32, 16) and lightSetAmbientOffset is
//     given the four halfwords {t, t, t, 0}; otherwise it is given the function-local static
//     {-32, -32, -32, 0} (observed at 0x4853c0 in the image).
//  2. A Matrix34 is built with matMakeTransYaw from a copy of the Point3 at object+16 and the
//     signed halfword at +56.
//  3. rec = modelDrawShadow(model, 0, matrix, w ? state : null, 0); then modelDraw(model,
//     texture, 0, copy of the Point3 at +16, matrix, w ? state : null, the doubleword at +120,
//     rec, 0), the word being re-read between the two.
//  4. When w is nonzero (read again): n = modelGetNmShadowPoints(model);
//     modelGetShadowPos(model, state, copy of the Point3 at +16, halfword at +56, n - 1, &out,
//     &scalar, null, 1.0); then P_FaceSprite(out, 18.0, isin(w << 9) * 2000.0 + 18000.0, 100, 100,
//     188, 64, haloTexture2, 0, 2); out is copied to the Point3 at object+184 and
//     lightUpdatePosition is called for the light.
//  5. In all cases lightResetOptionalParams is called last. Nothing is returned.
// Unknown: what w counts; the meaning of the doubleword at +120. Nothing here ran on the EE.
extern "C" {
void lightSetAmbientOffset__FPs(short* offset);
void matMakeTransYaw__FP8Matrix34G6Point3s(void* matrix, const void* position, short yaw);
void* modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(void* model, int value, void* matrix, void* state, char last);
int modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, const void* point, void* matrix, void* animation,
    unsigned long long flags, void* draw_record, int last);
int modelGetNmShadowPoints__FP12_modelHeader(void* model);
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
    void* model, void* state, const void* position, short yaw, int index, void* out, float* scalar, void* matrix, float scale);
float isin__Fi(int angle);
void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(const void* position, float first, float second, int a, int b, int c, int d,
                                               void* texture, short e, int f);
void lightUpdatePosition__FP5Light(void* light);
void lightResetOptionalParams__Fv();
extern void* haloTexture2;

void msg_draw__9Savepoint(void* self) {
    static short offset[4] = {-32, -32, -32, 0};
    unsigned char* const o = static_cast<unsigned char*>(self);
    if (w10l::sword(o, 128) != 0) {
        int t = w10l::sword(o, 128) * 4 - 32;
        if (t >= 17) t = 16;
        short values[4] = {static_cast<short>(t), static_cast<short>(t), static_cast<short>(t), 0};
        lightSetAmbientOffset__FPs(values);
    } else {
        lightSetAmbientOffset__FPs(offset);
    }
    float matrix[12];
    Point3 first = *reinterpret_cast<Point3*>(o + 16);
    matMakeTransYaw__FP8Matrix34G6Point3s(matrix, &first, w10l::half(o, 56));
    unsigned char* const group = w10l::from_word(w10l::word(o, 36));
    void* const model = w10l::from_word(w10l::word(group, 12));
    void* const texture = w10l::from_word(w10l::word(group, 20));
    void* const state = o + 76;
    void* const record = modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(
        model, 0, matrix, w10l::sword(o, 128) != 0 ? state : nullptr, 0);
    Point3 second = *reinterpret_cast<Point3*>(o + 16);
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
        model, texture, 0, &second, matrix, w10l::sword(o, 128) != 0 ? state : nullptr, w10l::dword(o, 120), record, 0);
    if (w10l::sword(o, 128) != 0) {
        const int count = modelGetNmShadowPoints__FP12_modelHeader(model);
        Point3 third = *reinterpret_cast<Point3*>(o + 16);
        Point3 out;
        float scalar;
        modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
            model, state, &third, w10l::half(o, 56), count - 1, &out, &scalar, nullptr, 1.0f);
        const float pulse = isin__Fi(w10l::sword(o, 128) << 9) * 2000.0f + 18000.0f;
        P_FaceSprite__FG6Point3ffiiiiP8_texturesi(&out, 18.0f, pulse, 100, 100, 188, 64, haloTexture2, 0, 2);
        *reinterpret_cast<Point3*>(o + 184) = out;
        lightUpdatePosition__FP5Light(o + 136);
    }
    lightResetOptionalParams__Fv();
}
}
