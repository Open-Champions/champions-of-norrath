// Observed __ieee754_acosf at 0x002f4550, 1060 bytes, pinned SLUS_205.65. Arc cosine of a float,
// the fdlibm scheme with a rational approximation of the error term and __ieee754_sqrtf
// (0x002f5b10) for the outer ranges. With ix = |x| bits and hx = x bits:
//   ix == 0x3f800000 (|x| == 1): 0 for x > 0 (hx > 0), else the constant 0x40490fda (pi).
//   ix >  0x3f800000:            (x - x) / (x - x), the NaN of 0/0.
//   ix <  0x3f000000 (|x| < 0.5): ix <= 0x23000000 gives 0x3fc90fda (pi/2); otherwise
//       z = x*x; r = p(z)/q(z); result pio2_hi - (x - (pio2_lo - x*r)).
//   hx <  0 (x <= -0.5): z = (1 + x) * 0.5; s = sqrt(z); r = p(z)/q(z); w = r*s - pio2_lo;
//       result pi - 2*(s + w).
//   otherwise (x >= 0.5): z = (1 - x) * 0.5; s = sqrt(z); df = s with its low 12 bits cleared;
//       c = (z - df*df) / (s + df); r = p(z)/q(z); w = r*s + c; result 2*(df + w).
// p(z) = z*(pS0 + z*(pS1 + z*(pS2 + z*(pS3 + z*(pS4 + z*pS5))))) and
// q(z) = 1 + z*(qS1 + z*(qS2 + z*(qS3 + z*qS4))); the constants are the single-precision fdlibm
// values listed in the executable's read-only data at 0x0047d300.. (bits below).
// The two folded constants (pi + 2*pio2_lo = 0x40490fda and pio2_hi + pio2_lo = 0x3fc90fda) appear
// as literals in the code; they are written here as bit patterns instead of folded by the compiler.
// Nothing was executed on the EE.
extern "C" {
float __ieee754_sqrtf(float x);

static inline float from_bits(int bits) {
    float f;
    __builtin_memcpy(&f, &bits, 4);
    return f;
}

float __ieee754_acosf(float x) {
    const float one = from_bits(0x3f800000);
    const float pio2_hi = from_bits(0x3fc90fda);
    const float pio2_lo = from_bits(0x33a22168);
    const float pS0 = from_bits(0x3e2aaaaa);
    const float pS1 = from_bits(0xbea6b08f);
    const float pS2 = from_bits(0x3e4e0aa8);
    const float pS3 = from_bits(0xbd241146);
    const float pS4 = from_bits(0x3a4f7f04);
    const float pS5 = from_bits(0x3811ef08);
    const float qS1 = from_bits(0xc019d138);
    const float qS2 = from_bits(0x4001572c);
    const float qS3 = from_bits(0xbf303360);
    const float qS4 = from_bits(0x3d9dc62d);
    int hx;
    __builtin_memcpy(&hx, &x, 4);
    const int ix = hx & 0x7fffffff;
    if (ix == 0x3f800000) {
        if (hx > 0) return 0.0f;
        return from_bits(0x40490fda);
    }
    if (ix > 0x3f800000) {
        // Kept as a real subtraction and division, as in the original (0/0 on the FPU); the
        // empty asm statements stop the compiler folding x - x to zero.
        float copy = x;
        __asm__("" : "+f"(copy));
        float diff = x - copy;
        float divisor = diff;
        __asm__("" : "+f"(divisor));
        return diff / divisor;
    }
    float z, p, q, r;
    if (ix < 0x3f000000) {
        if (ix <= 0x23000000) return from_bits(0x3fc90fda);
        z = x * x;
        p = z * (pS0 + z * (pS1 + z * (pS2 + z * (pS3 + z * (pS4 + z * pS5)))));
        q = one + z * (qS1 + z * (qS2 + z * (qS3 + z * qS4)));
        r = p / q;
        return pio2_hi - (x - (pio2_lo - x * r));
    }
    if (hx < 0) {
        z = (one + x) * from_bits(0x3f000000);
        p = z * (pS0 + z * (pS1 + z * (pS2 + z * (pS3 + z * (pS4 + z * pS5)))));
        q = one + z * (qS1 + z * (qS2 + z * (qS3 + z * qS4)));
        const float s = __ieee754_sqrtf(z);
        r = p / q;
        const float w = r * s - pio2_lo;
        return from_bits(0x40490fda) - (s + w) * 2.0f;
    }
    z = (one - x) * from_bits(0x3f000000);
    const float s = __ieee754_sqrtf(z);
    int sbits;
    __builtin_memcpy(&sbits, &s, 4);
    sbits &= static_cast<int>(0xfffff000u);
    const float df = from_bits(sbits);
    const float c = (z - df * df) / (s + df);
    p = z * (pS0 + z * (pS1 + z * (pS2 + z * (pS3 + z * (pS4 + z * pS5)))));
    q = one + z * (qS1 + z * (qS2 + z * (qS3 + z * qS4)));
    r = p / q;
    const float w = r * s + c;
    return (df + w) * 2.0f;
}
}
