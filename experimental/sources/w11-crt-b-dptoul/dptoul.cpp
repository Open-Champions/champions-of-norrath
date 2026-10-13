// Observed dptoul at 0x002fbc78, 156 bytes. Converts a double-precision bit pattern (passed as a
// 64-bit value) to an unsigned 32-bit integer by unpacking it with __unpack_d into a 32-byte
// record (class +0, sign +4, power +8, 64-bit fraction +16).
//   class 2 (zero) or class 0/1: result 0.
//   sign word set (any other class, infinity included): result 0.
//   class 4 (positive infinity):  0xffffffff.
//   other, power < 0:             result 0.
//   other, power >= 32:           0xffffffff.
//   other:                        fraction >> (60 - power), low 32 bits.
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

unsigned dptoul(unsigned long long bits) {
    W11CrtBUnpackedDouble rec;
    __unpack_d(&bits, &rec);
    if (rec.cls == 2 || rec.cls < 2) {
        return 0;
    }
    if (rec.sign != 0) {
        return 0;
    }
    if (rec.cls == 4) {
        return 0xffffffffu;
    }
    if (rec.power < 0) {
        return 0;
    }
    if (rec.power >= 32) {
        return 0xffffffffu;
    }
    return static_cast<unsigned>(rec.fraction >> (60 - rec.power));
}
}
