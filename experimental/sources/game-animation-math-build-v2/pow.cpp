#include "animation_math.hpp"
#include "scalar_operations.hpp"
using namespace creature_detail;
namespace {
unsigned representation(float value) { return word(address(&value)); }
float high_part(float value) { return bits(representation(value) & 0xfffff000u); }
}
extern "C" float norrath_math_ieee754_powf(float x, float y) {
    const unsigned x_bits = representation(x), y_bits = representation(y);
    const unsigned x_magnitude = x_bits & 0x7fffffffu;
    const unsigned y_magnitude = y_bits & 0x7fffffffu;
    if (y_magnitude <= 0x007fffffu) return bits(0x3f800000u);
    unsigned integer_kind = 0u; // Actual negative-base exponent classification.
    if ((x_bits & 0x80000000u) != 0u) {
        if (y_magnitude > 0x4b7fffffu) integer_kind = 2u;
        else if (y_magnitude > 0x3f7fffffu) {
            const unsigned shift = 150u - (y_magnitude >> 23);
            const unsigned whole = sra(y_magnitude, shift);
            if ((whole << (shift & 31u)) == y_magnitude)
                integer_kind = 2u - (whole & 1u);
        }
    }
    if (y_magnitude == 0x3f800000u) {
        if ((y_bits & 0x80000000u) == 0u) return x;
        return bits(0x3f800000u) / x;
    }
    if (y_bits == 0x40000000u) return x * x;
    if (y_bits == 0x3f000000u && (x_bits & 0x80000000u) == 0u)
        return norrath_math_sqrtf(x);
    const float absolute_x = norrath_math_fabsf(x);
    if (x_magnitude <= 0x007fffffu || x_magnitude == 0x3f800000u) {
        float result = absolute_x;
        if ((y_bits & 0x80000000u) != 0u) result = bits(0x3f800000u) / result;
        if ((x_bits & 0x80000000u) != 0u) {
            if (((x_magnitude + 0xc0800000u) | integer_kind) == 0u) {
                const float difference = norrath_ee_scalar_operations::subtract(result, result);
                result = norrath_ee_scalar_operations::divide(difference, difference);
            } else if (integer_kind == 1u) result = -result;
        }
        return result;
    }
    const unsigned negative_base = x_bits >> 31;
    if (((negative_base - 1u) | integer_kind) == 0u) {
        const float difference = norrath_ee_scalar_operations::subtract(x, x);
        return norrath_ee_scalar_operations::divide(difference, difference);
    }

    float logarithm_high, logarithm_low;
    if (y_magnitude > 0x4d000000u) {
        if (x_magnitude <= 0x3f7ffff7u) {
            if ((y_bits & 0x80000000u) != 0u)
                return view(norrath_animation_math_view_47d434);
            return bits(0u);
        }
        if (x_magnitude > 0x3f800007u) {
            if (signed_word(y_bits) > 0) return view(norrath_animation_math_view_47d434);
            return bits(0u);
        }
        // This original branch uses x, not the preceding fabs result.
        const float distance = x - bits(0x3f800000u);
        const float quarter_distance = distance * bits(0x3e800000u);
        const float square = distance * distance;
        const float low_linear = distance * bits(0x36eca56fu);
        const float high_linear = distance * bits(0x3fb8aa00u);
        const float cubic_inner = bits(0x3eaaaaaau) - quarter_distance;
        const float cubic = distance * cubic_inner;
        const float remainder_inner = bits(0x3f000000u) - cubic;
        const float remainder = square * remainder_inner;
        const float correction = remainder * bits(0x3fb8aa3au);
        const float low = low_linear - correction;
        const float sum = high_linear + low;
        logarithm_high = high_part(sum);
        const float lost_high = logarithm_high - high_linear;
        logarithm_low = low - lost_high;
    } else {
        const unsigned fraction = x_magnitude & 0x007fffffu;
        int exponent = int(x_magnitude >> 23) - 127;
        unsigned normalized = fraction | 0x3f800000u;
        unsigned interval = 0u;
        if (fraction > 0x001cc471u) {
            if (fraction > 0x005db3d6u) {
                exponent = int(x_magnitude >> 23) - 126;
                normalized -= 0x00800000u;
            } else interval = 1u;
        }
        const unsigned table_offset = interval << 2;
        const float normalized_x = bits(normalized);
        const float center = scalar(address(norrath_animation_math_view_47d3b0) + table_offset);
        const float denominator = normalized_x + center;
        const float numerator = normalized_x - center;
        const float reciprocal = bits(0x3f800000u) / denominator;
        const float ratio = numerator * reciprocal;
        const float ratio_high = high_part(ratio);
        const unsigned denominator_bits = ((normalized >> 1) | 0x20000000u) +
                                          (interval << 21) + 0x00040000u;
        const float denominator_high = bits(denominator_bits);
        const float shifted_denominator = denominator_high - center;
        const float ratio_square = ratio * ratio;
        const float denominator_low = normalized_x - shifted_denominator;
        const float high_product = ratio_high * denominator_high;
        const float ratio_sum = ratio_high + ratio;
        float polynomial = ratio_square * bits(0x3e53f142u);
        const float low_product = ratio_high * denominator_low;
        const float difference = numerator - high_product;
        const float fourth_power = ratio_square * ratio_square;
        polynomial = polynomial + bits(0x3e6c3254u);
        const float low_difference = difference - low_product;
        polynomial = ratio_square * polynomial;
        const float ratio_low = reciprocal * low_difference;
        polynomial = polynomial + bits(0x3e8ba304u);
        const float low_correction = ratio_low * ratio_sum;
        polynomial = ratio_square * polynomial;
        polynomial = polynomial + bits(0x3eaaaaabu);
        polynomial = ratio_square * polynomial;
        polynomial = polynomial + bits(0x3edb6db7u);
        polynomial = ratio_square * polynomial;
        const float high_square = ratio_high * ratio_high;
        polynomial = polynomial + bits(0x3f199999u);
        const float square_plus_three = high_square + bits(0x40400000u);
        polynomial = fourth_power * polynomial;
        polynomial = polynomial + low_correction;
        const float denominator_sum = square_plus_three + polynomial;
        const float quotient_high = high_part(denominator_sum);
        const float high_minus_three = quotient_high - bits(0x40400000u);
        const float quotient_error = high_minus_three - high_square;
        const float low_times_high = ratio_low * quotient_high;
        const float high_times_high = ratio_high * quotient_high;
        const float quotient_low = polynomial - quotient_error;
        const float quotient_correction = quotient_low * ratio;
        const float product_low = low_times_high + quotient_correction;
        const float product_sum = high_times_high + product_low;
        const float product_high = high_part(product_sum);
        const float product_error = product_high - high_times_high;
        const float product_remainder = product_low - product_error;
        // Preserve the actual table read order: high logarithm offset then low.
        const float table_high = scalar(address(norrath_animation_math_view_47d3b8) + table_offset);
        const float scaled_low = product_high * bits(0x369dc39fu);
        const float table_low = scalar(address(norrath_animation_math_view_47d3c0) + table_offset);
        const float scaled_high = product_high * bits(0x3f763800u);
        const float exponent_float = static_cast<float>(exponent);
        const float scaled_remainder = product_remainder * bits(0x3f76384eu);
        const float low_sum = scaled_low + scaled_remainder;
        const float offset_low = low_sum + table_low;
        const float combined = scaled_high + offset_low;
        const float with_offset = combined + table_high;
        const float with_exponent = with_offset + exponent_float;
        logarithm_high = high_part(with_exponent);
        const float without_exponent = logarithm_high - exponent_float;
        const float without_offset = without_exponent - table_high;
        const float lost_high = without_offset - scaled_high;
        logarithm_low = offset_low - lost_high;
    }

    const float sign = ((negative_base - 1u) | (integer_kind - 1u)) == 0u
                     ? bits(0xbf800000u) : bits(0x3f800000u);
    const float exponent_high = high_part(y);
    const float exponent_low = y - exponent_high;
    const float low_logarithm_product = y * logarithm_low;
    const float low_exponent_product = exponent_low * logarithm_high;
    float product_high = exponent_high * logarithm_high;
    const float product_low = low_exponent_product + low_logarithm_product;
    const float product = product_low + product_high;
    const unsigned product_bits = representation(product);
    const unsigned product_magnitude = product_bits & 0x7fffffffu;
    if (signed_word(product_bits) > 0) {
        bool overflow = product_magnitude > 0x43010000u;
        if (product_magnitude == 0x43010000u) {
            const float residual = product - product_high;
            const float boundary = product_low + bits(0x3338aa3bu);
            overflow = residual < boundary;
        }
        if (overflow) {
            const float large = sign * bits(0x7149f2c9u);
            return large * bits(0x7149f2c9u);
        }
    } else {
        bool underflow = product_magnitude > 0x42fc0000u;
        if (product_magnitude == 0x42fc0000u) {
            const float residual = product - product_high;
            underflow = product_low <= residual;
        }
        if (underflow) {
            const float small = sign * bits(0x0da2425fu);
            return small * bits(0x0da2425fu);
        }
    }
    unsigned scaling = 0u;
    unsigned fractional_bits = product_bits;
    if (product_magnitude > 0x3f000000u) {
        const unsigned first_shift = (product_magnitude >> 23) - 126u;
        const unsigned half_unit = sra(0x00800000u, first_shift);
        const unsigned rounded = product_bits + half_unit;
        const unsigned exponent = ((rounded >> 23) & 255u) - 127u;
        const unsigned fraction_mask = ~sra(0x007fffffu, exponent);
        const float integral = bits(rounded & fraction_mask);
        product_high = product_high - integral;
        const unsigned significand = (rounded & 0x007fffffu) | 0x00800000u;
        scaling = sra(significand, 23u - exponent);
        if ((product_bits & 0x80000000u) != 0u) scaling = 0u - scaling;
        const float fractional = product_low + product_high;
        fractional_bits = representation(fractional);
    }
    const float fractional_high = bits(fractional_bits & 0xfffff000u);
    const float high_difference = fractional_high - product_high;
    const float low_linear = fractional_high * bits(0x35bfbe8cu);
    const float high_linear = fractional_high * bits(0x3f317200u);
    const float low_difference = product_low - high_difference;
    const float scaled_difference = low_difference * bits(0x3f317217u);
    const float logarithm_remainder = scaled_difference + low_linear;
    const float logarithm = high_linear + logarithm_remainder;
    const float logarithm_square = logarithm * logarithm;
    const float lost_high = logarithm - high_linear;
    float polynomial = logarithm_square * bits(0x3331bb4bu);
    const float remainder = logarithm_remainder - lost_high;
    polynomial = polynomial + bits(0xb5ddea0eu);
    const float remainder_product = logarithm * remainder;
    polynomial = logarithm_square * polynomial;
    const float correction = remainder + remainder_product;
    polynomial = polynomial + bits(0x388ab354u);
    polynomial = logarithm_square * polynomial;
    polynomial = polynomial + bits(0xbb360b60u);
    polynomial = logarithm_square * polynomial;
    polynomial = polynomial + bits(0x3e2aaaaau);
    polynomial = logarithm_square * polynomial;
    const float adjusted = logarithm - polynomial;
    const float quotient_numerator = logarithm * adjusted;
    const float quotient_denominator = adjusted - bits(0x40000000u);
    const float quotient = quotient_numerator / quotient_denominator;
    const float corrected = quotient - correction;
    const float without_linear = corrected - logarithm;
    float result = bits(0x3f800000u) - without_linear;
    const unsigned scaled_bits = representation(result) + (scaling << 23);
    if (signed_word(sra(scaled_bits, 23u)) > 0) result = bits(scaled_bits);
    else result = norrath_math_scalbnf(result, signed_word(scaling));
    return sign * result;
}
