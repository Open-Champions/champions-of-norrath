// Observed dptofp at 0x002fbc20, 84 bytes. Double to single-float conversion in
// the soft-float layer. The 64-bit argument is stored in a local and
// __unpack_d (0x002ff610) unpacks it into a 32-byte record: class word at +0,
// sign word at +4, unbiased power word at +8, and a 64-bit fraction at +16.
// __make_fp (0x002ff508) is then called with the class, sign and power words
// and a 32-bit fraction: the fraction shifted right 30, with bit 0 set when any
// of the low 30 bits of the fraction were set (a sticky bit). Its float result
// is returned unchanged.
// Unknown: the bodies of __unpack_d and __make_fp; the record's field names.
extern "C" {
struct W9UnpackedDouble {
    int cls;
    int sign;
    int exponent;
    int reserved;
    unsigned long long fraction;
    unsigned long long reserved2;
};
void __unpack_d(const unsigned long long* value, W9UnpackedDouble* unpacked);
float __make_fp(int cls, int sign, int exponent, unsigned fraction);

float dptofp(unsigned long long value) {
    W9UnpackedDouble unpacked;
    __unpack_d(&value, &unpacked);
    unsigned fraction = static_cast<unsigned>(unpacked.fraction >> 30);
    if ((unpacked.fraction & 0x3fffffffull) != 0ull) fraction |= 1u;
    return __make_fp(unpacked.cls, unpacked.sign, unpacked.exponent, fraction);
}
}
