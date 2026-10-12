// Observed msg_draw__9Container at 0x0021d900, 192 bytes.
// Copies the 12 bytes at object+16 and builds a matrix with matMakeTransYaw from that
// copy and the signed 16-bit sum of the halfword at object+56 plus 16384. Then
// modelDrawShadow gets the model word at object+96, the constant 1, the matrix, a null
// animation pointer and the constant 1; its result is passed as the draw-record argument
// of modelDraw. modelDraw gets the words at object+96 and object+100, 1, a second copy of
// the point, the matrix, a null animation pointer, the 32-bit value -1, the shadow result
// and a zero final argument. Returns nothing. The Matrix34 layout is not established here.

extern "C" void matMakeTransYaw__FP8Matrix34G6Point3s(void* matrix, const void* point, short yaw);
extern "C" void* modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(
    void* model, int mode, void* matrix, void* animation, char flag);
extern "C" int modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, const void* point, void* matrix, void* animation,
    unsigned flags, void* draw_record, int last);

namespace {
unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
void* pointer_from_word(unsigned value) {
    return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(value));
}
void copy_bytes(unsigned char* dest, const unsigned char* src, unsigned count) {
    for (unsigned i = 0; i < count; ++i) dest[i] = src[i];
}
}

extern "C" void msg_draw__9Container(void* object_storage) {
    const unsigned char* object = static_cast<const unsigned char*>(object_storage);
    unsigned char matrix[64];
    unsigned char first_point[12];
    unsigned char second_point[12];
    copy_bytes(first_point, object + 16u, 12u);
    const unsigned yaw_raw = unsigned(object[56]) | (unsigned(object[57]) << 8);
    const unsigned summed = (yaw_raw + 16384u) & 0xffffu;
    const short yaw = static_cast<short>(summed < 0x8000u ? static_cast<int>(summed) : static_cast<int>(summed) - 65536);
    matMakeTransYaw__FP8Matrix34G6Point3s(matrix, first_point, yaw);
    void* shadow = modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(
        pointer_from_word(read_word(object + 96u)), 1, matrix, nullptr, 1);
    copy_bytes(second_point, object + 16u, 12u);
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
        pointer_from_word(read_word(object + 96u)), pointer_from_word(read_word(object + 100u)), 1,
        second_point, matrix, nullptr, 0xffffffffu, shadow, 0);
}
