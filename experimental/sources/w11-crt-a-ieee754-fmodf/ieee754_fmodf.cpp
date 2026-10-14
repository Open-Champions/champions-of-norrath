// Observed __ieee754_fmodf at 0x002f4db0, 464 bytes, pinned SLUS_205.65. Remainder of x divided by y
// with the sign of x, computed on the bit patterns by repeated shift and subtract (the
// fdlibm scheme). Differences from the usual published routine, all taken from the code:
//   - Only y with |y| bits <= 0x007fffff (zero or subnormal) is rejected, by returning
//     (x * y) / (x * y). An infinite or NaN x, or a NaN y, is not tested for and goes through
//     the ordinary path with the exponent field 255 treated as a number (the paired runs gave
//     finite garbage for an infinite x, not a NaN).
//   - |x| < |y| (by bits) returns x; equal magnitudes return a signed zero from the two-word
//     table at 0x0047d3a8 ({+0, -0}) selected by the sign of x.
//   - Exponents are read as (bits >> 23) - 127 without a subnormal count, so a subnormal x counts
//     as -127 and is shifted left by one; the same code exists for y. Neither shift can run:
//     y subnormal was rejected above and a subnormal x is always smaller than a normal y, so
//     the function has already returned x.
//   - After the shift-subtract loop, a zero remainder returns the signed zero; otherwise the
//     result is renormalised (exponent lowered per shift) and packed, or shifted right
//     (arithmetic) into a subnormal. The 'create the underflow signal' multiply of the
//     published routine is absent.
// The result is also left in the float result register; nothing else is written.
// Nothing was executed on the EE.
extern "C" float __ieee754_fmodf(float x, float y) {
    static const float Zero[2] = {0.0f, -0.0f};
    int hx, hy;
    __builtin_memcpy(&hx, &x, 4);
    __builtin_memcpy(&hy, &y, 4);
    const int sx = hx & static_cast<int>(0x80000000u);
    hx ^= sx;
    hy &= 0x7fffffff;
    if (hy <= 0x007fffff) {
        // The empty asm statements keep the compiler from folding the self-division; the
        // original multiplies and then divides the product by itself.
        float product = x * y;
        float divisor = product;
        __asm__("" : "+f"(divisor));
        return product / divisor;
    }
    if (hx < hy) return x;
    if (hx == hy) return Zero[static_cast<unsigned>(sx) >> 31];
    const int ix = (hx >> 23) - 127;
    int iy = (hy >> 23) - 127;
    const bool x_sub = ix < -126;
    const bool y_sub = iy < -126;
    if (!x_sub) hx = (hx & 0x007fffff) | 0x00800000;
    else hx <<= (-126 - ix);
    if (!y_sub) hy = (hy & 0x007fffff) | 0x00800000;
    else hy <<= (-126 - iy);
    int n = ix - iy;
    while (n-- != 0) {
        const int hz = hx - hy;
        if (hz < 0) {
            hx = hx + hx;
        } else {
            if (hz == 0) return Zero[static_cast<unsigned>(sx) >> 31];
            hx = hz + hz;
        }
    }
    const int hz = hx - hy;
    if (hz >= 0) hx = hz;
    if (hx == 0) return Zero[static_cast<unsigned>(sx) >> 31];
    bool out_sub = y_sub;
    if (hx <= 0x007fffff) {
        do {
            hx = hx + hx;
            iy -= 1;
        } while (hx <= 0x007fffff);
        out_sub = iy < -126;
    }
    int bits;
    if (!out_sub) {
        bits = ((hx - 0x00800000) | ((iy + 127) << 23)) | sx;
    } else {
        hx >>= (-126 - iy);
        bits = hx | sx;
    }
    float result;
    __builtin_memcpy(&result, &bits, 4);
    return result;
}
