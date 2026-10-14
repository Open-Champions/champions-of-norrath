// Host comparison only. Not an EE build input.
#define floorf w8_floorf_under_test
#include "floorf.cpp"
#undef floorf

#include <cmath>
#include <cstdio>
#include <cstring>

static unsigned bits_of(float value) { unsigned b; std::memcpy(&b, &value, 4); return b; }
static float from_bits(unsigned b) { float v; std::memcpy(&v, &b, 4); return v; }

// Model: libm result for ordinary values; the observed routine leaves values
// whose magnitude is below the smallest normal alone on the side where libm
// would return zero of the other sign (see the notes in floorf.cpp).
static unsigned expect_bits(unsigned bits) {
    const unsigned magnitude = bits & 0x7fffffffu;
    const int exponent = static_cast<int>(magnitude >> 23) - 127;
    const float value = from_bits(bits);
    if (exponent >= 23) return bits;
    const bool negative = (bits & 0x80000000u) != 0u;
    if (exponent < 0) {
        if (magnitude == 0u) return bits;
        if (magnitude <= 0x007fffffu) return negative ? bits : 0u; // subnormal
        return bits_of(std::floor(value));
    }
    return bits_of(std::floor(value));
}

static int check(unsigned bits) {
    const unsigned got = bits_of(w8_floorf_under_test(from_bits(bits)));
    const unsigned expect = expect_bits(bits);
    if (got != expect) {
        std::printf("floorf %08x got %08x expect %08x\n", bits, got, expect);
        return 1;
    }
    return 0;
}

int main() {
    int failures = 0;
    for (unsigned bits = 0; bits < 0x7f800000u; bits += 0x1357u) failures += check(bits) + check(bits | 0x80000000u);
    for (unsigned bits = 0x3e800000u; bits < 0x4c000000u; bits += 0x11u) failures += check(bits) + check(bits | 0x80000000u);
    const unsigned edges[] = {0x00000000u, 0x00000001u, 0x007fffffu, 0x00800000u, 0x3f7fffffu, 0x3f800000u,
                              0x3f800001u, 0x4b7fffffu, 0x4b800000u, 0x7f800000u, 0x7fc00000u, 0x7fffffffu};
    for (unsigned bits : edges) failures += check(bits) + check(bits | 0x80000000u);
    if (failures) return 1;
    std::printf("floorf cases passed\n");
    return 0;
}
