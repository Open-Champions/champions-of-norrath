#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_draw__7Natasla at 0x233400, 940 bytes.
// draw__8Creature gets the object, the words at +12 and +20 of the group at
// object+36 and a null fourth argument. Five parts then follow, index i from
// 0 to 4, each re-reading the group word at object+36:
//   modelGetCharAttachmentPos gets the group word +12, a fresh copy of the
//   12-byte point at object+16, the signed halfword at object+56, the
//   animation state at object+288, i, two 12-byte results A (stack +16) and
//   B (stack +32), and the single 1.0. A 48-byte local matrix is then set with
//   matIdent, matTrans(A.x, A.y, A.z) and matRot(axis, angle) where the angle
//   is the 16-bit narrowing of a shift of the unsigned halfword at object+808:
//     i=0: axis 2, h << 7;  i=1: axis 1, h << 12;  i=2: axis 0, h << 8;
//     i=3: axis 2, h << 10; i=4: axis 2, (0 - h) << 9.
//   modelDraw gets the group words at 36+16i and 44+16i, mode 1, A, the
//   matrix, a null animation, flags 0xffffffff, a null record and 0.
// Unknown: what the five attachments are, and what B is for (it is not read).
extern "C" void draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, void* extra);
extern "C" void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
    void* model, const void* point, short yaw, void* state, int index, void* first, void* second, float scale);
extern "C" void matIdent__FP8Matrix34(void* matrix);
extern "C" void matTrans__FP8Matrix34fff(void* matrix, float x, float y, float z);
extern "C" void matRot__FP8Matrix34is(void* matrix, int axis, short angle);
extern "C" int modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, const void* point, void* matrix, void* animation,
    unsigned flags, void* draw_record, int last);

extern "C" void msg_draw__7Natasla(void* self) {
    using namespace vslots_b;
    const Address base = address(self);
    unsigned char attach[12];
    unsigned char other[12];
    unsigned char matrix[48];
    unsigned char point_copy[12];
    {
        const Address group = address(pointer(read_word(base + 36u)));
        draw__8CreatureP12_modelHeaderP8_textureT1(self, pointer(read_word(group + 12u)), pointer(read_word(group + 20u)), nullptr);
    }
    static const int axes[5] = {2, 1, 0, 2, 2};
    for (unsigned i = 0; i < 5u; ++i) {
        Address group = address(pointer(read_word(base + 36u)));
        for (unsigned j = 0; j < 12u; ++j) point_copy[j] = static_cast<unsigned char>(read_u8(base + 16u + j));
        modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
            pointer(read_word(group + 12u)), point_copy, read_i16(base + 56u), pointer(base + 288u),
            static_cast<int>(i), attach, other, 1.0f);
        matIdent__FP8Matrix34(matrix);
        matTrans__FP8Matrix34fff(matrix, read_single(address(attach)), read_single(address(attach) + 4u),
                                 read_single(address(attach) + 8u));
        const unsigned half = read_u16(base + 808u);
        unsigned angle;
        switch (i) {
        case 0: angle = half << 7; break;
        case 1: angle = half << 12; break;
        case 2: angle = half << 8; break;
        case 3: angle = half << 10; break;
        default: angle = (0u - half) << 9; break;
        }
        matRot__FP8Matrix34is(matrix, axes[i], static_cast<short>(angle & 0xffffu));
        group = address(pointer(read_word(base + 36u)));
        modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
            pointer(read_word(group + 36u + 16u * i)), pointer(read_word(group + 44u + 16u * i)), 1, attach, matrix,
            nullptr, 0xffffffffu, nullptr, 0);
    }
}
