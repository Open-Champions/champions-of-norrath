// Observed scalbn at 0x002f7e88, 484 bytes, pinned SLUS_205.65: x * 2^n by exponent-field arithmetic, of the
// usual fdlibm kind (the image's copy carries the Sun Microsystems notice that permits free use). A double is
// one 64-bit value; hi is its signed high word. The calls it makes are dpmul (0x002fb680), dpadd (0x002fb5c0)
// and copysign (0x002f8138). The constants are the doubles at 0x0047dae8 (1.0e-300, 'tiny') and 0x0047daf0
// (1.0e300 as the image holds it, 'huge'), 2^54 and 2^-54 as immediates.
//   * k = the exponent field. A zero field: a zero magnitude returns x; a subnormal is multiplied by 2^54, k is
//     re-read from the product and lowered by 54, and n < -50000 returns dpmul(product, tiny).
//   * A full exponent (infinity or NaN) returns dpadd(x, x) of the (possibly scaled) value.
//   * k += n. Above 2046: dpmul(copysign(huge, x), huge). In 1..2046 the exponent field of the high word is
//     replaced and the result returned with no further call.
//   * k <= -54: n > 50000 gives the overflow product above, anything else dpmul(copysign(tiny, x), tiny).
//   * Otherwise k += 54, the exponent field is replaced and the result is multiplied by 2^-54.
// The high word is rebuilt as (hi & 0x800fffff) | (k << 20) with hi the word read after any 2^54 scaling.
// Unknown: the callee bodies. Nothing was executed on the EE.
extern "C" {
typedef unsigned long long Dp;
Dp dpmul(Dp left, Dp right);
Dp dpadd(Dp left, Dp right);
Dp copysign(Dp magnitude, Dp sign_source);

static const Dp kTwo54 = 0x4350000000000000ull;
static const Dp kTwoMinus54 = 0x3c90000000000000ull;
static const Dp kTiny = 0x01a56e1fc2f8f358ull;
static const Dp kHuge = 0x7e37e43c8800759bull;

Dp scalbn(Dp x, int n) {
    int hi = static_cast<int>(x >> 32);
    const unsigned lo = static_cast<unsigned>(x);
    int k = (hi & 0x7ff00000) >> 20;
    if (k == 0) {
        if ((lo | (static_cast<unsigned>(hi) & 0x7fffffffu)) == 0) return x;
        x = dpmul(x, kTwo54);
        hi = static_cast<int>(x >> 32);
        k = ((hi & 0x7ff00000) >> 20) - 54;
        if (n < -50000) return dpmul(x, kTiny);
    }
    if (k == 0x7ff) return dpadd(x, x);
    k = k + n;
    if (k > 0x7fe) return dpmul(copysign(kHuge, x), kHuge);
    if (k > 0) {
        const unsigned high = (static_cast<unsigned>(hi) & 0x800fffffu) | (static_cast<unsigned>(k) << 20);
        return (x & 0xffffffffull) | (static_cast<Dp>(high) << 32);
    }
    if (k <= -54) {
        if (n > 50000) return dpmul(copysign(kHuge, x), kHuge);
        return dpmul(copysign(kTiny, x), kTiny);
    }
    k += 54;
    const unsigned high = (static_cast<unsigned>(hi) & 0x800fffffu) | (static_cast<unsigned>(k) << 20);
    x = (x & 0xffffffffull) | (static_cast<Dp>(high) << 32);
    return dpmul(x, kTwoMinus54);
}
}
