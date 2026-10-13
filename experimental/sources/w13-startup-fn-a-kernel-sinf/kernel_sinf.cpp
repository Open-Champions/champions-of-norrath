// Observed __kernel_sinf at 0x002f75f8, 260 bytes: the single-float sine kernel, __kernel_sinf(x, y, iy), with x the
// reduced argument (|x| about pi/4 or less), y its low-order correction and iy a flag: zero means y is ignored.
// In order, as read from the code:
//  1. ix = raw bits of x without the sign. When ix < 0x32000000 and the integer conversion of x is zero, return x.
//  2. z = x*x, v = z*x, and r = c5 + z*(c4 + z*(c3 + z*(c2 + z*c1))) with the five constants below.
//  3. iy == 0: return x + v*(s1 + z*r).
//     otherwise: return x - ((z*(0.5*y - v*r) - y) - v*s1).
// The constants are the immediates found in the code (hex below). Inferred: this is the usual fdlibm / Newlib
// single-float sine kernel and s1 is its -1/6 term; the names are mine. Every step is a separate single-float
// operation (the build must not fuse them).
// Unknown: whether the original source wrote the polynomial as one expression.
extern "C" {
namespace {
static float from_bits(unsigned bits) { return __builtin_bit_cast(float, bits); }
static unsigned to_bits(float value) { return __builtin_bit_cast(unsigned, value); }
}

float __kernel_sinf(float x, float y, int iy) {
    const int ix = static_cast<int>(to_bits(x)) & 0x7fffffff;
    if (ix < 0x32000000) {
        if (static_cast<int>(x) == 0) return x;
    }
    const float z = x * x;
    const float v = z * x;
    float r = z * from_bits(0x2f2ec9d2u);
    r = r + from_bits(0xb2d72f34u);
    r = z * r;
    r = r + from_bits(0x3638ef1au);
    r = z * r;
    r = r + from_bits(0xb9500d01u);
    r = z * r;
    r = r + from_bits(0x3c088889u);
    const float s1 = from_bits(0xbe2aaaaau);
    if (iy == 0) {
        float t = z * r;
        t = t + s1;
        t = v * t;
        return x + t;
    }
    const float half_y = y * from_bits(0x3f000000u);
    const float vr = v * r;
    const float vs = v * s1;
    float t = half_y - vr;
    t = z * t;
    t = t - y;
    t = t - vs;
    return x - t;
}
}
