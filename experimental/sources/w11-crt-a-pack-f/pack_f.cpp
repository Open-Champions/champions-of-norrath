// Observed __pack_f at 0x002ff808, 188 bytes, pinned SLUS_205.65. Takes a pointer to the
// four-word unpacked single record that __make_fp builds (class, sign, power, fraction at
// +0, +4, +8, +12) and returns the packed float. The fraction is a 31-bit normalised value with
// the leading one at bit 30; packing truncates (no rounding) by shifting it right by 7.
//   - class 2, or a zero fraction: exponent 0 and fraction 0 (a zero of the given sign).
//   - power below -126: also exponent 0 and fraction 0 (flush to zero; no denormals).
//   - power 129 or more: exponent 255, fraction 0 (infinity).
//   - power 128: exponent 255; a fraction with bit 31 set gives fraction 0, otherwise the
//     shifted fraction is kept.
//   - any other power: exponent = power + 127 and the shifted fraction.
//   The result is sign bit 0 of the sign word, the low eight bits of the exponent and the
//   low 23 bits of the shifted fraction. No other class is looked at, so the original treats
//   classes 0, 1, 3 and 4 alike, by their power and fraction.
// Unknown: what the class numbers stand for (2 behaves as zero); the original leaves the
// register that carries the unused third argument alone.
// Nothing was executed on the EE.
extern "C" {
struct W11UnpackedSingle {
    int cls;
    int sign;
    int power;
    unsigned fraction;
};

float __pack_f(const W11UnpackedSingle* value) {
    unsigned exponent = 0;
    unsigned fraction = value->fraction;
    if (value->cls == 2 || fraction == 0) {
        fraction = 0;
    } else if (value->power < -126) {
        fraction = 0;
    } else if (value->power >= 129) {
        exponent = 255;
        fraction = 0;
    } else {
        exponent = static_cast<unsigned>(value->power + 127);
        if (value->power == 128 && static_cast<int>(fraction) < 0) {
            exponent = 255;
            fraction = 0;
        } else {
            fraction >>= 7;
        }
    }
    const unsigned bits = (static_cast<unsigned>(value->sign) << 31) | ((exponent & 0xff) << 23) | (fraction & 0x7fffff);
    return __builtin_bit_cast(float, bits);
}
}
