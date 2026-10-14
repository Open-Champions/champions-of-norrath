// Observed msg_draw__7Vehicle at 0x270cc0, extent 612.
// 1. The byte modelClippingOn (0x48407c) is set to 1.
// 2. The vehicle body is drawn with modelDraw: model word +112, texture word
//    +116, int 1, the point at +16 by value, a local 3x4 float matrix, no
//    animation state, the 64-bit word at +392, no draw record, int 0. The
//    matrix is the nine floats at +1116 followed by the point at +16.
//    modelResetOptionalParams then runs.
// 3. For wheel slots 0..3 (words at +2088, step 4; local point offset +1932,
//    step 12), a null slot is skipped. Otherwise, with w the wheel:
//      rows: the nine floats at +1116 are copied into the local matrix again,
//      matRot(matrix, 2, short(int(w[280] * K1 * K2))) and then
//      matRot(matrix, 0, short(int(w[284] * K1 * K2))), where K1 = 0x3bb60b60
//      (word at 0x482174, about 1/180) and K2 = 0x46fffe00 (32767.0);
//      matTransform(rel, matrix33 at +1116, point at +1932 + 12*slot);
//      the wheel position is the vehicle point at +16 plus rel, per axis;
//      that position goes into the matrix translation and into rel, and
//      modelDraw runs with w model word [0], texture word [4], int 1, rel by
//      value, the matrix, no animation state, the 64-bit word at w+272, no
//      draw record and int 0, followed by modelResetOptionalParams.
//    The matrix translation survives between wheels and is overwritten before
//    each draw.
// Unknown: the wheel record's fields beyond the five read here, and what
// modelClippingOn does elsewhere. The 64-bit mask arguments are typed as
// unsigned long long because the original passes them in one 64-bit register.
// When NORRATH_W8_HOST is defined a host hook resolves 32-bit object words that
// the EE treats as pointers.
struct Point3 { float x, y, z; };
extern "C" unsigned char modelClippingOn;
extern "C" void modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int flag, Point3 point, void* matrix, void* animation, unsigned long long mask, void* record, int extra);
extern "C" void modelResetOptionalParams__Fv();
extern "C" void matRot__FP8Matrix34is(void* matrix, int axis, short angle);
extern "C" void matTransform__FP6Point3P8Matrix33T0(Point3* out, void* matrix33, Point3* in);
#if defined(NORRATH_W8_HOST)
extern "C" const unsigned char* norrath_w8_host_resolve(unsigned bits);
#endif

namespace {
typedef unsigned __attribute__((may_alias)) AliasedWord;
typedef float __attribute__((may_alias)) AliasedFloat;
typedef unsigned long long __attribute__((may_alias)) AliasedLong;
union FloatBits { unsigned bits; float value; };
const FloatBits scale_a = {0x3bb60b60u};
const FloatBits scale_b = {0x46fffe00u};
inline unsigned word(const unsigned char* o, unsigned off) { return *reinterpret_cast<const AliasedWord*>(o + off); }
inline float flt(const unsigned char* o, unsigned off) { return *reinterpret_cast<const AliasedFloat*>(o + off); }
inline void* as_pointer(unsigned bits) { return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(bits)); }
inline const unsigned char* resolve(unsigned bits) {
#if defined(NORRATH_W8_HOST)
    return norrath_w8_host_resolve(bits);
#else
    return static_cast<const unsigned char*>(as_pointer(bits));
#endif
}
}

extern "C" void msg_draw__7Vehicle(void* vehicle) {
    const unsigned char* v = static_cast<const unsigned char*>(vehicle);
    float matrix[12];
    Point3 rel;
    modelClippingOn = 1;
    for (unsigned i = 0; i < 9; ++i) matrix[i] = flt(v, 1116 + i * 4);
    matrix[9] = flt(v, 16);
    matrix[10] = flt(v, 20);
    matrix[11] = flt(v, 24);
    Point3 origin = {flt(v, 16), flt(v, 20), flt(v, 24)};
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
        as_pointer(word(v, 112)), as_pointer(word(v, 116)), 1, origin, matrix, 0,
        *reinterpret_cast<const AliasedLong*>(v + 392), 0, 0);
    modelResetOptionalParams__Fv();
    for (int slot = 0; slot < 4; ++slot) {
        const unsigned wheel_bits = word(v, 2088u + static_cast<unsigned>(slot) * 4u);
        if (wheel_bits == 0u) continue;
        const unsigned char* w = resolve(wheel_bits);
        for (unsigned i = 0; i < 9; ++i) matrix[i] = flt(v, 1116 + i * 4);
        matRot__FP8Matrix34is(matrix, 2, static_cast<short>(static_cast<int>(flt(w, 280) * scale_a.value * scale_b.value)));
        matRot__FP8Matrix34is(matrix, 0, static_cast<short>(static_cast<int>(flt(w, 284) * scale_a.value * scale_b.value)));
        matTransform__FP6Point3P8Matrix33T0(&rel, const_cast<unsigned char*>(v) + 1116,
                                            reinterpret_cast<Point3*>(const_cast<unsigned char*>(v) + 1932 + slot * 12));
        const float x = flt(v, 16) + rel.x;
        const float y = flt(v, 20) + rel.y;
        const float z = flt(v, 24) + rel.z;
        rel.x = x; rel.y = y; rel.z = z;
        matrix[9] = x; matrix[10] = y; matrix[11] = z;
        modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
            as_pointer(word(w, 0)), as_pointer(word(w, 4)), 1, rel, matrix, 0,
            *reinterpret_cast<const AliasedLong*>(w + 272), 0, 0);
        modelResetOptionalParams__Fv();
    }
}
