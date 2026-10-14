// Observed msg_draw__3Ice at 0x001e6330, 168 bytes.
// Copies the 12 bytes at object+16 and builds a matrix with matMakeTransYaw from that copy and the
// 16-bit sum of the halfword at object+56 plus 16384. Then calls modelDraw with the model word at
// object+96, the texture word at object+100, mode 1, a second copy of the 12 bytes, the matrix, a
// null animation pointer, the 32-bit value -1, a null draw record and a zero final argument.
// Returns nothing. Unlike the container draw there is no shadow call. The Matrix34 layout is not
// established here. Nothing here ran on the EE.
#include "../w10-actor-h-common/words.hpp"

extern "C" {
void matMakeTransYaw__FP8Matrix34G6Point3s(void* matrix, const void* point, short yaw);
int modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, const void* point, void* matrix, void* animation,
    unsigned flags, void* draw_record, int last);
}

extern "C" void msg_draw__3Ice(void* self) {
    unsigned char* object = static_cast<unsigned char*>(self);
    alignas(16) unsigned char matrix[64];
    unsigned char first_point[12];
    unsigned char second_point[12];
    w10h::copy_bytes(first_point, object + 16, 12);
    const short yaw = static_cast<short>(*reinterpret_cast<unsigned short*>(object + 56) + 16384);
    matMakeTransYaw__FP8Matrix34G6Point3s(matrix, first_point, yaw);
    w10h::copy_bytes(second_point, object + 16, 12);
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
        w10h::from_word(w10h::word_at(self, 96)), w10h::from_word(w10h::word_at(self, 100)), 1,
        second_point, matrix, nullptr, 0xffffffffu, nullptr, 0);
}
