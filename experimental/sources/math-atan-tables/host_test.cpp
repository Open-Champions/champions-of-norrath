// Host comparison only. Not an EE build input.
#include "atan_tables.cpp"

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
    if (sizeof norrath_animation_math_atan_high_view != 16u ||
        sizeof norrath_animation_math_atan_low_view != 16u ||
        sizeof norrath_animation_math_atan_coefficients_view != 44u) {
        return 1;
    }
    const double targets[] = {std::atan(0.5), std::atan(1.0), std::atan(1.5), 1.5707963267948966};
    for (unsigned interval = 0; interval < 4u; ++interval) {
        const float sum = word_at(norrath_animation_math_atan_high_view, interval * 4u) +
                          word_at(norrath_animation_math_atan_low_view, interval * 4u);
        if (!(std::fabs(static_cast<double>(sum) - targets[interval]) < 1.0e-7)) {
            std::printf("atan interval %u sum %.10g target %.10g\n", interval, sum, targets[interval]);
            return 1;
        }
    }
    const unsigned expected_coefficients[] = {
        0x3eaaaaabu, 0xbe4cccccu, 0x3e124924u, 0xbde38e38u, 0x3dba2e6eu, 0xbd9d8795u,
        0x3d886b34u, 0xbd6ef16au, 0x3d4bda59u, 0xbd15a221u, 0x3c8569d7u,
    };
    for (unsigned index = 0; index < 11u; ++index) {
        const float value = word_at(norrath_animation_math_atan_coefficients_view, index * 4u);
        unsigned bits;
        std::memcpy(&bits, &value, 4);
        if (bits != expected_coefficients[index]) {
            std::printf("coefficient %u bits %08x\n", index, bits);
            return 1;
        }
    }
    // The odd polynomial at a tiny argument is near the argument itself only if
    // the leading coefficient is the observed one-third. A zero table would not.
    const float z = 0.125f;
    const float square = z * z;
    const float leading = word_at(norrath_animation_math_atan_coefficients_view, 0);
    const float approx = z - square * z * leading;
    if (!(std::fabs(static_cast<double>(approx) - std::atan(0.125)) < 2.0e-4)) return 1;
    return 0;
}
