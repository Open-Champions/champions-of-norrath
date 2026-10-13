// Host comparison only. Not an EE build input. The three soft-double callees are modelled with
// host doubles; the result must equal the host's own conversion of the 64-bit integer.
#include <cstdio>
#include <cstring>
#include "floatdidf.cpp"
static int litodp_calls;
static double D(unsigned long long b) { double d; std::memcpy(&d, &b, 8); return d; }
static unsigned long long B(double d) { unsigned long long b; std::memcpy(&b, &d, 8); return b; }
extern "C" unsigned long long litodp(int v) { ++litodp_calls; return B(static_cast<double>(v)); }
extern "C" unsigned long long dpmul(unsigned long long l, unsigned long long r) { return B(D(l) * D(r)); }
extern "C" unsigned long long dpadd(unsigned long long l, unsigned long long r) { return B(D(l) + D(r)); }
int main() {
    int failures = 0;
    const long long v[] = {0, 1, -1, 2, 4294967295LL, 4294967296LL, 4294967297LL, -4294967295LL, -4294967296LL,
                           2147483647LL, -2147483648LL, 2147483648LL, 123456789012345LL, -123456789012345LL,
                           9007199254740993LL, 9223372036854775807LL, -9223372036854775807LL - 1,
                           0x00000000ffffffffLL, static_cast<long long>(0xffffffff00000000ull),
                           static_cast<long long>(0x8000000080000000ull), static_cast<long long>(0x00000001ffffffffull)};
    for (long long x : v) {
        litodp_calls = 0;
        const unsigned long long got = __floatdidf(x);
        if (got != B(static_cast<double>(x)) || litodp_calls != 2) {
            std::printf("%lld: %016llx want %016llx\n", x, got, B(static_cast<double>(x)));
            ++failures;
        }
    }
    unsigned long long s = 88172645463325252ull;
    for (int i = 0; i < 20000; ++i) {
        s ^= s << 13; s ^= s >> 7; s ^= s << 17;
        const long long x = static_cast<long long>(s >> (i % 60));
        if (__floatdidf(x) != B(static_cast<double>(x))) { std::printf("random %lld\n", x); ++failures; }
    }
    if (failures) return 2;
    std::printf("floatdidf cases passed\n");
    return 0;
}
