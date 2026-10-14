#include "rotation.hpp"
extern "C" float norrath_integer_sine(int angle) {
    const unsigned low = (static_cast<unsigned>(angle) - 16384u) & 0xffffu;
    int folded = low < 0x8000u ? static_cast<int>(low) : static_cast<int>(low) - 65536;
    const bool negative = ((low ^ (low << 1)) & 0x8000u) != 0;
    if (negative) {
        const unsigned low15 = low & 0x7fffu;
        folded = low15 < 0x4000u ? static_cast<int>(low15) : static_cast<int>(low15) - 32768;
    }
    const float multiplier = negative ? -1.0f : 1.0f;
    const float z = static_cast<float>(folded) * norrath_isin_scale;
    const float square = z * z;
    const float cube = z * square;
    const float cubic_term = cube * norrath_isin_cubic;
    float polynomial = z - cubic_term;
    const float fifth = cube * square;
    const float quintic_term = fifth * norrath_isin_quintic;
    polynomial = polynomial + quintic_term;
    const float seventh = fifth * square;
    const float septic_term = seventh * norrath_isin_septic;
    polynomial = polynomial - septic_term;
    const float doubled = polynomial + polynomial;
    const float product = doubled * polynomial;
    const float result = 1.0f - product;
    return multiplier * result;
}
