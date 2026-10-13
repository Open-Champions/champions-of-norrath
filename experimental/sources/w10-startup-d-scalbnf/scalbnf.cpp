// Observed scalbnf at 0x002f8070, 200 bytes: x * 2^n by adjusting the exponent field.
//  - If the magnitude bits are not above 0x007fffff (zero, or a value with a zero exponent
//    field; this image does not treat denormals specially) the argument is returned as is.
//  - Otherwise the new biased exponent is (exponent field of x) + n. When it is 256 or more
//    the result is huge * copysignf(huge, x), with huge the single 0x7149f2c9. When it is
//    positive and below 256 the exponent field is replaced and sign and fraction are kept.
//  - When it is zero or negative the result is tiny * copysignf(tiny, x) with tiny the
//    single 0x0da2425f, except that n above 50000 selects huge instead.
// Inferred: this is the usual fdlibm routine reduced for a target without denormals; the
// exponent value 255 is accepted by the comparison as the image does (it is below 256), so
// an infinity or NaN with n = 0 is returned unchanged.
// The n-above-50000 test in the last case cannot change a result (that case needs a sum of
// zero or less from a nonzero exponent, so n is negative); it is kept as the image has it.
// Nothing here was executed on the EE.
extern "C" {
float copysignf(float magnitude, float sign);

float scalbnf(float x, int n) {
    const unsigned bits = __builtin_bit_cast(unsigned, x);
    const unsigned magnitude = bits & 0x7fffffffu;
    if (!(magnitude > 0x007fffffu)) return x;
    const int exponent = static_cast<int>(magnitude >> 23) + n;
    float scale;
    if (exponent >= 256) {
        scale = __builtin_bit_cast(float, 0x7149f2c9u);
    } else if (exponent > 0) {
        return __builtin_bit_cast(float, (bits & 0x807fffffu) | (static_cast<unsigned>(exponent) << 23));
    } else if (n > 50000) {
        scale = __builtin_bit_cast(float, 0x7149f2c9u);
    } else {
        scale = __builtin_bit_cast(float, 0x0da2425fu);
    }
    return scale * copysignf(scale, x);
}
}
