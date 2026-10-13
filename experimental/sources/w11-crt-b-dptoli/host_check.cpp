// Host comparison only. Not an EE build input. __unpack_d is replaced by a host stand-in that
// follows the observed record layout; results must equal the host double-to-integer conversion
// (truncation, saturation, NaN as zero).
#include <cstdio>
#include <cstring>
#include <cstdint>
#include <cmath>
#include <initializer_list>
#include "dptoli.cpp"
extern "C" void __unpack_d(const unsigned long long* value, W11CrtBUnpackedDouble* out) {
    unsigned long long bits = *value;
    unsigned field = static_cast<unsigned>(bits >> 52) & 0x7ff;
    unsigned long long fraction = bits & (~0ull >> 12);
    out->sign = static_cast<int>(bits >> 63);
    if (field == 0) { out->cls = 2; return; }
    if (field == 2047) {
        out->power = 1023;
        if (fraction == 0) { out->fraction = 1ull << 60; out->cls = 4; return; }
        out->cls = (fraction & (1ull << 51)) ? 1 : 0;
        out->fraction = fraction;
        return;
    }
    out->fraction = (fraction << 8) | (1ull << 60);
    out->power = static_cast<int>(field) - 1023;
    out->cls = 3;
}
static long long reference(double d) {
    if (d != d) return 0;
    if (d >= 2147483648.0) return 0x7fffffff;
    if (d <= -2147483649.0) return static_cast<long long>(INT32_MIN);
    return static_cast<long long>(static_cast<int>(d));
}
static int failures;
static void check(double d) {
    unsigned long long bits;
    std::memcpy(&bits, &d, 8);
    long long want = reference(d);
    long long got = static_cast<long long>(dptoli(bits));
    if (int(got) != int(want)) { std::printf("%a: got %lld want %lld\n", d, got, want); ++failures; }
}
int main() {
    const double cases[] = {0.0, -0.0, 0.5, -0.5, 1.0, -1.0, 1.5, -1.5, 2.9, -2.9, 255.75, 65535.99, 2147483647.0, 2147483647.9,
        -2147483648.0, -2147483648.9, 2147483648.0, -2147483649.0, 4294967295.0, 4294967295.5, 4294967296.0, -4294967296.0,
        1e30, -1e30, 1e-30, 5e-324, INFINITY, -INFINITY, NAN, -NAN, 1073741824.0, 3000000000.0};
    for (double d : cases) check(d);
    for (int e = -3; e < 40; ++e) {
        double base = std::ldexp(1.0, e);
        for (double m : {1.0, 1.25, 1.999999}) { check(base * m); check(-base * m); }
        check(base - 1.0 / 1024); check(-(base - 1.0 / 1024));
    }
    if (failures) return 2;
    std::printf("dptoli cases passed\n");
    return 0;
}
