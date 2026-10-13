// Observed __unpack_f at 0x002ff4a8, 92 bytes. Splits a single-precision value into a 16-byte
// record: class word +0, sign word +4, unbiased power +8, 32-bit fraction +12. The sign word is
// stored first and always. A zero biased-exponent field gives class 2 and nothing else is
// written (the fraction field is not examined, so subnormals take this path too). Any other
// exponent field, including 255, is treated as an ordinary number: class 3, power = field - 127,
// and the 23 fraction bits shifted up by 7 with bit 30 (0x40000000) set as the leading bit.
// Unknown: the meaning of the class values beyond what its callers show (2 is the only value
// this routine gives to a zero field; 3 to everything else). Nothing was executed on the EE.
extern "C" {
struct W10UnpackedSingle {
    int cls;
    int sign;
    int power;
    unsigned fraction;
};

void __unpack_f(const unsigned* value, W10UnpackedSingle* out) {
    unsigned bits = *value;
    unsigned field = (bits >> 23) & 0xff;
    unsigned fraction = bits & 0x7fffff;
    out->sign = static_cast<int>(bits >> 31);
    if (field == 0) {
        out->cls = 2;
        return;
    }
    out->power = static_cast<int>(field) - 127;
    out->cls = 3;
    out->fraction = (fraction << 7) | 0x40000000u;
}
}
