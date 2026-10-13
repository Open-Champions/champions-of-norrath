// Observed __kernel_cosf at 0x002f6c40, 336 bytes: the single-float cosine kernel, __kernel_cosf(x, y), with x the
// reduced argument (|x| about pi/4 or less) and y its low-order correction. In order, as read from the code:
//  1. ix = raw bits of x without the sign. When ix < 0x32000000 and the integer conversion of x is zero, return 1.
//  2. z = x*x and r = z*(c5 + z*(c4 + z*(c3 + z*(c2 + z*(c1 + z*c0))))), constants below (the immediates found).
//  3. ix < 0x3e99999a: return 1 - (0.5*z - (z*r - x*y)).
//     Otherwise qx is 0.28125 when ix > 0x3f480000 and else the float whose bits are ix - 0x01000000; then
//     hz = 0.5*z - qx, a = 1 - qx, and the result is a - (hz - (z*r - x*y)).
// Inferred: this is the usual fdlibm / Newlib single-float cosine kernel; the names are mine. Every step is a
// separate single-float operation (the build must not fuse them).
// Unknown: whether the original wrote the Horner chain as one expression.
extern "C" {
namespace {
float from_bits(unsigned bits) { return __builtin_bit_cast(float, bits); }
unsigned to_bits(float value) { return __builtin_bit_cast(unsigned, value); }
}

float __kernel_cosf(float x, float y) {
    const int ix = static_cast<int>(to_bits(x)) & 0x7fffffff;
    if (ix < 0x32000000) {
        if (static_cast<int>(x) == 0) return 1.0f;
    }
    const float z = x * x;
    float p = z * from_bits(0xad47d74eu);
    p = p + from_bits(0x310f74f5u);
    p = z * p;
    p = p + from_bits(0xb493f27bu);
    p = z * p;
    p = p + from_bits(0x37d00d00u);
    p = z * p;
    p = p + from_bits(0xbab60b60u);
    p = z * p;
    p = p + from_bits(0x3d2aaaaau);
    const float r = z * p;
    if (ix < 0x3e99999a) {
        float t = z * r;
        t = t - x * y;
        float h = z * 0.5f;
        h = h - t;
        return 1.0f - h;
    }
    float qx;
    if (ix > 0x3f480000) {
        qx = from_bits(0x3e900000u);
    } else {
        qx = from_bits(static_cast<unsigned>(ix - 0x01000000));
    }
    const float zr = z * r;
    const float xy = x * y;
    float hz = z * 0.5f;
    const float a = 1.0f - qx;
    hz = hz - qx;
    float t = zr - xy;
    t = hz - t;
    return a - t;
}
}
