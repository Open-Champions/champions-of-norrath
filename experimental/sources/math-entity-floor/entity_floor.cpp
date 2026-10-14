// Observed: megatileEntitize__FPci calls floorf at 0x2f1390 twice (0x26917c and
// 0x269190). That body keeps the input when the unbiased exponent is at least 23,
// including infinities and NaNs. Below that it adds the constant 0x7149f2c9 and
// continues only when the sum compares greater than +0. Magnitudes below 1 become
// +0 when positive, -1 when negative and at least the smallest normal, and stay
// unchanged for -0 and negative subnormals. Other non-integers drop the fraction
// bits after a negative value first steps away from zero.
// Inferred: norrath_entity_floorf is that behavior under the reconstruction name.
// The optional Newlib floorf import is a different symbol and is not defined here.
// Unknown: the inexact flag from the large add was not executed.
static_assert(__FLT_MANT_DIG__ == 24, "binary32 mantissa");
static_assert(sizeof(float) == 4 && sizeof(unsigned) == 4, "float word");

static unsigned float_bits(float value) {
    union { float f; unsigned u; } bits;
    bits.f = value;
    return bits.u;
}

static float float_from_bits(unsigned value) {
    union { float f; unsigned u; } bits;
    bits.u = value;
    return bits.f;
}

extern "C" float norrath_entity_floorf(float value) {
    const unsigned bits = float_bits(value);
    const unsigned magnitude = bits & 0x7fffffffu;
    const int exponent = static_cast<int>(magnitude >> 23) - 127;
    if (exponent >= 23) return value;
    unsigned result = bits;
    const bool small = exponent < 0;
    const unsigned fraction_mask = small ? 0u : (0x007fffffu >> static_cast<unsigned>(exponent));
    if (small || (bits & fraction_mask) != 0u) {
        const float probe = value + float_from_bits(0x7149f2c9u);
        if (0.0f < probe) {
            if (small) {
                if ((bits & 0x80000000u) == 0u) result = 0u;
                else if (0x007fffffu < magnitude) result = 0xbf800000u;
            } else {
                if ((bits & 0x80000000u) != 0u)
                    result = bits + (0x00800000u >> static_cast<unsigned>(exponent));
                result &= ~fraction_mask;
            }
        }
    }
    return float_from_bits(result);
}
