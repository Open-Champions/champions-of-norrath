#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_draw__16CosmeticPropAnim at 0x155110, 168 bytes.
// A copy of the 12-byte point at object+16 and the sign-extended sum of the
// halfword at object+56 and 16384 go to matMakeTransYaw with a 64-byte local
// matrix. modelDraw then receives the words at object+92 and object+96, mode
// 1, a second copy of the point, that matrix, the animation state at
// object+100, the word at object+144 as flags, a null record and a final 0.
// Nothing is returned. The Matrix34 field layout is not established here.
extern "C" void matMakeTransYaw__FP8Matrix34G6Point3s(void* matrix, const void* point, short yaw);
extern "C" int modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, const void* point, void* matrix, void* animation,
    unsigned flags, void* draw_record, int last);

extern "C" void msg_draw__16CosmeticPropAnim(void* self) {
    using namespace vslots_b;
    const Address base = address(self);
    unsigned char matrix[64];
    unsigned char first_point[12];
    unsigned char second_point[12];
    for (unsigned i = 0; i < 12u; ++i) first_point[i] = static_cast<unsigned char>(read_u8(base + 16u + i));
    matMakeTransYaw__FP8Matrix34G6Point3s(matrix, first_point, static_cast<short>(read_u16(base + 56u) + 16384u));
    for (unsigned i = 0; i < 12u; ++i) second_point[i] = static_cast<unsigned char>(read_u8(base + 16u + i));
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
        pointer(read_word(base + 92u)), pointer(read_word(base + 96u)), 1, second_point, matrix,
        pointer(base + 100u), read_word(base + 144u), nullptr, 0);
}
