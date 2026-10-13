// Observed msg_draw__3Orc at 0x00227638, 416 bytes. "t" is the pointer at object+36 (a block
// whose words +12 and +20 are a model header and a texture), "B" is the word at +1136 (a table
// base), "idx" is the word at +972. With 8-byte steps, B+4+8*i is read as a model header and
// B+140+8*i as its texture. Steps, in order:
//  1. When idx is nonzero: fxRunWeapon (the full mangled name of the original) is called with
//     (object+1040, B[4+8*idx], object+976, the three floats at +16 by value, halfword +56,
//     t[12], object+288, 0.5, 1 when the halfword at +572 is 6 else 0, 0, float +224, null).
//  2. s = draw__8CreatureP12_modelHeaderP8_textureT1(object, t[12], t[20], 0).
//  3. When idx is nonzero, modelDrawAttachment(B[4+8*idx], B[140+8*idx], 0, t[12], s, -1, -1, 0)
//     is called. When the word at +1112 (call it j) is nonzero the same call is made with j in
//     place of idx.
//  4. When the word at +1116 equals 4: modelDrawAttachment(B[52], B[188], 0, t[12], s, -1, -1, 0)
//     and, when the word at +1124 is nonzero, modelDrawAttachment(B[140], B[276], 0, t[12], s,
//     -1, -1, 0).
// Nothing is returned. Nothing here ran on the EE; what the fields mean is unknown.
struct Point3 { float x, y, z; };
struct Matrix34 { float m[16]; };

extern "C" {
void* draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, void* texture2);
void modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(
    void* model, void* texture, int a, void* parentModel, void* record, unsigned long flags, int b, int c);
void fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
    void* description, void* model, void* state, Point3 position, short angle, void* model2,
    void* animation, float f1, int a, int b, float f2, Matrix34* matrix);

void msg_draw__3Orc(void* self) {
    unsigned char* const o = static_cast<unsigned char*>(self);
    unsigned char* const t = *reinterpret_cast<unsigned char**>(o + 36);
    const unsigned idx = *reinterpret_cast<unsigned*>(o + 972);
    if (idx != 0) {
        unsigned char* const b = *reinterpret_cast<unsigned char**>(o + 1136);
        fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
            o + 1040, *reinterpret_cast<void**>(b + 4 + 8 * idx), o + 976, *reinterpret_cast<Point3*>(o + 16),
            *reinterpret_cast<short*>(o + 56), *reinterpret_cast<void**>(t + 12), o + 288, 0.5f,
            *reinterpret_cast<short*>(o + 572) == 6, 0, *reinterpret_cast<float*>(o + 224), nullptr);
    }
    void* const record = draw__8CreatureP12_modelHeaderP8_textureT1(self, *reinterpret_cast<void**>(t + 12), *reinterpret_cast<void**>(t + 20), nullptr);
    if (*reinterpret_cast<unsigned*>(o + 972) != 0) {
        unsigned char* const b = *reinterpret_cast<unsigned char**>(o + 1136);
        const unsigned i = *reinterpret_cast<unsigned*>(o + 972);
        modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(
            *reinterpret_cast<void**>(b + 4 + 8 * i), *reinterpret_cast<void**>(b + 140 + 8 * i), 0, *reinterpret_cast<void**>(t + 12), record, ~0ul, -1, 0);
    }
    if (*reinterpret_cast<unsigned*>(o + 1112) != 0) {
        unsigned char* const b = *reinterpret_cast<unsigned char**>(o + 1136);
        const unsigned j = *reinterpret_cast<unsigned*>(o + 1112);
        modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(
            *reinterpret_cast<void**>(b + 4 + 8 * j), *reinterpret_cast<void**>(b + 140 + 8 * j), 0, *reinterpret_cast<void**>(t + 12), record, ~0ul, -1, 0);
    }
    if (*reinterpret_cast<unsigned*>(o + 1116) == 4) {
        unsigned char* const b = *reinterpret_cast<unsigned char**>(o + 1136);
        modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(
            *reinterpret_cast<void**>(b + 52), *reinterpret_cast<void**>(b + 188), 0, *reinterpret_cast<void**>(t + 12), record, ~0ul, -1, 0);
        if (*reinterpret_cast<unsigned*>(o + 1124) != 0) {
            unsigned char* const b2 = *reinterpret_cast<unsigned char**>(o + 1136);
            modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(
                *reinterpret_cast<void**>(b2 + 140), *reinterpret_cast<void**>(b2 + 276), 0, *reinterpret_cast<void**>(t + 12), record, ~0ul, -1, 0);
        }
    }
}
}
