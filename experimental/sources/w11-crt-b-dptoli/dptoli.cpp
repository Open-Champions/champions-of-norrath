// Observed dptoli at 0x002fbb50, 156 bytes. Converts a double-precision bit pattern (passed as a
// 64-bit value) to a signed 32-bit integer by unpacking it with __unpack_d into a 32-byte
// record (class +0, sign +4, power +8, 64-bit fraction +16).
//   class 2 (zero) or class 0/1: result 0.
//   class 4:                     saturates, 0x7fffffff, or 0x80000000 when the sign word is set.
//   other, power < 0:            result 0.
//   other, power >= 31:          saturates the same way.
//   other:                       fraction >> (60 - power), low 32 bits, negated when signed.
// Inferred only: the class values (see __unpack_d). Nothing was executed on the EE.
extern "C" {
struct W11CrtBUnpackedDouble {
    int cls;
    int sign;
    int power;
    int unused;
    unsigned long long fraction;
    unsigned long long unused2;
};

void __unpack_d(const unsigned long long* value, W11CrtBUnpackedDouble* out);

int dptoli(unsigned long long bits) {
    W11CrtBUnpackedDouble rec;
    __unpack_d(&bits, &rec);
    if (rec.cls == 2 || rec.cls < 2) {
        return 0;
    }
    if (rec.cls != 4) {
        if (rec.power < 0) {
            return 0;
        }
        if (rec.power < 31) {
            int magnitude = static_cast<int>(rec.fraction >> (60 - rec.power));
            return rec.sign ? -magnitude : magnitude;
        }
    }
    return rec.sign ? static_cast<int>(0x80000000u) : 0x7fffffff;
}
}
