// Observed __unpack_d at 0x002ff610, 188 bytes. Splits a double-precision value, passed by
// address, into a 32-byte record: class word +0, sign word +4, unbiased power +8, 64-bit
// fraction +16 (+12 is not written). The sign word is stored first in every case.
//   biased exponent 0        : class 2; nothing else written (the fraction field is ignored).
//   biased exponent 2047,
//     fraction field 0       : power 1023, class 4, fraction 1<<60.
//     fraction field != 0    : power 1023, class 1 if bit 51 of the fraction field is set and
//                              class 0 if not; fraction = the 52 raw fraction bits, unshifted.
//   anything else            : power = exponent - 1023, class 3, fraction = field << 8 | 1<<60.
// Inferred only: 4 is infinity and 0/1 are the two NaN kinds, from how __pack_d and
// __fpcmp_parts_d treat those values. Nothing was executed on the EE.
extern "C" {
struct W10UnpackedDouble {
    int cls;
    int sign;
    int power;
    int unused;
    unsigned long long fraction;
    unsigned long long unused2;
};

void __unpack_d(const unsigned long long* value, W10UnpackedDouble* out) {
    unsigned long long bits = *value;
    unsigned field = static_cast<unsigned>(bits >> 52) & 0x7ff;
    unsigned long long fraction = bits & (~0ull >> 12);
    out->sign = static_cast<int>(bits >> 63);
    if (field == 0) {
        out->cls = 2;
        return;
    }
    if (field == 2047) {
        out->power = 1023;
        if (fraction == 0) {
            out->fraction = 1ull << 60;
            out->cls = 4;
            return;
        }
        out->cls = (fraction & (1ull << 51)) ? 1 : 0;
        out->fraction = fraction;
        return;
    }
    out->fraction = (fraction << 8) | (1ull << 60);
    out->power = static_cast<int>(field) - 1023;
    out->cls = 3;
}
}
