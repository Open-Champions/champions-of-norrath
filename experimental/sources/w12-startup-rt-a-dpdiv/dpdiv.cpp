// Observed dpdiv at 0x002fb908, 320 bytes, pinned SLUS_205.65: soft-double division. The two doubles
// arrive as 64-bit values (dividend, divisor); the 64-bit quotient bits are returned. Both are split
// with __unpack_d into the 32-byte records of __unpack_d (class +0, sign +4, power +8, 64-bit fraction
// +16) and the result is built with __pack_d. Read from the code:
//   - dividend class 0 or 1 (NaN): that record is packed as it is; else divisor class 0 or 1: the
//     divisor record is packed (sign untouched).
//   - the dividend record's sign becomes the xor of both signs from here on.
//   - dividend class 4 or 2 (infinity or zero): if the divisor has the same class, the 24-byte record
//     at __thenan_df is packed; otherwise the dividend record is packed.
//   - divisor class 4: dividend fraction and power are set to 0 and it is packed.
//   - divisor class 2: dividend class becomes 4 and it is packed.
//   - otherwise the power is the difference of the powers, then, when the dividend fraction is below
//     the divisor fraction, the power drops by one and the dividend fraction doubles. 61 steps follow,
//     bit 60 down to bit 0: when the dividend fraction is not below the divisor fraction, subtract
//     it and set the step's bit in the quotient; then double the dividend fraction. The quotient is
//     stored as the dividend record's fraction and packed. No sticky bit and no rounding.
// Inferred only: the class values follow __unpack_d's notes (0 and 1 NaN, 2 zero, 3 normal, 4 infinity).
// Nothing was executed on the EE.
extern "C" {
struct DoubleParts {
    int cls;
    int sign;
    int power;
    int unused;
    unsigned long long fraction;
    unsigned long long unused2;
};

void __unpack_d(const unsigned long long* value, DoubleParts* out);
unsigned long long __pack_d(const DoubleParts* value);
extern const unsigned char norrath_player_softdouble_nan_parts_view[24] __attribute__((aligned(8)));

unsigned long long dpdiv(unsigned long long left, unsigned long long right) {
    DoubleParts a, b;
    __unpack_d(&left, &a);
    __unpack_d(&right, &b);
    if (a.cls < 2) return __pack_d(&a);
    if (b.cls < 2) return __pack_d(&b);
    a.sign = a.sign ^ b.sign;
    if (a.cls == 4 || a.cls == 2) {
        if (a.cls != b.cls) return __pack_d(&a);
        return __pack_d(reinterpret_cast<const DoubleParts*>(norrath_player_softdouble_nan_parts_view));
    }
    if (b.cls == 4) {
        a.fraction = 0;
        a.power = 0;
        return __pack_d(&a);
    }
    if (b.cls == 2) {
        a.cls = 4;
        return __pack_d(&a);
    }
    a.power = a.power - b.power;
    unsigned long long numerator = a.fraction;
    const unsigned long long denominator = b.fraction;
    if (numerator < denominator) {
        a.power = a.power - 1;
        numerator <<= 1;
    }
    unsigned long long quotient = 0;
    for (unsigned long long bit = 1ull << 60; bit != 0; bit >>= 1) {
        if (!(numerator < denominator)) {
            numerator -= denominator;
            quotient |= bit;
        }
        numerator <<= 1;
    }
    a.fraction = quotient;
    return __pack_d(&a);
}
}
