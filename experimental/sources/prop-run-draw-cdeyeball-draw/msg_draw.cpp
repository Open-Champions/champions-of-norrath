#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_draw__9CdEyeball at 0x216878, 244 bytes.
// matMakeTransYawScale gets a 64-byte local matrix, a copy of the 12-byte
// point at object+16, the signed halfword at object+56 and the single at
// object+224. matRot then rotates that matrix on axis 1 by the signed
// halfword at object+960. A draw point is built from the singles at object+16
// and +20 and the single at +24 plus half the single at +32. modelDraw runs
// twice with that point, mode 1, the matrix, a null animation, the 8 bytes at
// object+552 as flags, a null record and a final 0: first with the words at
// +28 and +36 of the group at object+36, then, re-reading object+36, with the
// words at +12 and +20 of that group.
// Observed: 0.5 is the lui 0x3f00 constant. Unknown: what the two models are
// and what object+224, +32 and +960 mean.
extern "C" void matMakeTransYawScale__FP8Matrix34G6Point3sf(void* matrix, const void* point, short yaw, float scale);
extern "C" void matRot__FP8Matrix34is(void* matrix, int axis, short angle);
extern "C" int modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, const void* point, void* matrix, void* animation,
    unsigned long long flags, void* draw_record, int last);

extern "C" void msg_draw__9CdEyeball(void* self) {
    using namespace vslots_b;
    const Address base = address(self);
    unsigned char matrix[64];
    unsigned char first_point[12];
    unsigned char draw_point[12];
    for (unsigned i = 0; i < 12u; ++i) first_point[i] = static_cast<unsigned char>(read_u8(base + 16u + i));
    matMakeTransYawScale__FP8Matrix34G6Point3sf(matrix, first_point, read_i16(base + 56u), read_single(base + 224u));
    matRot__FP8Matrix34is(matrix, 1, read_i16(base + 960u));
    const float x = read_single(base + 16u);
    const float y = read_single(base + 20u);
    const float z = read_single(base + 24u) + read_single(base + 32u) * 0.5f;
    write_single(address(draw_point), x);
    write_single(address(draw_point) + 4u, y);
    write_single(address(draw_point) + 8u, z);
    Address group = address(pointer(read_word(base + 36u)));
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
        pointer(read_word(group + 28u)), pointer(read_word(group + 36u)), 1, draw_point, matrix, nullptr,
        read_u64(base + 552u), nullptr, 0);
    group = address(pointer(read_word(base + 36u)));
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
        pointer(read_word(group + 12u)), pointer(read_word(group + 20u)), 1, draw_point, matrix, nullptr,
        read_u64(base + 552u), nullptr, 0);
}
