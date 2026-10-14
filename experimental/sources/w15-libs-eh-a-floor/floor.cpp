// Observed floor at 0x002f0b98, 532 bytes, pinned SLUS_205.65: the double-precision floor of the usual
// fdlibm word-manipulation kind (the image's copy carries the Sun Microsystems notice that permits free use).
// A double is one 64-bit value; the image adds with dpadd (0x002fb5c0) and compares with dpcmp (0x002fba48).
// With hi the signed high word, lo the low word and e = ((hi >> 20) & 0x7ff) - 1023:
//   e < 20, e < 0 (|x| < 1): when huge + x compares above zero (the inexact probe, huge being the double at
//       0x0047cae8) a non-negative x gives +0.0 and a negative x with any nonzero magnitude gives -1.0
//       (high word 0xbff00000); a negative zero stays as it is.
//   e < 20, e >= 0: mask = 0x000fffff >> e. If no fraction bit is set (hi & mask and lo are zero) x is returned
//       as given. Otherwise, after the same probe, a negative x adds 0x00100000 >> e to hi, then hi &= ~mask
//       and lo = 0.
//   20 <= e <= 51: mask = 0xffffffff >> (e - 20). No fraction bit in lo returns x. Otherwise, after the probe,
//       a negative x adds one to hi when e == 20, or adds 1 << (52 - e) to lo with a carry into hi; then
//       lo &= ~mask.
//   e > 51: e == 1024 (infinity or NaN) returns x + x; any other x is returned unchanged.
// When the probe does not exceed zero the words are rebuilt from the unchanged hi and lo.
// Constant 'huge' is the observed bit pattern 0x7e37e43c8800759b. Nothing was executed on the EE.
extern "C" {
typedef unsigned long long Dp;
Dp dpadd(Dp left, Dp right);
int dpcmp(Dp left, Dp right);

static const Dp kHuge = 0x7e37e43c8800759bull;

static bool probe_inexact(Dp x) { return dpcmp(dpadd(x, kHuge), 0ull) > 0; }

Dp floor(Dp x) {
    int hi = static_cast<int>(x >> 32);
    unsigned lo = static_cast<unsigned>(x);
    const int e = ((hi >> 20) & 0x7ff) - 1023;
    if (e < 20) {
        if (e < 0) {
            if (probe_inexact(x)) {
                if (hi >= 0) {
                    hi = 0;
                    lo = 0;
                } else if (((hi & 0x7fffffff) | static_cast<int>(lo)) != 0) {
                    hi = static_cast<int>(0xbff00000u);
                    lo = 0;
                }
            }
        } else {
            const int mask = 0x000fffff >> e;
            if (((hi & mask) | static_cast<int>(lo)) == 0) return x;
            if (probe_inexact(x)) {
                if (hi < 0) hi += 0x00100000 >> e;
                hi &= ~mask;
                lo = 0;
            }
        }
    } else if (e > 51) {
        if (e == 1024) return dpadd(x, x);
        return x;
    } else {
        const unsigned mask = 0xffffffffu >> (e - 20);
        if ((lo & mask) == 0) return x;
        if (probe_inexact(x)) {
            if (hi < 0) {
                if (e == 20) {
                    hi += 1;
                } else {
                    const unsigned sum = lo + (1u << (52 - e));
                    if (sum < lo) hi += 1;
                    lo = sum;
                }
            }
            lo &= ~mask;
        }
    }
    return (static_cast<Dp>(static_cast<unsigned>(hi)) << 32) | lo;
}
}
