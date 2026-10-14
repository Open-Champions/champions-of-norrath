// Host comparison only. Not an EE build input.
#include "entity_fptodp.cpp"

#include <cmath>
#include <initializer_list>
#include <cstdio>
#include <cstring>

static float from_bits(unsigned bits) {
    float value;
    std::memcpy(&value, &bits, 4);
    return value;
}

static unsigned long long bits_of_double(double value) {
    unsigned long long bits;
    std::memcpy(&bits, &value, 8);
    return bits;
}

static unsigned long long expect(unsigned bits) {
    const unsigned sign = bits >> 31;
    const unsigned biased = (bits >> 23) & 0xffu;
    const unsigned fraction = bits & 0x7fffffu;
    if (biased == 0u) return static_cast<unsigned long long>(sign) << 63;
    if (biased < 255u) return bits_of_double(static_cast<double>(from_bits(bits)));
    const double significand = std::ldexp(static_cast<double>((1u << 23) | fraction), -23);
    const double magnitude = std::ldexp(significand, static_cast<int>(biased) - 127);
    return bits_of_double(sign ? -magnitude : magnitude);
}

static int check(unsigned bits) {
    const unsigned long long got = norrath_entity_fptodp(from_bits(bits));
    const unsigned long long wanted = expect(bits);
    if (got != wanted) {
        std::printf("fptodp %08x got %llx expect %llx\n", bits, got, wanted);
        return 1;
    }
    return 0;
}

int main() {
    const unsigned samples[] = {
        0u, 0x80000000u, 0x3f800000u, 0xbf800000u, 0x40000000u, 0xc0200000u,
        0x7f800000u, 0xff800000u, 0x7f800001u, 0xffc00000u, 0x00800000u,
        0x00000001u, 0x807fffffu, 0x3f000000u, 0x1u, 0x7f7fffffu,
    };
    for (unsigned bits : samples) {
        if (check(bits)) return 1;
    }
    for (unsigned exponent : {1u, 2u, 23u, 126u, 127u, 128u, 254u, 255u}) {
        for (unsigned sign = 0; sign < 2u; ++sign) {
            for (unsigned fraction = 0; fraction < 0x00800000u; ++fraction) {
                if (check((sign << 31) | (exponent << 23) | fraction)) return 1;
            }
        }
    }
    for (unsigned exponent = 0; exponent < 256u; ++exponent) {
        for (unsigned sign = 0; sign < 2u; ++sign) {
            for (unsigned fraction = 0; fraction < 0x00800000u; fraction += 0x40u) {
                if (check((sign << 31) | (exponent << 23) | fraction)) return 1;
            }
        }
    }
    return 0;
}
