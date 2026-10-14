// Observed msg_draw__7Ulthork at 0x001f5358, 272 bytes. Let R be the block at the word object+36 and
// w the word at object+960.
//  1. When w is nonzero: fxRunWeapon with the description at +1032, the word at R + 8w + 4, the weapon
//     state at +964, a copy of the point at +16, the halfword at +56, the word R[12], the animation
//     state at +288, 0.5, (halfword +572 == 6), 0, 1.0 and a null matrix.
//  2. modelSetShadowScale(2.0f).
//  3. draw(this, R[12], the word at R + 8 * (signed byte at +1104) + 20, 0) draws the creature and
//     returns a record (the image reads R again after the shadow call).
//  4. modelResetOptionalParams().
//  5. When w is nonzero: modelDrawAttachment(the word at R + 8w + 4, the word at R + 8w + 12, 0, R[12],
//     the record, -1, -1, 0).
// What the table words are is unknown (model headers and textures by the argument types).
// Nothing here ran on the EE.
#include "../w10-actor-o-common/access.hpp"

extern "C" {
void fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
    void* description, void* model, void* state, Point3 position, short a, void* model2, void* animation, float f, int g, int h,
    float i, void* matrix);
void modelSetShadowScale__Ff(float scale);
void* draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, void* texture2);
void modelResetOptionalParams__Fv();
void modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(void* model, void* texture, int a, void* model2, void* record,
                                                                         unsigned long b, int c, int d);

void msg_draw__7Ulthork(void* self) {
    using namespace w10o;
    unsigned char* const o = static_cast<unsigned char*>(self);
    const unsigned w = word(o, 960);
    if (w != 0) {
        unsigned char* const group = from_word(word(o, 36));
        const Point3 position = *reinterpret_cast<Point3*>(o + 16);
        fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
            o + 1032, from_word(word(group, w * 8 + 4)), o + 964, position, half(o, 56),
            from_word(word(group, 12)), o + 288, 0.5f, half(o, 572) == 6 ? 1 : 0, 0, 1.0f, nullptr);
    }
    modelSetShadowScale__Ff(2.0f);
    unsigned char* group = from_word(word(o, 36));
    void* const record = draw__8CreatureP12_modelHeaderP8_textureT1(
        self, from_word(word(group, 12)), from_word(word(group, static_cast<signed char>(o[1104]) * 8 + 20)), nullptr);
    modelResetOptionalParams__Fv();
    if (word(o, 960) != 0) {
        group = from_word(word(o, 36));
        unsigned char* const entry = group + word(o, 960) * 8;
        modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(
            from_word(word(entry, 4)), from_word(word(entry, 12)), 0, from_word(word(group, 12)), record, static_cast<unsigned long>(-1), -1, 0);
    }
}
}
