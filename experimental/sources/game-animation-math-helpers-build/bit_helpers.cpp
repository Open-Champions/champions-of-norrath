#include "math_helpers.hpp"
using namespace creature_detail;
extern "C" float norrath_math_fabsf(float value) {
    return bits(word(address(&value)) & 0x7fffffffu);
}
extern "C" float norrath_math_copysignf(float magnitude, float sign) {
    const unsigned sign_word = word(address(&sign)) & 0x80000000u;
    const unsigned magnitude_word = word(address(&magnitude)) & 0x7fffffffu;
    return bits(magnitude_word | sign_word);
}
extern "C" float norrath_math_sqrtf(float value) {
    const unsigned representation = word(address(&value));
    const unsigned magnitude = representation & 0x7fffffffu;
    if (magnitude <= 0x007fffffu) return value;
    if (signed_word(representation) < 0) {
        // Actual SUB.S then dependent DIV.S must remain operations. The fixed
        // finite-format compiler folded the equivalent ordinary C++ path in
        // powf to +1; typed native adapters prevent that algebraic replacement.
        const float difference = norrath_ee_scalar_operations::subtract(value, value);
        return norrath_ee_scalar_operations::divide(difference, difference);
    }
    unsigned exponent = (representation >> 23) - 127u;
    unsigned remainder = (representation & 0x007fffffu) | 0x00800000u;
    remainder <<= exponent & 1u;
    unsigned root = 0u;
    unsigned trial = 0x01000000u;
    exponent = sra(exponent, 1u);
    remainder <<= 1;
    unsigned sum = 0u;
    do {
        const unsigned candidate = root + trial;
        if (!(signed_word(remainder) < signed_word(candidate))) {
            remainder -= candidate;
            root = candidate + trial;
            sum += trial;
        }
        trial >>= 1;
        remainder <<= 1;
    } while (trial != 0u);
    if (remainder != 0u) sum += sum & 1u;
    const unsigned result = 0x3f000000u + sra(sum, 1u) + (exponent << 23);
    return bits(result);
}
extern "C" float norrath_math_scalbnf(float value, int scale) {
    const unsigned representation = word(address(&value));
    const unsigned magnitude = representation & 0x7fffffffu;
    const unsigned biased_exponent = magnitude >> 23;
    const unsigned changed = biased_exponent + static_cast<unsigned>(scale);
    if (magnitude <= 0x007fffffu) return value;
    float multiplier;
    if (signed_word(changed) >= 256) multiplier = bits(0x7149f2c9u);
    else if (signed_word(changed) > 0)
        return bits((representation & 0x807fffffu) | (changed << 23));
    else if (scale > 50000) multiplier = bits(0x7149f2c9u);
    else multiplier = bits(0x0da2425fu);
    const float signed_multiplier = norrath_math_copysignf(multiplier, value);
    return signed_multiplier * multiplier;
}
