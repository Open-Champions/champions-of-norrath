// Observed __ieee754_expf at 0x002f4b58, 596 bytes: single-precision exp in the usual
// fdlibm style (range reduction by ln2, a degree-5 polynomial in x*x, exponent scaling by
// integer addition). Constants are written as the 32-bit patterns the image holds so no
// constant arithmetic is left to the compiler. Branches, in order, on h = bits(x):
//   * h > 0x42b2d4fc (signed): return the largest finite float (0x7f7fffff, from the table
//     at 0x0047d398); NaN and +infinity take this branch too.
//   * x negative and |h| > 0x42ae ac50: return 0.0 (infinity and NaN of negative sign too).
//   * |h| > 0x3eb17218 (0.5 ln2): reduce. Up to 0x3f851591 (1.5 ln2): k = 1 - 2*sign,
//     hi = x - ln2HI[sign], lo = ln2LO[sign]. Above: k = (int)(1.4426949 * x + half[sign]),
//     hi = x - k*ln2HI[0], lo = k*ln2LO[0]; then x = hi - lo.
//   * |h| <= 0x317fffff: k = 0 and, because 1e30 + x > 1, the result is 1 + x.
//   * otherwise k = 0 and no reduction.
// With c = x - t*(P1 + t*(P2 + t*(P3 + t*(P4 + t*P5)))), t = x*x: k == 0 returns
// 1 - ((x*c)/(c-2) - x); otherwise y = 1 - ((lo - (x*c)/(2-c)) - hi), and k >= -125 adds
// k << 23 to the bits of y, while smaller k adds (k + 100) << 23 and multiplies by the
// constant 0x0d7fffff.
// Unknown: the C-level names of the constants; the table entries at 0x0047d368/6c repeat the
// 1e30 and 0x0d7fffff immediates used by the code.
namespace {
inline float from_bits(unsigned bits) {
    float value;
    __builtin_memcpy(&value, &bits, sizeof value);
    return value;
}
inline unsigned to_bits(float value) {
    unsigned bits;
    __builtin_memcpy(&bits, &value, sizeof bits);
    return bits;
}

const unsigned kHalf[2] = {0x3f000000u, 0xbf000000u};
const unsigned kLn2High[2] = {0x3f317180u, 0xbf317180u};
const unsigned kLn2Low[2] = {0x3717f7d1u, 0xb717f7d1u};
}

extern "C" float __ieee754_expf(float x) {
    const int hx = static_cast<int>(to_bits(x));
    const int sign = static_cast<int>(static_cast<unsigned>(hx) >> 31);
    const unsigned magnitude = static_cast<unsigned>(hx) & 0x7fffffffu;
    if (hx > 0x42b2d4fc) return from_bits(0x7f7fffffu);
    if (hx < 0 && magnitude > 0x42aeac50u) return 0.0f;

    int k;
    float hi = 0.0f;
    float lo = 0.0f;
    if (magnitude > 0x3eb17218u) {
        if (magnitude < 0x3f851592u) {
            hi = x - from_bits(kLn2High[sign]);
            lo = from_bits(kLn2Low[sign]);
            k = 1 - sign - sign;
        } else {
            k = static_cast<int>(from_bits(0x3fb8aa3au) * x + from_bits(kHalf[sign]));
            const float t = static_cast<float>(k);
            hi = x - t * from_bits(kLn2High[0]);
            lo = t * from_bits(kLn2Low[0]);
        }
        x = hi - lo;
    } else if (magnitude <= 0x317fffffu) {
        if (from_bits(0x7149f2c9u) + x > from_bits(0x3f800000u)) return from_bits(0x3f800000u) + x;
        k = 0;
    } else {
        k = 0;
    }

    const float t = x * x;
    const float c = x - t * (from_bits(0x3e2aaaaau) + t * (from_bits(0xbb360b60u) + t * (from_bits(0x388ab354u) +
                    t * (from_bits(0xb5ddea0eu) + t * from_bits(0x3331bb4bu)))));
    const float one = from_bits(0x3f800000u);
    const float two = from_bits(0x40000000u);
    if (k == 0) return one - ((x * c) / (c - two) - x);
    const float y = one - ((lo - (x * c) / (two - c)) - hi);
    if (k < -125) return from_bits(to_bits(y) + (static_cast<unsigned>(k + 100) << 23)) * from_bits(0x0d7fffffu);
    return from_bits(to_bits(y) + (static_cast<unsigned>(k) << 23));
}
