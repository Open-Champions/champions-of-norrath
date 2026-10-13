// Observed msg_draw__8Catapult at 0x0028bf78, 600 bytes. Offsets are into the object.
//   - when the word at +84 has any of its upper 16 bits set, nothing happens;
//   - otherwise: D = object point (+16, three floats) minus worldCameraPos; V = the third column
//     of worldViewMatrix (floats 2, 6 and 10), scaled to unit length with the architectural
//     reciprocal square root of the sum of squares (computed as x*x + y*y + z*z, in that order);
//   - modelSetShadowScale gets a float constant from the image's small-data pool (about 2.918);
//   - matMakeTransYaw builds the matrix from a copy of the +16 point and the halfword at +56
//     plus 16384, truncated to a signed 16-bit value;
//   - when the dot product D.V (summed x, y, z in that order) is below -244.0, the draw record
//     comes from modelDrawShadow(model, 1, matrix, object+92, 0), otherwise it is null;
//   - modelDraw(model, texture, 0, a copy of the +16 point, matrix, A, all-ones 64-bit flags,
//     record, 0), where A is object+92 when the signed byte at +85 is nonzero, else null;
//   - modelResetOptionalParams.
// Model and texture are the words at +12 and +20 of the structure pointed to by the 32-bit
// pointer at +36 (the shadow call uses only the model). Returns nothing.
// Unknown: what +84 and +85 mean, and what the -244 threshold represents.
extern "C" {
void modelSetShadowScale__Ff(float scale);
void matMakeTransYaw__FP8Matrix34G6Point3s(void* matrix, const void* point, short yaw);
void* modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(
    void* model, int mode, void* matrix, void* animation, char flag);
int modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, const void* point, void* matrix, void* animation,
    unsigned long long flags, void* draw_record, int last);
void modelResetOptionalParams__Fv();
extern float worldCameraPos[3];
extern float worldViewMatrix[16];
#if defined(NORRATH_W10AC_HOST)
extern unsigned char* norrath_w10ac_host_base;
#endif
}

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
float read_float(const unsigned char* p) {
    union { unsigned u; float f; } v;
    v.u = read_word(p);
    return v.f;
}
const float kShadowScale = 2.917963981628418f;

__attribute__((always_inline)) inline float reciprocal_sqrt(float value) {
#if defined(__mips__)
    float result;
    float one = 1.0f;
    __asm__ volatile("rsqrt.s %0, %1, %2" : "=f"(result) : "f"(one), "f"(value));
    return result;
#else
    return 1.0f / __builtin_sqrtf(value);
#endif
}
}

extern "C" void msg_draw__8Catapult(void* object_storage) {
    unsigned char* object = static_cast<unsigned char*>(object_storage);
    if ((read_word(object + 84u) & 0xffff0000u) != 0u) return;
    float dx = read_float(object + 16u) - worldCameraPos[0];
    float dy = read_float(object + 20u) - worldCameraPos[1];
    float dz = read_float(object + 24u) - worldCameraPos[2];
    float vx = worldViewMatrix[2];
    float vy = worldViewMatrix[6];
    float vz = worldViewMatrix[10];
    float length_squared = vx * vx + vy * vy + vz * vz;
    float scale = reciprocal_sqrt(length_squared);
    vx = vx * scale;
    vy = vy * scale;
    vz = vz * scale;
    modelSetShadowScale__Ff(kShadowScale);
    float matrix[12];
    unsigned char point[12];
    copy_bytes(point, object + 16u, 12u);
    unsigned yaw = static_cast<unsigned>(object[56]) | (static_cast<unsigned>(object[57]) << 8);
    matMakeTransYaw__FP8Matrix34G6Point3s(matrix, point, static_cast<short>(yaw + 16384u));
    unsigned char* shared = static_cast<unsigned char*>(pointer_from_word(read_word(object + 36u)));
    void* record = nullptr;
    float along = dx * vx + dy * vy + dz * vz;
    if (along < -244.0f)
        record = modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(
            pointer_from_word(read_word(shared + 12u)), 1, matrix, object + 92, 0);
    void* animation = nullptr;
    if (static_cast<signed char>(object[85]) != 0) animation = object + 92;
    unsigned char draw_point[12];
    copy_bytes(draw_point, object + 16u, 12u);
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
        pointer_from_word(read_word(shared + 12u)), pointer_from_word(read_word(shared + 20u)), 0, draw_point, matrix,
        animation, 0xffffffffffffffffull, record, 0);
    modelResetOptionalParams__Fv();
}
