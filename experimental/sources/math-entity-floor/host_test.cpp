// Host comparison only. Not an EE build input.
#include "entity_floor.cpp"

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>

static unsigned bits_of(float value) {
    unsigned bits;
    std::memcpy(&bits, &value, 4);
    return bits;
}

static float from_bits(unsigned bits) {
    float value;
    std::memcpy(&value, &bits, 4);
    return value;
}

static unsigned expect_floor(unsigned bits) {
    const unsigned magnitude = bits & 0x7fffffffu;
    const int exponent = static_cast<int>(magnitude >> 23) - 127;
    const float value = from_bits(bits);
    if (exponent >= 23) return bits;
    if (exponent < 0) {
        if ((bits & 0x80000000u) == 0u) return 0u;
        if (magnitude <= 0x007fffffu) return bits;
        return bits_of(std::floor(value));
    }
    return bits_of(std::floor(value));
}

static int check(unsigned bits) {
    const float got_value = norrath_entity_floorf(from_bits(bits));
    const unsigned got = bits_of(got_value);
    const unsigned expect = expect_floor(bits);
    if (got != expect) {
        std::printf("floor %08x got %08x expect %08x\n", bits, got, expect);
        return 1;
    }
    return 0;
}

int main() {
    const unsigned samples[] = {
        0x00000000u, 0x80000000u, 0x3f800000u, 0xbf800000u, 0x3fc00000u, 0xbfc00000u,
        0x3f000000u, 0xbf000000u, 0x4b800000u, 0xcb800000u, 0x7f800000u, 0xff800000u,
        0x7fc00000u, 0xffc00001u, 0x00800000u, 0x80800000u, 0x00000001u, 0x80000001u,
        0x007fffffu, 0x807fffffu, 0x3f7fffffu, 0xbf7fffffu, 0xc0000000u, 0x42c80000u,
    };
    for (unsigned bits : samples) {
        if (check(bits)) return 1;
    }
    for (unsigned exponent = 0; exponent < 256u; ++exponent) {
        for (unsigned sign = 0; sign < 2u; ++sign) {
            for (unsigned fraction = 0; fraction < 0x00800000u; fraction += 0x101u) {
                const unsigned bits = (sign << 31) | (exponent << 23) | fraction;
                if (check(bits)) return 1;
            }
        }
    }
    return 0;
}
