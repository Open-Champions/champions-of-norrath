// Observed msg_draw__12CosmeticProp at 0x1554e8, 168 bytes.
// Copies the 12 bytes at object+16, adds 16384 to the unsigned halfword at
// object+56, and sign-extends that 16-bit sum. matMakeTransYaw receives the
// matrix storage, that point copy, and the sum. modelDraw then receives the
// words at object+88 and object+92, the constant 1, a second copy of the
// same 12 bytes, the matrix, a zero animation pointer, the 32-bit value -1,
// a zero draw-record pointer, and a zero final argument. The return word is
// modelDraw's result. The original stack leaves 64 bytes between the matrix
// pointer and the first point copy; this local is that span. The Matrix34
// field layout is not established here.

extern "C" void matMakeTransYaw__FP8Matrix34G6Point3s(void* matrix, const void* point, short yaw);
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

extern "C" int norrath_vt_msg_draw_12CosmeticProp(void* object_storage) {
    const unsigned char* object = static_cast<const unsigned char*>(object_storage);
    unsigned char matrix[64];
    unsigned char first_point[12];
    unsigned char second_point[12];
    copy_bytes(first_point, object + 16u, 12u);
    const unsigned yaw_raw = unsigned(object[56]) | (unsigned(object[57]) << 8);
    const unsigned summed = (yaw_raw + 16384u) & 0xffffu;
    const short yaw = static_cast<short>(summed < 0x8000u ? static_cast<int>(summed) : static_cast<int>(summed) - 65536);
    matMakeTransYaw__FP8Matrix34G6Point3s(matrix, first_point, yaw);
    copy_bytes(second_point, object + 16u, 12u);
    return modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
        pointer_from_word(read_word(object + 88u)), pointer_from_word(read_word(object + 92u)), 1,
        second_point, matrix, nullptr, 0xffffffffu, nullptr, 0);
}
