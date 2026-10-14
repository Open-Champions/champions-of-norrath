// Observed __ieee754_atan2f at 0x002f4978, 480 bytes, pinned SLUS_205.65. Arguments (y, x) arrive in
// $f12 and $f13. Four-quadrant arc tangent built on atanf (0x002f0f28) and fabsf (0x002f1368),
// in the fdlibm style, from the code only:
//   - x bits equal to 0x3f800000 (x == 1.0): atanf(y), as a tail call.
//   - m = (sign bit of y) | ((sign bit of x) << 1), taken from the raw bits.
//   - |y| bits <= 0x007fffff (zero or subnormal y): m 0 or 1 return y; m 2 returns pi; m 3
//     returns -pi. (The usual "+tiny" terms are absent.)
//   - otherwise |x| bits <= 0x007fffff (zero or subnormal x): -pi/2 when y is negative, else pi/2.
//   - otherwise k = (|y| bits - |x| bits) >> 23 (arithmetic) and z is: pi/2 when k >= 61; 0 when
//     k < -60 and x is negative; else atanf(fabsf(y / x)). Then m 0: z; m 1: -z (sign flip on the
//     bits); m 2: pi - (z - pi_lo); m 3: (z - pi_lo) - pi.
// No test for NaN or infinite operands exists, so they follow the same paths.
// Constants (bits): pi 0x40490fda (a hair below the nearest float), pi_lo 0x34222168,
// pi/2 0x3fc90fda (the compiler's value for pi/2 + pi_lo/2). Comparisons are signed.
// Nothing was executed on the EE.
extern "C" {
float atanf(float x);
float fabsf(float x);

static inline float from_bits(int bits) {
    float f;
    __builtin_memcpy(&f, &bits, 4);
    return f;
}

float __ieee754_atan2f(float y, float x) {
    const float pi = from_bits(0x40490fda);
    const float pi_lo = from_bits(0x34222168);
    const float pi_o_2 = from_bits(0x3fc90fda);
    int hx, hy;
    __builtin_memcpy(&hx, &x, 4);
    __builtin_memcpy(&hy, &y, 4);
    const int ix = hx & 0x7fffffff;
    const int iy = hy & 0x7fffffff;
    if (hx == 0x3f800000) return atanf(y);
    const int m = static_cast<int>(static_cast<unsigned>(hy) >> 31) | ((hx >> 30) & 2);
    if (iy <= 0x007fffff) {
        switch (m) {
            case 0:
            case 1:
                return y;
            case 2:
                return pi;
            default:
                return -pi;
        }
    }
    if (ix <= 0x007fffff) return hy < 0 ? -pi_o_2 : pi_o_2;
    const int k = (iy - ix) >> 23;
    float z;
    if (k > 60) {
        z = pi_o_2;
    } else if (k < -60 && hx < 0) {
        z = 0.0f;
    } else {
        z = atanf(fabsf(y / x));
    }
    switch (m) {
        case 0:
            return z;
        case 1: {
            int zb;
            __builtin_memcpy(&zb, &z, 4);
            zb ^= static_cast<int>(0x80000000u);
            return from_bits(zb);
        }
        case 2:
            return pi - (z - pi_lo);
        default:
            return (z - pi_lo) - pi;
    }
}
}
