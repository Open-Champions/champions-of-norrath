// Host comparison only. Not an EE build input.
#include "double_isnan.cpp"

#include <cstdio>
#include <initializer_list>

static int expect(unsigned long long bits) {
    const bool exponent = (bits & 0x7ff0000000000000ULL) == 0x7ff0000000000000ULL;
    const bool fraction = (bits & 0x000fffffffffffffULL) != 0ull;
    return exponent && fraction ? 1 : 0;
}

int main() {
    const unsigned long long samples[] = {
        0ull, 0x8000000000000000ull, 0x3ff0000000000000ull,
        0x7ff0000000000000ull, 0xfff0000000000000ull,
        0x7ff0000000000001ull, 0xfff0000000000001ull,
        0x7ff8000000000000ull, 0x7fffffffffffffffll, 0xffffffffffffffffull,
        0x7fefffffffffffffull, 0x000fffffffffffffll,
    };
    for (unsigned long long bits : samples) {
        const int got = norrath_factory_double_isnan(bits);
        if (got != expect(bits) || (got != 0 && got != 1)) {
            std::printf("sample %llx got %d expect %d\n", bits, got, expect(bits));
            return 1;
        }
    }
    const unsigned fractions[] = {0u, 1u, 0x80000u, 0xfffffu};
    for (unsigned exponent = 0; exponent < 2048u; ++exponent) {
        for (unsigned sign = 0; sign < 2u; ++sign) {
            for (unsigned fraction_high : fractions) {
                for (unsigned low : {0u, 1u, 0x80000000u, 0xffffffffu}) {
                    const unsigned high = (sign << 31) | (exponent << 20) | fraction_high;
                    const unsigned long long bits =
                        (static_cast<unsigned long long>(high) << 32) | low;
                    const int got = norrath_factory_double_isnan(bits);
                    if (got != expect(bits)) {
                        std::printf("grid %llx got %d expect %d\n", bits, got, expect(bits));
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}
