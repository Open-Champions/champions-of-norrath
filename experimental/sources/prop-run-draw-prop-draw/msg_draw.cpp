#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_draw__4Prop at 0x154828, 308 bytes.
// If bit 0 of the halfword at object+320 is clear, a copy of the 12-byte point
// at object+16 and (halfword object+56 + 16384), sign-extended, go to
// matMakeTransYaw with the matrix storage at object+272. When the word at
// object+260 is greater (signed) than frameNm - 3, an 8-byte local is zeroed
// through memset, its halfword at byte 6 is set to 32, and lightSetAmbientOffset
// receives it. modelDraw then gets the words at object+76 and object+80, mode 1,
// a second copy of the point, the matrix at object+272, a null animation, the
// 8 bytes at object+232 as the flags, a null record and a final 0. When the
// same frameNm compare holds again, lightResetOptionalParams runs.
// Inferred: the flags are read with one 64-bit load in the reference; the
// callee's flag parameter is an unsigned long, so only the low word is
// established as used. frameNm is the word at 0x4844a4. Unknown: what the
// object+320 bit, object+260 word and the 8-byte local mean.

extern "C" int frameNm;
extern "C" void* memset(void* dest, int value, __SIZE_TYPE__ count);
extern "C" void matMakeTransYaw__FP8Matrix34G6Point3s(void* matrix, const void* point, short yaw);
extern "C" void lightSetAmbientOffset__FPs(void* offsets);
extern "C" void lightResetOptionalParams__Fv();
extern "C" int modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, const void* point, void* matrix, void* animation,
    unsigned long long flags, void* draw_record, int last);

extern "C" void msg_draw__4Prop(void* self) {
    using namespace vslots_b;
    const Address base = address(self);
    unsigned char first_point[12];
    unsigned char second_point[12];
    unsigned char local[8];
    if ((read_u16(base + 320u) & 1u) == 0u) {
        for (unsigned i = 0; i < 12u; ++i) first_point[i] = static_cast<unsigned char>(read_u8(base + 16u + i));
        const short yaw = static_cast<short>(read_u16(base + 56u) + 16384u);
        matMakeTransYaw__FP8Matrix34G6Point3s(pointer(base + 272u), first_point, yaw);
    }
    const bool lit = frameNm - 3 < static_cast<int>(read_word(base + 260u));
    if (lit) {
        memset(local, 0, 8);
        local[6] = 32;
        local[7] = 0;
        lightSetAmbientOffset__FPs(local);
    }
    for (unsigned i = 0; i < 12u; ++i) second_point[i] = static_cast<unsigned char>(read_u8(base + 16u + i));
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
        pointer(read_word(base + 76u)), pointer(read_word(base + 80u)), 1, second_point,
        pointer(base + 272u), nullptr, read_u64(base + 232u), nullptr, 0);
    if (frameNm - 3 < static_cast<int>(read_word(base + 260u))) lightResetOptionalParams__Fv();
}
