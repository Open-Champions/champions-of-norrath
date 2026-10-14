#include "animation_math.hpp"
using namespace creature_detail;
extern "C" float norrath_math_ieee754_atan2f(float y, float x) {
    const unsigned y_bits = word(address(&y));
    const unsigned x_bits = word(address(&x));
    const unsigned y_magnitude = y_bits & 0x7fffffffu;
    const unsigned x_magnitude = x_bits & 0x7fffffffu;
    if (x_bits == 0x3f800000u) return norrath_math_atanf(y);
    const unsigned quadrant = ((x_bits >> 30) & 2u) | (y_bits >> 31);
    // Actual body treats all magnitude bits <=007fffff as this special path.
    // This is deliberately broader than a host IEEE zero test.
    if (y_magnitude <= 0x007fffffu) {
        if (quadrant == 2u) return bits(0x40490fdau);
        if (quadrant == 3u) return bits(0xc0490fdau);
        return y;
    }
    if (x_magnitude <= 0x007fffffu)
        return (y_bits & 0x80000000u) != 0u ? bits(0xbfc90fdau) : bits(0x3fc90fdau);
    const int exponent_difference = signed_word(sra(y_magnitude - x_magnitude, 23u));
    float angle;
    if (exponent_difference > 60) angle = bits(0x3fc90fdau);
    else if ((x_bits & 0x80000000u) != 0u && exponent_difference < -60) angle = bits(0u);
    else {
        const float ratio = y / x;
        const float magnitude = norrath_math_fabsf(ratio);
        angle = norrath_math_atanf(magnitude);
    }
    if (quadrant == 0u) return angle;
    if (quadrant == 1u) return bits(word(address(&angle)) ^ 0x80000000u);
    const float correction = angle - bits(0x34222168u);
    if (quadrant == 2u) return bits(0x40490fdau) - correction;
    return correction - bits(0x40490fdau);
}
