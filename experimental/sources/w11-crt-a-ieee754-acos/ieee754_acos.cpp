// Observed __ieee754_acos at 0x002f16e8, 1828 bytes, pinned SLUS_205.65. Double-precision arc
// cosine in the fdlibm scheme, written as the sequence of soft-double calls the original makes
// (doubles travel as 64-bit values in one register): dpadd (0x002fb5c0), dpsub (0x002fb618),
// dpmul (0x002fb680), dpdiv (0x002fb908) and __ieee754_sqrt (0x002f4230). With hx the high word of x
// and ix = hx & 0x7fffffff:
//   ix >= 0x3ff00000: if ix is exactly 0x3ff00000 and the low word is zero (|x| == 1) the result is
//       0.0 for hx > 0 and the constant pi + 2*pio2_lo (0x400921fb54442d18) otherwise; any other
//       |x| >= 1 gives dpdiv(dpsub(x, x), dpsub(x, x)) (the NaN of 0/0; the subtraction is made once).
//   ix <= 0x3c600000 (|x| < 2^-57): the constant 0x3ff921fb54442d18 (pio2_hi + pio2_lo).
//   ix <  0x3fe00000: z = x*x; p, q from the two polynomials below; r = p/q;
//       result pio2_hi - (x - (pio2_lo - x*r)).
//   hx <  0 (x <= -0.5): z = (1 + x) * 0.5; p, q; s = sqrt(z); r = p/q; w = r*s - pio2_lo;
//       result pi - 2*(s + w) (the doubling is dpadd(t, t)).
//   otherwise: z = (1 - x) * 0.5; s = sqrt(z); df = s with its low word cleared;
//       c = (z - df*df) / (s + df); p, q; r = p/q; w = r*s + c; result 2*(df + w).
// p = z*(pS0 + z*(pS1 + z*(pS2 + z*(pS3 + z*(pS4 + z*pS5))))) and
// q = 1 + z*(qS1 + z*(qS2 + z*(qS3 + z*qS4))) are built as a chain of dpmul/dpadd calls, the p chain
// first and then the q chain; each step multiplies z by the running value and adds the next
// coefficient, with the product as the first argument of the addition. The coefficients are the
// fdlibm double values; the executable keeps three identical copies, from 0x0047cbd8, 0x0047cc38 and
// 0x0047cc98, one per branch. This unit uses one set, given as bit patterns.
// Unknown: the bodies of the five callees. Nothing was executed on the EE.
extern "C" {
typedef unsigned long long Dp;
Dp dpadd(Dp left, Dp right);
Dp dpsub(Dp left, Dp right);
Dp dpmul(Dp left, Dp right);
Dp dpdiv(Dp left, Dp right);
Dp __ieee754_sqrt(Dp value);

static const Dp kOne = 0x3ff0000000000000ull;
static const Dp kHalf = 0x3fe0000000000000ull;
static const Dp kPio2Hi = 0x3ff921fb54442d18ull;   // also the folded pio2_hi + pio2_lo
static const Dp kPio2Lo = 0x3c91a62633145c07ull;
static const Dp kPi = 0x400921fb54442d18ull;       // also the folded pi + 2*pio2_lo
static const Dp kPS0 = 0x3fc5555555555555ull;
static const Dp kPS1 = 0xbfd4d61203eb6f7cull;
static const Dp kPS2 = 0x3fc9c1550e884454ull;
static const Dp kPS3 = 0xbfa48228b5688f3bull;
static const Dp kPS4 = 0x3f49efe07501b288ull;
static const Dp kPS5 = 0x3f023de10dfdf709ull;
static const Dp kQS1 = 0xc0033a271c8a2d4aull;
static const Dp kQS2 = 0x40002ae59c598ac7ull;
static const Dp kQS3 = 0xbfe6066c1b8d0158ull;
static const Dp kQS4 = 0x3fb3b8c5b12e9281ull;

// p(z) first, then q(z), in the order the original makes the calls.
static void rational_parts(Dp z, Dp* p, Dp* q) {
    Dp t = dpmul(z, kPS5);
    t = dpadd(t, kPS4);
    t = dpmul(z, t);
    t = dpadd(t, kPS3);
    t = dpmul(z, t);
    t = dpadd(t, kPS2);
    t = dpmul(z, t);
    t = dpadd(t, kPS1);
    t = dpmul(z, t);
    t = dpadd(t, kPS0);
    *p = dpmul(z, t);
    t = dpmul(z, kQS4);
    t = dpadd(t, kQS3);
    t = dpmul(z, t);
    t = dpadd(t, kQS2);
    t = dpmul(z, t);
    t = dpadd(t, kQS1);
    t = dpmul(z, t);
    *q = dpadd(t, kOne);
}

Dp __ieee754_acos(Dp x) {
    const int hx = static_cast<int>(x >> 32);
    const int ix = hx & 0x7fffffff;
    if (ix > 0x3fefffff) {
        if (static_cast<unsigned>((ix + static_cast<int>(0xc0100000u)) | static_cast<int>(x)) == 0) {
            if (hx > 0) return 0;
            return kPi;
        }
        const Dp diff = dpsub(x, x);
        return dpdiv(diff, diff);
    }
    if (ix <= 0x3fdfffff) {
        if (ix <= 0x3c600000) return kPio2Hi;
        const Dp z = dpmul(x, x);
        Dp p, q;
        rational_parts(z, &p, &q);
        const Dp r = dpdiv(p, q);
        Dp t = dpmul(x, r);
        t = dpsub(kPio2Lo, t);
        t = dpsub(x, t);
        return dpsub(kPio2Hi, t);
    }
    if (hx < 0) {
        const Dp z = dpmul(dpadd(x, kOne), kHalf);
        Dp p, q;
        rational_parts(z, &p, &q);
        const Dp s = __ieee754_sqrt(z);
        const Dp r = dpdiv(p, q);
        Dp w = dpmul(r, s);
        w = dpsub(w, kPio2Lo);
        Dp t = dpadd(s, w);
        t = dpadd(t, t);
        return dpsub(kPi, t);
    }
    const Dp z = dpmul(dpsub(kOne, x), kHalf);
    const Dp s = __ieee754_sqrt(z);
    const Dp df = s & 0xffffffff00000000ull;
    Dp c = dpmul(df, df);
    c = dpsub(z, c);
    c = dpdiv(c, dpadd(s, df));
    Dp p, q;
    rational_parts(z, &p, &q);
    const Dp r = dpdiv(p, q);
    Dp w = dpmul(r, s);
    w = dpadd(w, c);
    Dp t = dpadd(df, w);
    return dpadd(t, t);
}
}
