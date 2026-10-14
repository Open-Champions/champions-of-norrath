// Observed ceilf at 0x2f11b8, 216 bytes. Same shape as floorf at 0x2f1390:
// exponent 23 or above returns the input; a fractional input needs the probe
// sum with 0x7149f2c9 to compare greater than +0 before it changes.
// Magnitudes below 1: negative becomes -0 (0x80000000); positive becomes +1
// when at least the smallest normal, while +0 and positive subnormals stay
// unchanged.
// Other values: a positive first steps away from zero by half the remaining
// mantissa unit, then the fraction bits are cleared; a negative or zero input
// only has its fraction bits cleared.
// Unknown: the inexact flag of the probe add.
static_assert(__FLT_MANT_DIG__ == 24, "binary32 mantissa");
static_assert(sizeof(float) == 4 && sizeof(unsigned) == 4, "float word");

static unsigned w8_ceilf_bits(float value) {
    union { float f; unsigned u; } bits;
    bits.f = value;
    return bits.u;
}

static float w8_ceilf_from_bits(unsigned value) {
    union { float f; unsigned u; } bits;
    bits.u = value;
    return bits.f;
}

extern "C" float ceilf(float value) {
    const unsigned bits = w8_ceilf_bits(value);
    const unsigned magnitude = bits & 0x7fffffffu;
    const int exponent = static_cast<int>(magnitude >> 23) - 127;
    if (exponent >= 23) return value;
    unsigned result = bits;
    const bool small = exponent < 0;
    const unsigned fraction_mask = small ? 0u : (0x007fffffu >> static_cast<unsigned>(exponent));
    if (small || (bits & fraction_mask) != 0u) {
        const float probe = value + w8_ceilf_from_bits(0x7149f2c9u);
        if (0.0f < probe) {
            if (small) {
                if ((bits & 0x80000000u) != 0u) result = 0x80000000u;
                else if (0x007fffffu < magnitude) result = 0x3f800000u;
            } else {
                if (static_cast<int>(bits) > 0)
                    result = bits + (0x00800000u >> static_cast<unsigned>(exponent));
                result &= ~fraction_mask;
            }
        }
    }
    return w8_ceilf_from_bits(result);
}
