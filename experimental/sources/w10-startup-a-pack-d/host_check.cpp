// Host comparison only. Not an EE build input. Hand-computed results per branch, plus a
// round trip through an independent host decomposition of ordinary doubles.
#include <cstdio>
#include <cstring>
#include <cmath>
#include "pack_d.cpp"
static int failures;
static const unsigned long long one = 1ull << 60;
static void expect(int cls, int sign, int power, unsigned long long fraction, unsigned long long want) {
    W10UnpackedDouble r = {cls, sign, power, 0, fraction, 0};
    unsigned long long got = __pack_d(&r);
    if (got != want) { std::printf("cls %d sign %d power %d frac %llx: %016llx want %016llx\n", cls, sign, power, fraction, got, want); ++failures; }
}
int main() {
    expect(3, 0, 0, one, 0x3ff0000000000000ull);
    expect(3, 1, 1, one, 0xc000000000000000ull);
    expect(3, 0, 0, one | (0x8000000000000ull << 8), 0x3ff8000000000000ull);
    expect(3, 0, -1022, one, 0x0010000000000000ull);
    expect(3, 0, -1023, one, 0x0000000000000000ull);
    expect(3, 1, -1023, one, 0x8000000000000000ull);
    expect(3, 0, 1023, one, 0x7fe0000000000000ull);
    expect(3, 0, 1024, one, 0x7ff0000000000000ull);
    expect(3, 1, 5000, one, 0xfff0000000000000ull);
    expect(3, 0, 0, 0, 0);
    expect(3, 1, 7, 0, 0x8000000000000000ull);
    expect(2, 0, 0, one, 0);
    expect(4, 0, 0, one, 0x7ff0000000000000ull);
    expect(4, 1, 0, 5, 0xfff0000000000000ull);
    expect(0, 0, 0, 1, 0x7ff8000000000001ull);
    expect(1, 1, 0, 0x8000000000000ull, 0xfff8000000000000ull);
    expect(1, 0, 0, 0x7ffffffffffffull, 0x7fffffffffffffffull);
    expect(3, 3, 0, one, 0xbff0000000000000ull);  // only bit 0 of the sign word counts
    expect(3, 2, 0, one, 0x3ff0000000000000ull);
    // round trip of finite normal doubles via host frexp
    const double samples[] = {1.0, -2.5, 3.14159, 1e300, -1e-300, 123456.789, 0.1};
    for (double d : samples) {
        int e; double m = std::frexp(std::fabs(d), &e);
        unsigned long long mant = static_cast<unsigned long long>(std::ldexp(m, 53));  // 53 significant bits
        unsigned long long frac = (mant << 8) << 0;  // leading bit lands on bit 60
        unsigned long long bits;
        std::memcpy(&bits, &d, 8);
        W10UnpackedDouble r = {3, d < 0, e - 1, 0, frac, 0};
        if (__pack_d(&r) != bits) { std::printf("round trip %g\n", d); ++failures; }
    }
    if (failures) return 2;
    std::printf("pack_d cases passed\n");
    return 0;
}
