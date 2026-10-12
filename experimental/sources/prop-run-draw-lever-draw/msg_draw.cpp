#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_draw__5Lever at 0x21cad8, 236 bytes.
// The word at object+36 points at a group whose words at +12 and +20 are the
// model and texture. matMakeTransYaw gets a 64-byte local matrix, a copy of
// the 12-byte point at object+16 and the signed halfword at object+56.
// evalHermite of (single object+76, -7000.0, 7000.0, 0.0, 0.0) is truncated to
// an integer, narrowed to 16 bits and passed to matRot with the matrix and
// axis 0. modelDraw then gets the model, texture, mode 3, a second copy of the
// point, the matrix, a null animation, flags 0xffffffff, a null record and 0.
// Observed: the two limits are the .lit4 words 0xc5dac000 and 0x45dac000 at
// 0x4816cc and 0x4816d0. Unknown: what the group or the hermite curve mean.
extern "C" void matMakeTransYaw__FP8Matrix34G6Point3s(void* matrix, const void* point, short yaw);
extern "C" float evalHermite__Ffffff(float a, float b, float c, float d, float e);
extern "C" void matRot__FP8Matrix33is(void* matrix, int axis, short angle);
extern "C" int modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, const void* point, void* matrix, void* animation,
    unsigned flags, void* draw_record, int last);

extern "C" void msg_draw__5Lever(void* self) {
    using namespace vslots_b;
    const Address base = address(self);
    const Address group = address(pointer(read_word(base + 36u)));
    void* const texture = pointer(read_word(group + 20u));
    void* const model = pointer(read_word(group + 12u));
    unsigned char matrix[64];
    unsigned char first_point[12];
    unsigned char second_point[12];
    for (unsigned i = 0; i < 12u; ++i) first_point[i] = static_cast<unsigned char>(read_u8(base + 16u + i));
    matMakeTransYaw__FP8Matrix34G6Point3s(matrix, first_point, read_i16(base + 56u));
    const float angle = evalHermite__Ffffff(read_single(base + 76u), -7000.0f, 7000.0f, 0.0f, 0.0f);
    matRot__FP8Matrix33is(matrix, 0, static_cast<short>(static_cast<int>(angle)));
    for (unsigned i = 0; i < 12u; ++i) second_point[i] = static_cast<unsigned char>(read_u8(base + 16u + i));
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
        model, texture, 3, second_point, matrix, nullptr, 0xffffffffu, nullptr, 0);
}
