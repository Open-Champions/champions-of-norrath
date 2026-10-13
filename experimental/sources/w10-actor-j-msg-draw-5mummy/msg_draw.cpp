// Observed msg_draw__5Mummy at 0x00237658, 304 bytes. Let R be the block at the word object+36 and
// k the word at object+964 (the entry table R+8*k holds a model header at +4 and a texture at +12).
//  1. When k is non-zero, fxRunWeapon(description at +1032, R[8k+4], state at +968, copy of the point
//     at +16, halfword +56, R[12], animation state at +288, 0.5, (halfword +572 == 6), 0, 1.0,
//     null) runs.
//  2. draw(this, R[12], R[20], 0) draws the creature and returns a record.
//  3. When the word at +964 is still non-zero (it is read again): modelSetRelightAttachment(copy
//     of the point at +16), lightSetAmbientOffset(the shorts at +1056), modelDrawAttachment(R[8k+4],
//     R[8k+12], 0, R[12], the record, -1, -1, 0), lightResetOptionalParams(),
//     modelResetOptionalParams().
// What the entries are is unknown. Nothing here ran on the EE.
#include "../w10-actor-j-common/access.hpp"

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

void msg_draw__5Mummy(void* self) {
    using namespace w10j;
    unsigned char* const o = static_cast<unsigned char*>(self);
    unsigned char* const block = from_word(word(o, 36));
    if (word(o, 964) != 0u) {
        unsigned char* const entry = block + word(o, 964) * 8u;
        fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
            o + 1032, from_word(word(entry, 4)), o + 968, *reinterpret_cast<Point3*>(o + 16), half(o, 56), from_word(word(block, 12)),
            o + 288, 0.5f, half(o, 572) == 6 ? 1 : 0, 0, 1.0f, nullptr);
    }
    unsigned char* const record = static_cast<unsigned char*>(
        draw__8CreatureP12_modelHeaderP8_textureT1(self, from_word(word(block, 12)), from_word(word(block, 20)), nullptr));
    if (word(o, 964) != 0u) {
        unsigned char* const entry = block + word(o, 964) * 8u;
        modelSetRelightAttachment__FG6Point3(*reinterpret_cast<Point3*>(o + 16));
        lightSetAmbientOffset__FPs(reinterpret_cast<short*>(o + 1056));
        modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(
            from_word(word(entry, 4)), from_word(word(entry, 12)), 0, from_word(word(block, 12)), record, static_cast<unsigned long>(-1), -1, 0);
        lightResetOptionalParams__Fv();
        modelResetOptionalParams__Fv();
    }
}
}
