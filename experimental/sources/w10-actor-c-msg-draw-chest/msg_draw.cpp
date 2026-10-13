// Observed msg_draw__5Chest at 0x0021db90, 240 bytes. Offsets are into the object. In order:
//   1. matMakeTransYaw builds the matrix from a copy of the 12 bytes at +16 and the halfword
//      at +56 plus 16384, truncated to a signed 16-bit value;
//   2. the model and texture are the words at +12 and +20 of the structure pointed to by the
//      32-bit pointer at +36;
//   3. the animation pointer A is object+92 when the signed byte at +96 is nonzero, else null;
//   4. modelDrawShadow(model, 1, matrix, A, 0) supplies the draw record R;
//   5. modelDraw(model, texture, 1, a second copy of the +16 point, matrix, A, the signed word
//      at +84 widened to 64 bits, R, 0).
// Returns nothing. Unknown: what the fields at +84 and +96 mean, and the offset 16384 (a
// quarter turn if the yaw is a 16-bit angle, which is not established here).
extern "C" {
void matMakeTransYaw__FP8Matrix34G6Point3s(void* matrix, const void* point, short yaw);
void* modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(
    void* model, int mode, void* matrix, void* animation, char flag);
int modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, const void* point, void* matrix, void* animation,
    unsigned long long flags, void* draw_record, int last);
}

#if defined(NORRATH_W10AC_HOST)
extern unsigned char* norrath_w10ac_host_base;
#endif

namespace {
unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
#if defined(NORRATH_W10AC_HOST)
// Host check only: pointer words are offsets from a base chosen by the check.
void* pointer_from_word(unsigned value) { return norrath_w10ac_host_base + value; }
#else
void* pointer_from_word(unsigned value) {
    return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(value));
}
#endif
void copy_bytes(unsigned char* dest, const unsigned char* src, unsigned count) {
    for (unsigned i = 0; i < count; ++i) dest[i] = src[i];
}
}

extern "C" void msg_draw__5Chest(void* object_storage) {
    unsigned char* object = static_cast<unsigned char*>(object_storage);
    float matrix[12];
    unsigned char first_point[12];
    copy_bytes(first_point, object + 16u, 12u);
    unsigned yaw = static_cast<unsigned>(object[56]) | (static_cast<unsigned>(object[57]) << 8);
    unsigned char* shared = static_cast<unsigned char*>(pointer_from_word(read_word(object + 36u)));
    void* model = pointer_from_word(read_word(shared + 12u));
    void* texture = pointer_from_word(read_word(shared + 20u));
    matMakeTransYaw__FP8Matrix34G6Point3s(matrix, first_point, static_cast<short>(yaw + 16384u));
    void* animation = nullptr;
    if (static_cast<signed char>(object[96]) != 0) animation = object + 92;
    void* record = modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(model, 1, matrix, animation, 0);
    unsigned char second_point[12];
    copy_bytes(second_point, object + 16u, 12u);
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
        model, texture, 1, second_point, matrix, animation,
        static_cast<unsigned long long>(static_cast<long long>(static_cast<int>(read_word(object + 84u)))), record, 0);
}
