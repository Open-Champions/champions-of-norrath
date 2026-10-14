// Observed __pack_d at 0x002ff538, 216 bytes. Builds a double-precision value from the 32-byte
// record that __unpack_d fills (class +0, sign +4, power +8, 64-bit fraction +16) and returns
// it as a 64-bit value. The three fields are combined as sign << 63, exponent << 52 and the
// low 52 bits of the fraction.
//   class 0 or 1       : exponent 2047, fraction | 1<<51 (the quiet bit is forced on).
//   class 4            : exponent 2047, fraction 0.
//   class 2            : exponent 0, fraction 0.
//   other, fraction 0  : exponent 0, fraction 0.
//   other              : power < -1022 gives exponent 0 and fraction 0; power >= 1024 gives
//                        exponent 2047 and fraction 0; otherwise exponent = power + 1023 and
//                        fraction >> 8. The shifted-out bits are discarded without rounding and
//                        bit 60 (the leading bit) falls on bit 52, which the mask removes.
// Inferred only: the meaning of the class values (see __unpack_d). Nothing was executed on the EE.
extern "C" {
struct W10UnpackedDouble {
    int cls;
    int sign;
    int power;
    int unused;
    unsigned long long fraction;
    unsigned long long unused2;
};

unsigned long long __pack_d(const W10UnpackedDouble* value) {
    unsigned cls = static_cast<unsigned>(value->cls);
    unsigned long long fraction = value->fraction;
    unsigned exponent = 0;
    if (cls < 2) {
        exponent = 2047;
        fraction |= 1ull << 51;
    } else if (cls == 4) {
        exponent = 2047;
        fraction = 0;
    } else if (cls == 2) {
        fraction = 0;
    } else if (fraction != 0) {
        int power = value->power;
        if (power < -1022) {
            fraction = 0;
        } else if (power < 1024) {
            exponent = static_cast<unsigned>(power + 1023);
            fraction >>= 8;
        } else {
            exponent = 2047;
            fraction = 0;
        }
    }
    return (fraction & (~0ull >> 12)) | (static_cast<unsigned long long>(exponent & 0x7ff) << 52) |
           (static_cast<unsigned long long>(value->sign) << 63);
}
}
