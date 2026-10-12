// Observed msg_draw__15PushPhysicsProp at 0x00155d68, 220 bytes.
// Builds a 12-float matrix from the object's floats: the nine floats at +148, +160,
// +172, +152, +164, +176, +156, +168, +180 (in that order) followed by the position
// floats at +96, +100, +104. modelDrawShadow gets the model word at +260, 1, that
// matrix, a null animation pointer and the char 0; its result becomes the draw-record
// argument of modelDraw. modelDraw gets the words at +260 and +264, 1, a copy of the
// 12 bytes at +96, the matrix, a null animation pointer, the 32-bit value -1, the shadow
// result and 0. Returns nothing. The Matrix34 field names are not established here.

extern "C" void* modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(
    void* model, int mode, void* matrix, void* animation, char flag);
extern "C" int modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, const void* point, void* matrix, void* animation,
    unsigned flags, void* draw_record, int last);

namespace {
typedef float FloatWord __attribute__((may_alias));
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

extern "C" void msg_draw__15PushPhysicsProp(void* object_storage) {
    const unsigned char* object = static_cast<const unsigned char*>(object_storage);
    float matrix[12];
    matrix[0] = *reinterpret_cast<const FloatWord*>(object + 148);
    matrix[1] = *reinterpret_cast<const FloatWord*>(object + 160);
    matrix[2] = *reinterpret_cast<const FloatWord*>(object + 172);
    matrix[3] = *reinterpret_cast<const FloatWord*>(object + 152);
    matrix[4] = *reinterpret_cast<const FloatWord*>(object + 164);
    matrix[5] = *reinterpret_cast<const FloatWord*>(object + 176);
    matrix[6] = *reinterpret_cast<const FloatWord*>(object + 156);
    matrix[7] = *reinterpret_cast<const FloatWord*>(object + 168);
    matrix[8] = *reinterpret_cast<const FloatWord*>(object + 180);
    matrix[9] = *reinterpret_cast<const FloatWord*>(object + 96);
    matrix[10] = *reinterpret_cast<const FloatWord*>(object + 100);
    matrix[11] = *reinterpret_cast<const FloatWord*>(object + 104);
    unsigned char point[12];
    void* shadow = modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(
        pointer_from_word(read_word(object + 260u)), 1, matrix, nullptr, 0);
    copy_bytes(point, object + 96u, 12u);
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
        pointer_from_word(read_word(object + 260u)), pointer_from_word(read_word(object + 264u)), 1, point,
        matrix, nullptr, 0xffffffffu, shadow, 0);
}
