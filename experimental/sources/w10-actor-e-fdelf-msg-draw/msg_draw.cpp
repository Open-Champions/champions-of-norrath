#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_draw__13FemaleDarkElf at 0x285708, 296 bytes. Let group be the word at object+36.
//  1. When the word at object+964 is non-zero: fxRunWeapon with the description at object+1032, the
//     model word at (group + 8 * index) + 4, the weapon state at object+968, a stack copy of the 12
//     bytes at object+16, the halfword at +56, the model at group+12, the animation state at
//     object+288, 0.5, 1 when the signed halfword at +572 is 6 (otherwise 0), 0, 1.0 and a null matrix.
//  2. The creature draw with the model at group+12 and the texture at group+20 (null fourth argument).
//  3. For the word at +964 when non-zero, and then for the word at +1104 when non-zero:
//     modelDrawAttachment with the model word at (group + 8 * index) + 4, the texture word at
//     (group + 8 * index) + 12, zero, the model at group+12, the value the creature draw returned, then -1, -1, 0.
// Unknown: what the two indexes select and what the weapon effect parameters mean.
extern "C" {
void* draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, void* extra);
void modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(
    void* model, void* texture, int zero, void* group_model, void* draw_record, unsigned long mask, int a, int b);
void fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
    void* description, void* model, void* state, const void* point, short angle, void* second_model, void* animation,
    float first, int flag, int zero, float second, void* matrix);
}
extern "C" void msg_draw__13FemaleDarkElf(void* self) {
    using namespace vslots_b;
    const Address object = address(self);
    const unsigned weapon = read_word(object + 964u);
    if (weapon != 0u) {
        unsigned char point[12];
        for (unsigned i = 0; i < 12u; ++i) point[i] = bytes(object + 16u)[i];
        const Address group = address(pointer(read_word(object + 36u)));
        fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
            bytes(object + 1032u), pointer(read_word(group + (weapon << 3) + 4u)), bytes(object + 968u), point,
            static_cast<short>(read_i16(object + 56u)), pointer(read_word(group + 12u)), bytes(object + 288u), 0.5f,
            read_i16(object + 572u) == 6 ? 1 : 0, 0, 1.0f, nullptr);
    }
    Address group = address(pointer(read_word(object + 36u)));
    void* record = draw__8CreatureP12_modelHeaderP8_textureT1(self, pointer(read_word(group + 12u)),
                                                              pointer(read_word(group + 20u)), nullptr);
    const unsigned first = read_word(object + 964u);
    if (first != 0u) {
        group = address(pointer(read_word(object + 36u)));
        const Address entry = group + (first << 3);
        modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(
            pointer(read_word(entry + 4u)), pointer(read_word(entry + 12u)), 0, pointer(read_word(group + 12u)),
            record, static_cast<unsigned long>(-1), -1, 0);
    }
    const unsigned second = read_word(object + 1104u);
    if (second != 0u) {
        group = address(pointer(read_word(object + 36u)));
        const Address entry = group + (second << 3);
        modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(
            pointer(read_word(entry + 4u)), pointer(read_word(entry + 12u)), 0, pointer(read_word(group + 12u)),
            record, static_cast<unsigned long>(-1), -1, 0);
    }
}
