// Observed __ieee754_sqrtf at 0x002f5b10, 204 bytes. Single-precision square root done with
// integer arithmetic on the bit pattern, one result bit per loop pass, with no hardware sqrt.
//   |x| bits <= 0x007fffff (zero and subnormals) : x is returned unchanged, so -0 stays -0.
//   sign bit set (otherwise)                     : (x - x) / (x - x), which is 0/0.
//   everything else, including exponent 255,
//   is treated as an ordinary number:
//     e = exponent field - 127; the 23 fraction bits get the leading bit 0x800000, are shifted
//     left by (e & 1) and then by one more place; e is halved (arithmetic shift).
//     Then 24 passes with r from 0x1000000 down by halves, s = 0 and q = 0:
//       t = s + r; if (t <= m) { m -= t; s = t + r; q += r; }  r >>= 1;  m <<= 1;
//     A nonzero leftover m adds the low bit of q to q (round to nearest even approximation).
//     The result bits are (q >> 1) + 0x3f000000 + (halved e << 23).
// The 0/0 is a real division on the hardware (its result is whatever the FPU gives for it).
// Comparisons are signed, as observed. Nothing was executed on the EE.
extern "C" float __ieee754_sqrtf(float x) {
    int ix;
    __builtin_memcpy(&ix, &x, 4);
    if (static_cast<unsigned>(ix & 0x7fffffff) <= 0x7fffffu) return x;
    if (ix < 0) {
        // The empty asm statements stop the compiler folding x - x and num / den to constants;
        // the original performs the subtraction and the division.
        float copy = x;
        __asm__("" : "+f"(copy));
        float num = x - copy;
        float den = num;
        __asm__("" : "+f"(den));
        return num / den;
    }
    int e = (ix >> 23) - 127;
    int m = (ix & 0x7fffff) | 0x800000;
    m <<= (e & 1);
    m <<= 1;
    e >>= 1;
    int s = 0;
    int q = 0;
    int r = 0x1000000;
    do {
        int t = s + r;
        if (t <= m) {
            m -= t;
            s = t + r;
            q += r;
        }
        r >>= 1;
        m <<= 1;
    } while (r != 0);
    if (m != 0) q += q & 1;
    int bits = (q >> 1) + 0x3f000000 + (e << 23);
    float result;
    __builtin_memcpy(&result, &bits, 4);
    return result;
}
