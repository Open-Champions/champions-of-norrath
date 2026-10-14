// Observed __ieee754_powf at 0x002f4f80, 2016 bytes: float power, x**y, with
// both arguments in float registers and the result in f0. The image's variant
// has no infinity or NaN handling (no test against 0x7f800000 anywhere) and
// treats exponent fields below 0x00800000 as zero. Order of tests, as read:
//  1. |y| bits < 0x00800000: return 1.
//  2. For x < 0: yisint = 0 (not an integer), 1 (odd) or 2 (even): 2 when
//     |y| bits >= 0x4b800000; for 0x3f800000 <= |y| bits, integer when the
//     bits shifted by (150 - exponent field) and back are unchanged, with the
//     low bit choosing odd or even.
//  3. |y| bits == 0x3f800000: y > 0 returns x, y < 0 returns 1/x.
//     y bits == 0x40000000 returns x*x. y bits == 0x3f000000 with x >= 0
//     tail calls __ieee754_sqrtf (0x002f5b10).
//  4. |x| bits < 0x00800000 or == 0x3f800000: z = |x|, 1/z when y < 0; for
//     negative x, z = (z-z)/(z-z) when the |x| bits equal 0x3f800000 and yisint
//     is 0, else z = -z when yisint is 1. Return z.
//  5. Negative x with yisint 0: return (x-x)/(x-x).
//  6. |y| bits > 0x4d000000 (huge y): |x| bits <= 0x3f7ffff7 returns 0x7f7fffff
//     (float max) for y < 0 and 0 otherwise; > 0x3f800007 returns 0 for y <= 0
//     and float max otherwise; between them t = x - 1 (note: the original uses
//     x, not |x|, here) and
//     w = t*t*(0.5 - t*(0.333333333 - 0.25*t)), u = t*ivln2_h, v = t*ivln2_l -
//     w*ivln2, t1 = trunc(u+v), t2 = v - (t1 - u).
//  7. Otherwise the usual log2 split: mantissa m = |x| bits & 0x7fffff,
//     n = exponent - 127, k = 0; m > 0x1cc471 gives k = 1, and m > 0x5db3d6
//     instead gives k = 0 with n one higher and the exponent field dropped by
//     one. Reduction with bp[k], s = (ax-bp)/(ax+bp), a series in s*s
//     (L1..L6), then the scaling by cp, cp_h, cp_l and dp_h[k], dp_l[k] to
//     t1 (high 20 bits) and t2.
//  8. Common tail: s = -1 when x < 0 and yisint is 1 else 1; y1 = trunc(y);
//     p_l = (y-y1)*t1 + y*t2, p_h = y1*t1, z = p_l + p_h. Bits of z at
//     0x43010000 and above (with the ovt test at equality) return s*1e30*1e30;
//     for negative z, |bits| above 0x42fc0000 (with p_l <= z - p_h at
//     equality) returns s*1e-30*1e-30. n and the rounded t are split off when
//     |bits| > 0x3f000000, then 2**z is built from a series (P1..P5) and
//     scaled by n through the exponent field or by scalbnf (0x002f8070) when
//     the biased exponent would not stay positive. Result s*z.
// The constants are the 32-bit patterns used in the instruction stream, and
// the tables bp, dp_h, dp_l are three pairs of words read from the image
// (0x0047d3b0, 0x0047d3b8, 0x0047d3c0). Operation order is kept as read.
// Inferred: this is the usual fdlibm single-float pow; the names are mine.
// Unknown: the owners of the neighbouring constants.
extern "C" {
float fabsf(float);
float __ieee754_sqrtf(float);
float scalbnf(float, int);
}

namespace {
constexpr float from_bits(unsigned bits) { return __builtin_bit_cast(float, bits); }
constexpr unsigned to_bits(float value) { return __builtin_bit_cast(unsigned, value); }
float trunc_low12(float value) { return from_bits(to_bits(value) & 0xfffff000u); }

// The image divides the zero difference by itself at run time. The EE float
// model lets the compiler fold d / d to 1, so the empty asm statements hide
// that both operands are the same value.
float zero_over_zero(float d) {
    float divisor = d;
#ifdef __mips__
    asm("" : "+f"(d));
    asm("" : "+f"(divisor));
#endif
    return d / divisor;
}

constexpr float bp[2] = {from_bits(0x3f800000u), from_bits(0x3fc00000u)};
constexpr float dp_h[2] = {from_bits(0x00000000u), from_bits(0x3f15c000u)};
constexpr float dp_l[2] = {from_bits(0x00000000u), from_bits(0x35d1cfdcu)};
}

extern "C" float __ieee754_powf(float x, float y) {
    const float one = 1.0f;
    const float two = 2.0f;
    const float three = 3.0f;
    const float huge = from_bits(0x7149f2c9u);
    const float tiny = from_bits(0x0da24260u);
    const float float_max = from_bits(0x7f7fffffu);

    const int hx = static_cast<int>(to_bits(x));
    const int hy = static_cast<int>(to_bits(y));
    const int ix = hx & 0x7fffffff;
    const int iy = hy & 0x7fffffff;

    if (iy < 0x00800000) return one;

    int yisint = 0;
    if (hx < 0) {
        if (iy >= 0x4b800000) {
            yisint = 2;
        } else if (iy >= 0x3f800000) {
            const int shift = 150 - (iy >> 23);
            const int j = iy >> shift;
            if ((j << shift) == iy) yisint = 2 - (j & 1);
        }
    }

    if (iy == 0x3f800000) {
        if (hy >= 0) return x;
        return one / x;
    }
    if (hy == 0x40000000) return x * x;
    if (hy == 0x3f000000 && hx >= 0) return __ieee754_sqrtf(x);

    const float ax = fabsf(x);

    if (ix < 0x00800000 || ix == 0x3f800000) {
        float z = ax;
        if (hy < 0) z = one / z;
        if (hx < 0) {
            if (((ix - 0x3f800000) | yisint) == 0) {
                z = zero_over_zero(z - z);
            } else if (yisint == 1) {
                z = -z;
            }
        }
        return z;
    }

    const int negative = static_cast<int>(static_cast<unsigned>(hx) >> 31);
    if (((negative - 1) | yisint) == 0) {
        return zero_over_zero(x - x);
    }

    float t1;
    float t2;
    int n = 0;
    if (iy > 0x4d000000) {
        if (ix <= 0x3f7ffff7) return hy < 0 ? float_max : 0.0f;
        if (ix > 0x3f800007) return hy > 0 ? float_max : 0.0f;
        const float t = x - one;
        const float w = (t * t) * (from_bits(0x3f000000u) - t * (from_bits(0x3eaaaaaau) - t * from_bits(0x3e800000u)));
        const float u = t * from_bits(0x3fb8aa00u);
        const float v = t * from_bits(0x36eca56fu) - w * from_bits(0x3fb8aa3au);
        t1 = trunc_low12(u + v);
        t2 = v - (t1 - u);
    } else {
        const int mantissa = ix & 0x007fffff;
        int exponent_bits = mantissa | 0x3f800000;
        int k = 0;
        n = (ix >> 23) - 127;
        if (mantissa > 0x001cc471) {
            k = 1;
            if (mantissa > 0x005db3d6) {
                k = 0;
                n = (ix >> 23) - 126;
                exponent_bits += static_cast<int>(0xff800000u);
            }
        }
        const float ax2 = from_bits(static_cast<unsigned>(exponent_bits));
        const float u = ax2 - bp[k];
        const float v = one / (ax2 + bp[k]);
        const float s = u * v;
        const float s_h = trunc_low12(s);
        const float t_h0 = from_bits(static_cast<unsigned>((((exponent_bits >> 1) | 0x20000000) + (k << 21)) + 0x00040000));
        const float t_l0 = ax2 - (t_h0 - bp[k]);
        const float s_l = v * ((u - s_h * t_h0) - s_h * t_l0);
        float s2 = s * s;
        float r = s2 * s2 *
                  (from_bits(0x3f199999u) +
                   s2 * (from_bits(0x3edb6db7u) +
                         s2 * (from_bits(0x3eaaaaabu) +
                               s2 * (from_bits(0x3e8ba304u) + s2 * (from_bits(0x3e6c3254u) + s2 * from_bits(0x3e53f142u))))));
        r += s_l * (s_h + s);
        s2 = s_h * s_h;
        const float t_h = trunc_low12(three + s2 + r);
        const float t_l = r - ((t_h - three) - s2);
        const float u2 = s_h * t_h;
        const float v2 = s_l * t_h + t_l * s;
        const float p_h = trunc_low12(u2 + v2);
        const float p_l = v2 - (p_h - u2);
        const float z_h = from_bits(0x3f763800u) * p_h;
        const float z_l = from_bits(0x369dc39fu) * p_h + p_l * from_bits(0x3f76384eu) + dp_l[k];
        const float tn = static_cast<float>(n);
        t1 = trunc_low12(((z_h + z_l) + dp_h[k]) + tn);
        t2 = z_l - (((t1 - tn) - dp_h[k]) - z_h);
    }

    const float sign = (((negative - 1) | (yisint - 1)) == 0) ? -one : one;
    const float y1 = trunc_low12(y);
    const float p_l = (y - y1) * t1 + y * t2;
    float p_h = y1 * t1;
    const float z0 = p_l + p_h;
    const int j = static_cast<int>(to_bits(z0));
    const int magnitude = j & 0x7fffffff;
    if (j > 0) {
        if (magnitude > 0x43010000) return sign * huge * huge;
        if (magnitude == 0x43010000 && z0 - p_h < p_l + from_bits(0x3338aa3bu)) return sign * huge * huge;
    } else {
        if (magnitude > 0x42fc0000) return sign * tiny * tiny;
        if (magnitude == 0x42fc0000 && p_l <= z0 - p_h) return sign * tiny * tiny;
    }

    n = 0;
    if (magnitude > 0x3f000000) {
        const int k = (magnitude >> 23) - 126;
        const int sum = j + (0x00800000 >> k);
        const int e = static_cast<int>((static_cast<unsigned>(sum) >> 23) & 0xff) - 127;
        const float t = from_bits(static_cast<unsigned>(sum & ~(0x007fffff >> e)));
        n = ((sum & 0x007fffff) | 0x00800000) >> (23 - e);
        if (j < 0) n = -n;
        p_h -= t;
    }
    const float t = trunc_low12(p_l + p_h);
    const float u = t * from_bits(0x3f317200u);
    const float v = (p_l - (t - p_h)) * from_bits(0x3f317217u) + t * from_bits(0x35bfbe8cu);
    const float z = u + v;
    const float w = v - (z - u);
    const float zz = z * z;
    const float t1b = z - zz * (from_bits(0x3e2aaaaau) +
                                zz * (from_bits(0xbb360b60u) +
                                      zz * (from_bits(0x388ab354u) + zz * (from_bits(0xb5ddea0eu) + zz * from_bits(0x3331bb4bu)))));
    const float r = (z * t1b) / (t1b - two) - (w + z * w);
    float result = one - (r - z);
    const int bits = static_cast<int>(to_bits(result)) + (n << 23);
    if ((bits >> 23) > 0) {
        result = from_bits(static_cast<unsigned>(bits));
    } else {
        result = scalbnf(result, n);
    }
    return sign * result;
}
