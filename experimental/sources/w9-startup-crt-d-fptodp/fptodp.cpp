// Observed fptodp at 0x002fb320, 60 bytes. Single-float to double conversion in
// the soft-float layer, as a wrapper over two library routines:
//   - the float argument (register f12) is stored in a local word and
//     __unpack_f (0x002ff4a8) unpacks it into a 16-byte record: class word at +0,
//     sign word at +4, unbiased power word at +8, and a 32-bit fraction word at +12;
//   - __make_dp (0x002fbbf0) is called with the class, sign and power words and the
//     fraction word shifted left by 30 (an unsigned 64-bit value); its 64-bit
//     result is returned unchanged.
// math-entity-fptodp authors the result of this whole path as a flattened
// function; this unit keeps the call structure instead and imports both callees.
// Unknown: the bodies of __unpack_f and __make_dp; the record's field names.
extern "C" {
struct W9UnpackedFloat {
    int cls;
    int sign;
    int exponent;
    unsigned fraction;
};
void __unpack_f(const float* value, W9UnpackedFloat* unpacked);
unsigned long long __make_dp(int cls, int sign, int exponent, unsigned long long fraction);

unsigned long long fptodp(float value) {
    W9UnpackedFloat unpacked;
    __unpack_f(&value, &unpacked);
    return __make_dp(unpacked.cls, unpacked.sign, unpacked.exponent,
                     static_cast<unsigned long long>(unpacked.fraction) << 30);
}
}
