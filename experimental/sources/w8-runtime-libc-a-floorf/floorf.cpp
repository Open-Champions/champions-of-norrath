// Observed floorf at 0x2f1390, 216 bytes (callers: megatileEntitize__FPci and
// the world height code). The unbiased exponent is taken from bits 23..30. At
// 23 or above (integers, infinities, NaNs) the input is returned. Otherwise a
// probe sum of the input and 0x7149f2c9 must compare greater than +0 before
// anything changes; with the input already integral the probe is skipped.
// Magnitudes below 1: +0 for positive, -1 for negative normals, unchanged for
// -0 and negative subnormals. Other values: a negative first steps away from
// zero by half the remaining mantissa unit, then the fraction bits are cleared.
// Inferred: same routine as norrath_entity_floorf, defined here under the
// symbol the callers import. Unknown: the inexact flag of the probe add.
static_assert(__FLT_MANT_DIG__ == 24, "binary32 mantissa");
static_assert(sizeof(float) == 4 && sizeof(unsigned) == 4, "float word");

static unsigned w8_floorf_bits(float value) {
    union { float f; unsigned u; } bits;
    bits.f = value;
    return bits.u;
}

static float w8_floorf_from_bits(unsigned value) {
    union { float f; unsigned u; } bits;
    bits.u = value;
    return bits.f;
}

extern "C" float floorf(float value) {
    const unsigned bits = w8_floorf_bits(value);
    const unsigned magnitude = bits & 0x7fffffffu;
    const int exponent = static_cast<int>(magnitude >> 23) - 127;
    if (exponent >= 23) return value;
    unsigned result = bits;
    const bool small = exponent < 0;
    const unsigned fraction_mask = small ? 0u : (0x007fffffu >> static_cast<unsigned>(exponent));
    if (small || (bits & fraction_mask) != 0u) {
        const float probe = value + w8_floorf_from_bits(0x7149f2c9u);
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
    return w8_floorf_from_bits(result);
}
