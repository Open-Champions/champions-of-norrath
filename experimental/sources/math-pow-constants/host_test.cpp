// Host comparison only. Not an EE build input.
#include "pow_constants.cpp"

#include <cmath>
#include <cstdio>
#include <cstring>

static float word_at(const unsigned char* base, unsigned offset) {
    unsigned bits = static_cast<unsigned>(base[offset]) |
                    (static_cast<unsigned>(base[offset + 1]) << 8) |
                    (static_cast<unsigned>(base[offset + 2]) << 16) |
                    (static_cast<unsigned>(base[offset + 3]) << 24);
    float value;
    std::memcpy(&value, &bits, 4);
    return value;
}

int main() {
    if (sizeof norrath_animation_math_view_47d3b0 != 8u ||
        sizeof norrath_animation_math_view_47d3b8 != 8u ||
        sizeof norrath_animation_math_view_47d3c0 != 8u ||
        sizeof norrath_animation_math_view_47d434 != 4u) {
        return 1;
    }
    if (word_at(norrath_animation_math_view_47d3b0, 0) != 1.0f) return 1;
    if (word_at(norrath_animation_math_view_47d3b0, 4) != 1.5f) return 1;
    if (word_at(norrath_animation_math_view_47d3b8, 0) != 0.0f) return 1;
    if (word_at(norrath_animation_math_view_47d3c0, 0) != 0.0f) return 1;
    const float log_high = word_at(norrath_animation_math_view_47d3b8, 4);
    const float log_low = word_at(norrath_animation_math_view_47d3c0, 4);
    const double log_sum = static_cast<double>(log_high) + static_cast<double>(log_low);
    if (!(std::fabs(log_sum - std::log2(1.5)) < 1.0e-7)) {
        std::printf("log2(1.5) sum %.10g\n", log_sum);
        return 1;
    }
    unsigned finite_max = 0;
    std::memcpy(&finite_max, norrath_animation_math_view_47d434, 4);
    if (finite_max != 0x7f7fffffu) return 1;
    return 0;
}
