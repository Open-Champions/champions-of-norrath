#include "math_helpers.hpp"
using namespace creature_detail;
extern "C" float norrath_math_atanf(float value) {
    const unsigned representation = word(address(&value));
    const unsigned magnitude = representation & 0x7fffffffu;
    if (magnitude > 0x507fffffu) {
        const float high = scalar(address(norrath_animation_math_atan_high_view) + 12u);
        const float low = scalar(address(norrath_animation_math_atan_low_view) + 12u);
        if (signed_word(representation) > 0) return high + low;
        const float negative_high = -high;
        return negative_high - low;
    }
    int interval = -1;
    float reduced = value;
    if (magnitude <= 0x3edfffffu) {
        if (magnitude <= 0x30ffffffu) {
            const float probe = value + bits(0x7149f2c9u);
            if (bits(0x3f800000u) < probe) return value;
        }
    } else {
        const float absolute = norrath_math_fabsf(value);
        if (magnitude <= 0x3f97ffffu) {
            if (magnitude <= 0x3f2fffffu) {
                const float doubled = absolute + absolute;
                const float denominator = absolute + bits(0x40000000u);
                const float numerator = doubled - bits(0x3f800000u);
                reduced = numerator / denominator;
                interval = 0;
            } else {
                const float denominator = absolute + bits(0x3f800000u);
                const float numerator = absolute - bits(0x3f800000u);
                reduced = numerator / denominator;
                interval = 1;
            }
        } else if (magnitude <= 0x401bffffu) {
            const float product = absolute * bits(0x3fc00000u);
            const float numerator = absolute - bits(0x3fc00000u);
            const float denominator = product + bits(0x3f800000u);
            reduced = numerator / denominator;
            interval = 2;
        } else {
            reduced = bits(0xbf800000u) / absolute;
            interval = 3;
        }
    }
    const float square = reduced * reduced;
    const Address table = address(norrath_animation_math_atan_coefficients_view);
    // Actual coefficient load order and separate polynomial operations.
    const float c10 = scalar(table + 40u);
    const float c8 = scalar(table + 32u);
    const float fourth = square * square;
    const float c9 = scalar(table + 36u);
    const float c6 = scalar(table + 24u);
    const float c7 = scalar(table + 28u);
    const float c4 = scalar(table + 16u);
    const float odd0 = fourth * c10;
    const float c5 = scalar(table + 20u);
    const float even0 = fourth * c9;
    const float c2 = scalar(table + 8u);
    const float c3 = scalar(table + 12u);
    const float c1 = scalar(table + 4u);
    const float odd_sum0 = c8 + odd0;
    const float c0 = scalar(table);
    const float even_sum0 = c7 + even0;
    const float odd1 = fourth * odd_sum0;
    const float even1 = fourth * even_sum0;
    const float odd_sum1 = c6 + odd1;
    const float even_sum1 = c5 + even1;
    const float odd2 = fourth * odd_sum1;
    const float even2 = fourth * even_sum1;
    const float odd_sum2 = c4 + odd2;
    const float even_sum2 = c3 + even2;
    const float odd3 = fourth * odd_sum2;
    const float even3 = fourth * even_sum2;
    const float odd_sum3 = c2 + odd3;
    const float even_sum3 = c1 + even3;
    const float odd4 = fourth * odd_sum3;
    const float even = fourth * even_sum3;
    const float odd_sum4 = c0 + odd4;
    const float odd = square * odd_sum4;
    const float polynomial = odd + even;
    if (interval < 0) {
        const float product = reduced * polynomial;
        return reduced - product;
    }
    const unsigned offset = static_cast<unsigned>(interval) << 2;
    const float low = scalar(address(norrath_animation_math_atan_low_view) + offset);
    const float high = scalar(address(norrath_animation_math_atan_high_view) + offset);
    const float product = reduced * polynomial;
    const float low_difference = product - low;
    const float reduced_difference = low_difference - reduced;
    float result = high - reduced_difference;
    if (signed_word(representation) < 0) result = -result;
    return result;
}
