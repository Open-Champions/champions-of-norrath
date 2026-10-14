// Observed __kernel_tanf at 0x002f7700, 644 bytes: float tangent kernel,
// __kernel_tanf(x, y, iy), with x the (reduced) argument, y its low part and
// iy either 1 (tangent) or -1 (negative reciprocal, used for odd quadrants).
// Reading of the instruction flow, in order:
//  1. Let hx be the raw bits of x and ix = hx & 0x7fffffff.
//  2. ix < 0x31800000 (|x| < 2^-28): when (int)x is zero, return 1/|x| if
//     (ix | (iy+1)) is zero, else x for iy == 1 and -1/x otherwise. When (int)x
//     is not zero the argument falls through to the polynomial.
//  3. ix >= 0x3f2ca140 (|x| near pi/4 or more): fold with x = (pi/4 - |x|) +
//     (pi/4_lo - |y|) with the sign of hx applied to x and y first, and y = 0.
//     Constants: 0x3f490fda and 0x33222167.
//  4. z = x*x, w = z*z, s = z*x. Two polynomials in w over the 13-word table T
//     (0x0047d9e0): r = T1 + w*(T3 + w*(T5 + w*(T7 + w*(T9 + w*T11)))) and
//     v = z*(T2 + w*(T4 + w*(T6 + w*(T8 + w*(T10 + w*T12))))).
//     Then r = y + z*(s*(r + v) + y); r += T0*s; w = x + r.
//  5. Folded case: v = (float)iy, result = (float)(1 - ((hx>>30) & 2)) *
//     (v - 2*(x - (w*w/(w+v) - r))).
//  6. Otherwise iy == 1 returns w. Else with t, a = -1/w and the top 20 bits
//     of w and of a kept (low 12 bits cleared): z = trunc(w), v = r - (z - x),
//     t = trunc(a), s = 1 + t*z, result = t + a*(s + t*v).
// The table values are the 13 words as found in the image (hex below). The
// operation order is kept as read; every step is a separate single-float
// operation. Inferred: this is the usual fdlibm/Newlib single-float tangent
// kernel; the names above are mine. Unknown: where T's neighbours belong.
// Callee fabsf (0x002f1368) is only used for the 1/|x| result.
extern "C" float fabsf(float);

namespace {
float from_bits(unsigned bits) { return __builtin_bit_cast(float, bits); }
unsigned to_bits(float value) { return __builtin_bit_cast(unsigned, value); }

const float T[13] = {
    __builtin_bit_cast(float, 0x3eaaaaabu), __builtin_bit_cast(float, 0x3e088889u),
    __builtin_bit_cast(float, 0x3d5d0dd1u), __builtin_bit_cast(float, 0x3cb327a3u),
    __builtin_bit_cast(float, 0x3c11371eu), __builtin_bit_cast(float, 0x3b6b6916u),
    __builtin_bit_cast(float, 0x3abede47u), __builtin_bit_cast(float, 0x3a1a26c7u),
    __builtin_bit_cast(float, 0x398137b8u), __builtin_bit_cast(float, 0x38a3f445u),
    __builtin_bit_cast(float, 0x3895c07au), __builtin_bit_cast(float, 0xb79bae5eu),
    __builtin_bit_cast(float, 0x37d95384u),
};
}

extern "C" float __kernel_tanf(float x, float y, int iy) {
    const int hx = static_cast<int>(to_bits(x));
    const int ix = hx & 0x7fffffff;
    if (ix < 0x31800000) {
        if (static_cast<int>(x) == 0) {
            if ((ix | (iy + 1)) == 0) return 1.0f / fabsf(x);
            if (iy == 1) return x;
            return -1.0f / x;
        }
    }
    const bool folded = ix >= 0x3f2ca140;
    if (folded) {
        if (hx < 0) {
            x = -x;
            y = -y;
        }
        const float high = from_bits(0x3f490fdau) - x;
        const float low = from_bits(0x33222167u) - y;
        x = high + low;
        y = 0.0f;
    }
    const float z = x * x;
    const float w = z * z;
    const float s = z * x;
    const float r_poly = T[1] + w * (T[3] + w * (T[5] + w * (T[7] + w * (T[9] + w * T[11]))));
    const float v_poly = z * (T[2] + w * (T[4] + w * (T[6] + w * (T[8] + w * (T[10] + w * T[12])))));
    float r = y + z * (s * (r_poly + v_poly) + y);
    r += T[0] * s;
    const float sum = x + r;
    if (folded) {
        const float v = static_cast<float>(iy);
        const float sign = static_cast<float>(1 - ((hx >> 30) & 2));
        return sign * (v - 2.0f * (x - (sum * sum / (sum + v) - r)));
    }
    if (iy == 1) return sum;
    const float head = from_bits(to_bits(sum) & 0xfffff000u);
    const float v = r - (head - x);
    const float a = -1.0f / sum;
    const float t = from_bits(to_bits(a) & 0xfffff000u);
    const float s2 = 1.0f + t * head;
    return t + a * (s2 + t * v);
}
