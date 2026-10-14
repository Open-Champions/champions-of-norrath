// Host comparison only. Not an EE build input. The soft-double callees are modelled with host
// doubles (dptofp rounds to nearest even like a host cast); the result is compared with the
// host's correctly rounded conversion of the 64-bit integer to float. Values just above a
// float tie with a sticky bit far below are the ones the guard exists for.
#include <cstdio>
#include <cstring>
#include "floatdisf.cpp"
static double D(unsigned long long b) { double d; std::memcpy(&d, &b, 8); return d; }
static unsigned long long B(double d) { unsigned long long b; std::memcpy(&b, &d, 8); return b; }
extern "C" unsigned long long litodp(int v) { return B(static_cast<double>(v)); }
extern "C" unsigned long long dpmul(unsigned long long l, unsigned long long r) { return B(D(l) * D(r)); }
extern "C" unsigned long long dpadd(unsigned long long l, unsigned long long r) { return B(D(l) + D(r)); }
extern "C" float dptofp(unsigned long long v) { return static_cast<float>(D(v)); }
static int check(long long x, int& failures) {
    const float want = static_cast<float>(x);
    const float got = __floatdisf(x);
    if (std::memcmp(&want, &got, 4) != 0) { std::printf("%lld: got %.9g want %.9g\n", x, got, want); ++failures; }
    return 0;
}
int main() {
    int failures = 0;
    const long long v[] = {0, 1, -1, 16777217, 16777219, -16777217, 4294967296LL, 4294967297LL, 9007199254740991LL,
                           9007199254740993LL, -9007199254740993LL, 9223372036854775807LL,
                           -9223372036854775807LL - 1, 0x0000010000008000LL, 0x0000010000008001LL,
                           0x4000008000000001LL, 0x4000004000000001LL, 0x4000004000000000LL, 0x4000008000000000LL, 0x40000080000007ffLL,
                           0x4000008000000800LL, 0x3fffffbfffffffffLL};
    for (long long x : v) check(x, failures);
    unsigned long long s = 88172645463325252ull;
    for (int i = 0; i < 30000; ++i) {
        s ^= s << 13; s ^= s >> 7; s ^= s << 17;
        check(static_cast<long long>(s >> (i % 60)), failures);
        // a tie in float position with a sticky bit placed low in the double-rounding zone
        check(static_cast<long long>((((s >> 41) << 41) | 0x4000000000ull | ((s & 2) << 8)) | (s & 1) << (i % 12)), failures);
        check(static_cast<long long>(((s >> 40) << 40) | 0x8000000000ull | ((s & 1) << (i % 12))), failures);
    }
    if (failures) return 2;
    std::printf("floatdisf cases passed\n");
    return 0;
}
