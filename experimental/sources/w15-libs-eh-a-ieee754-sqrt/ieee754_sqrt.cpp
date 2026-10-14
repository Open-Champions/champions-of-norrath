// Observed __ieee754_sqrt at 0x002f4230, 796 bytes, pinned SLUS_205.65: the integer, bit-by-bit square root of
// the usual fdlibm kind (the image's copy carries the Sun Microsystems notice that permits free use). A double
// is one 64-bit value; hi is its signed high word and lo its low word. The calls it makes are dpmul
// (0x002fb680), dpadd (0x002fb5c0), dpsub (0x002fb618), dpdiv (0x002fb908) and dpcmp (0x002fba48).
//   * A full exponent (infinity or NaN) returns dpadd(dpmul(x, x), x).
//   * hi <= 0: a zero magnitude returns x as given; a negative x returns dpdiv(d, d) with d = dpsub(x, x), the
//     difference taken once.
//   * A zero exponent field (subnormal) is normalised: while hi is zero, 21 is taken off the exponent and hi
//     takes lo >> 11 while lo shifts left 21; then hi shifts left until bit 20 is set (i steps), the exponent
//     loses i - 1, and lo >> (32 - i) is or-ed into hi (the variable shift uses its low five bits, so i == 0
//     shifts by zero, as the generated code does) and lo shifts left i.
//   * The exponent is unbiased (minus 1023) and the 21-bit mantissa gets its implicit bit; an odd exponent
//     doubles the two-word mantissa. The mantissa is doubled once more, then the root is built a bit at a time:
//     the high part (bit r from 0x00200000 down) and then the low part (bit r from 0x80000000 down), keeping a
//     remainder in (hi, lo), with a carry fix-up into the high root word when the low trial sum wraps.
//   * A nonzero remainder selects the rounding. Two compares dpcmp(1.0, 1.0) stand in for the "one minus tiny"
//     and "one plus tiny" probes (the image has the sums folded to 1.0, so both compare equal): the first
//     (>= 0 holds) lets a low root of all ones wrap to zero with a carry into the high word; otherwise the second
//     (> 0 does not hold) adds the low bit of the low root, i.e. rounds to even.
//   * The result words are hi = (q >> 1) + 0x3fe00000 + (m << 20) and lo = (q1 >> 1) | (q odd ? 0x80000000 : 0).
// The rounding branch with a positive second compare (adds two to the low root) is not reachable with the
// compare results of the image's dpcmp(1.0, 1.0) and is kept for the shape the generated code has.
// Unknown: the callee bodies. Nothing was executed on the EE.
extern "C" {
typedef unsigned long long Dp;
Dp dpmul(Dp left, Dp right);
Dp dpadd(Dp left, Dp right);
Dp dpsub(Dp left, Dp right);
Dp dpdiv(Dp left, Dp right);
int dpcmp(Dp left, Dp right);

static const Dp kOne = 0x3ff0000000000000ull;

Dp __ieee754_sqrt(Dp x) {
    const unsigned sign = 0x80000000u;
    int hi = static_cast<int>(x >> 32);
    unsigned lo = static_cast<unsigned>(x);

    if ((hi & 0x7ff00000) == 0x7ff00000) return dpadd(dpmul(x, x), x);
    if (hi <= 0) {
        if (((hi & 0x7fffffff) | static_cast<int>(lo)) == 0) return x;
        if (hi < 0) {
            const Dp difference = dpsub(x, x);
            return dpdiv(difference, difference);
        }
    }

    int m = hi >> 20;
    if (m == 0) {
        while (hi == 0) {
            m -= 21;
            hi |= static_cast<int>(lo >> 11);
            lo <<= 21;
        }
        int i = 0;
        while ((hi & 0x00100000) == 0) {
            hi <<= 1;
            ++i;
        }
        m -= i - 1;
        hi |= static_cast<int>(lo >> ((32 - i) & 31));
        lo <<= i;
    }
    m -= 1023;
    hi = (hi & 0x000fffff) | 0x00100000;
    if (m & 1) {
        hi += hi + static_cast<int>((lo & sign) >> 31);
        lo += lo;
    }
    m >>= 1;
    hi += hi + static_cast<int>((lo & sign) >> 31);
    lo += lo;

    int q = 0;
    unsigned q1 = 0;
    int s0 = 0;
    unsigned s1 = 0;
    unsigned r = 0x00200000u;
    while (r != 0) {
        const int t = s0 + static_cast<int>(r);
        if (t <= hi) {
            s0 = t + static_cast<int>(r);
            hi -= t;
            q += static_cast<int>(r);
        }
        hi += hi + static_cast<int>((lo & sign) >> 31);
        lo += lo;
        r >>= 1;
    }
    r = sign;
    while (r != 0) {
        const unsigned t1 = s1 + r;
        const int t = s0;
        if (t < hi || (t == hi && t1 <= lo)) {
            s1 = t1 + r;
            if ((t1 & sign) == sign && (s1 & sign) == 0) s0 += 1;
            hi -= t;
            if (lo < t1) hi -= 1;
            lo -= t1;
            q1 += r;
        }
        hi += hi + static_cast<int>((lo & sign) >> 31);
        lo += lo;
        r >>= 1;
    }

    if ((hi | static_cast<int>(lo)) != 0) {
        if (dpcmp(kOne, kOne) >= 0) {
            if (q1 == 0xffffffffu) {
                q1 = 0;
                q += 1;
            } else if (dpcmp(kOne, kOne) > 0) {
                if (q1 == 0xfffffffeu) q += 1;
                q1 += 2;
            } else {
                q1 += q1 & 1;
            }
        }
    }
    const int out_hi = (q >> 1) + 0x3fe00000 + (m << 20);
    unsigned out_lo = q1 >> 1;
    if (q & 1) out_lo |= sign;
    return (static_cast<Dp>(static_cast<unsigned>(out_hi)) << 32) | out_lo;
}
}
