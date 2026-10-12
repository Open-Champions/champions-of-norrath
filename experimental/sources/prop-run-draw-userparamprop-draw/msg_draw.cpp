#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_draw__13UserParamProp at 0x14a910, 296 bytes.
// matMakeTransYaw gets a 64-byte local matrix, a copy of the 12-byte point at
// object+16 and the sign-extended sum of halfword object+56 and 16384. When
// the word at object+360 is greater (signed) than frameNm - 3, isin of
// frameNm << 9 is made absolute, multiplied by 12.0 and offset by 12.0, then
// truncated to an integer whose low byte is stored at byte 3 of an 8-byte
// static, and modelSetHalo receives that static and 1. modelDrawShadow is
// then called with the model word at object+76, 1, the matrix, the animation
// state at object+84 and a zero byte. Its result is the record argument of
// modelDraw, which gets the words at object+76 and object+80, mode 1, a
// second copy of the point, the matrix, the animation state, the 8 bytes at
// object+352 as flags and a final 0. modelResetOptionalParams runs last.
// Observed: the static is in .sdata at 0x483c00 (the image calls it
// halos.1192) with bytes ff ff ff ff 00 80 cc 43 before any run.
// Inferred: bytes 4..7 are the single 408.0, and flags are read with one 64-bit
// load. Unknown: the HaloDef layout, and what the object+360 word means.
extern "C" int frameNm;
extern "C" float isin__Fi(int angle);
extern "C" void matMakeTransYaw__FP8Matrix34G6Point3s(void* matrix, const void* point, short yaw);
extern "C" void modelSetHalo__FP7HaloDefi(void* halo, int value);
extern "C" void* modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(
    void* model, int value, void* matrix, void* animation, signed char flag);
extern "C" int modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, const void* point, void* matrix, void* animation,
    unsigned long long flags, void* draw_record, int last);
extern "C" void modelResetOptionalParams__Fv();

extern "C" void msg_draw__13UserParamProp(void* self) {
    using namespace vslots_b;
    static unsigned char halos[8] = {0xff, 0xff, 0xff, 0xff, 0x00, 0x80, 0xcc, 0x43};
    const Address base = address(self);
    unsigned char matrix[64];
    unsigned char first_point[12];
    unsigned char second_point[12];
    for (unsigned i = 0; i < 12u; ++i) first_point[i] = static_cast<unsigned char>(read_u8(base + 16u + i));
    matMakeTransYaw__FP8Matrix34G6Point3s(matrix, first_point, static_cast<short>(read_u16(base + 56u) + 16384u));
    if (frameNm - 3 < static_cast<int>(read_word(base + 360u))) {
        const float level = __builtin_fabsf(isin__Fi(static_cast<int>(static_cast<unsigned>(frameNm) << 9))) * 12.0f + 12.0f;
        halos[3] = static_cast<unsigned char>(static_cast<int>(level));
        modelSetHalo__FP7HaloDefi(halos, 1);
    }
    void* const animation = pointer(base + 84u);
    void* const record = modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(
        pointer(read_word(base + 76u)), 1, matrix, animation, 0);
    for (unsigned i = 0; i < 12u; ++i) second_point[i] = static_cast<unsigned char>(read_u8(base + 16u + i));
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
        pointer(read_word(base + 76u)), pointer(read_word(base + 80u)), 1, second_point, matrix,
        animation, read_u64(base + 352u), record, 0);
    modelResetOptionalParams__Fv();
}
