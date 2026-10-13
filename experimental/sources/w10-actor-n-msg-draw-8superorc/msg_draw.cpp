// Observed msg_draw__8SuperOrc at 0x00229f90, 276 bytes. Let R be the block at the word object+36.
// All steps are unconditional:
//  1. fxRunWeapon(description at +1040, R[124], state at +972, copy of the point at +16, halfword
//     +56, R[12], animation state at +288, 0.5, (halfword +572 == 6), 0, float at +224, null).
//  2. draw(this, R[12], R[20], 0) draws the creature and returns a record.
//  3. modelSetRelightAttachment(copy of the point at +16); lightSetAmbientOffset(the shorts at
//     +1064); modelDrawAttachment(R[124], R[132], 0, R[12], the record, -1, -1, 0);
//     lightResetOptionalParams(); modelResetOptionalParams().
// What R[124] and R[132] are is unknown (a model header and a texture by the argument types).
// Nothing here ran on the EE.
#include "../w10-actor-n-common/access.hpp"

extern "C" {
void fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
    void* description, void* model, void* state, Point3 position, short a, void* model2, void* animation, float f, int g, int h,
    float i, void* matrix);
void* draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, void* texture2);
void modelSetRelightAttachment__FG6Point3(Point3 position);
void lightSetAmbientOffset__FPs(short* offset);
void modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(void* model, void* texture, int a, void* model2, void* record,
                                                                         unsigned long b, int c, int d);
void lightResetOptionalParams__Fv();
void modelResetOptionalParams__Fv();

void msg_draw__8SuperOrc(void* self) {
    using namespace w10n;
    unsigned char* const o = static_cast<unsigned char*>(self);
    unsigned char* const block = from_word(word(o, 36));
    fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
        o + 1040, from_word(word(block, 124)), o + 972, *reinterpret_cast<Point3*>(o + 16), half(o, 56), from_word(word(block, 12)),
        o + 288, 0.5f, half(o, 572) == 6 ? 1 : 0, 0, real(o, 224), nullptr);
    unsigned char* const record = static_cast<unsigned char*>(
        draw__8CreatureP12_modelHeaderP8_textureT1(self, from_word(word(block, 12)), from_word(word(block, 20)), nullptr));
    modelSetRelightAttachment__FG6Point3(*reinterpret_cast<Point3*>(o + 16));
    lightSetAmbientOffset__FPs(reinterpret_cast<short*>(o + 1064));
    modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(
        from_word(word(block, 124)), from_word(word(block, 132)), 0, from_word(word(block, 12)), record, static_cast<unsigned long>(-1), -1, 0);
    lightResetOptionalParams__Fv();
    modelResetOptionalParams__Fv();
}
}
