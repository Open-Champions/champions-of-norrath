// Observed litodp at 0x002fba98, 188 bytes. Converts a signed 32-bit integer to a
// double-precision bit pattern (returned as a 64-bit value) by filling the 32-byte record
// that __pack_d consumes (class +0, sign +4, power +8, 64-bit fraction +16) and calling it.
//   value 0          : class 2, nothing else is written to the record.
//   otherwise        : class 3, sign = the top bit, power starts at 60, fraction = |value|.
//   0x80000000       : returned directly as 0xc1e0000000000000 without calling __pack_d.
//   normalisation    : while the fraction is at most 0x0fffffffffffffff it is doubled and
//                      the power drops by one, so bit 60 ends up set.
// Inferred only: the class values (see __unpack_d); the record's +12 and +24 words are not
// written. Nothing was executed on the EE.
extern "C" {
struct W11CrtBUnpackedDouble {
    int cls;
    int sign;
    int power;
    int unused;
    unsigned long long fraction;
    unsigned long long unused2;
};

unsigned long long __pack_d(const W11CrtBUnpackedDouble* value);

unsigned long long litodp(int value) {
    W11CrtBUnpackedDouble rec;
    rec.cls = 3;
    rec.sign = static_cast<int>(static_cast<unsigned>(value) >> 31);
    if (value == 0) {
        rec.cls = 2;
        return __pack_d(&rec);
    }
    rec.power = 60;
    if (value == static_cast<int>(0x80000000u)) {
        return 0xc1e0000000000000ull;
    }
    rec.fraction = static_cast<unsigned long long>(static_cast<long long>(value < 0 ? -value : value));
    const unsigned long long limit = ~0ull >> 4;
    if (rec.fraction <= limit) {
        int power = rec.power;
        unsigned long long fraction = rec.fraction;
        do {
            fraction <<= 1;
            --power;
        } while (fraction <= limit);
        rec.power = power;
        rec.fraction = fraction;
    }
    return __pack_d(&rec);
}
}
