// Host check only; not an EE build input. Build the source with -D__ieee754_sqrt=w15_sqrt -Ddpmul=w15_dpmul
// -Ddpadd=w15_dpadd -Ddpsub=w15_dpsub -Ddpdiv=w15_dpdiv -Ddpcmp=w15_dpcmp; this file is compiled without them.
// The soft-double helpers are stubbed with host doubles; the result must equal the host sqrt bit for bit
// (non-NaN) over boundary and random bit patterns, including subnormals, and NaN where the host gives NaN.
#include <cmath>
#include <cstdio>
#include <cstring>
typedef unsigned long long Dp;
static double to_d(Dp b) { double d; std::memcpy(&d, &b, 8); return d; }
static Dp to_b(double d) { Dp b; std::memcpy(&b, &d, 8); return b; }
extern "C" Dp w15_dpmul(Dp a, Dp b) { return to_b(to_d(a) * to_d(b)); }
extern "C" Dp w15_dpadd(Dp a, Dp b) { return to_b(to_d(a) + to_d(b)); }
extern "C" Dp w15_dpsub(Dp a, Dp b) { return to_b(to_d(a) - to_d(b)); }
extern "C" Dp w15_dpdiv(Dp a, Dp b) { return to_b(to_d(a) / to_d(b)); }
extern "C" int w15_dpcmp(Dp a, Dp b) { double x = to_d(a), y = to_d(b); return x < y ? -1 : x > y ? 1 : 0; }
extern "C" Dp w15_sqrt(Dp x);
static int bad;
static void check(Dp bits) {
    const double v = to_d(bits);
    const Dp got = w15_sqrt(bits);
    const double want = std::sqrt(v);
    if (std::isnan(want)) { if (!std::isnan(to_d(got))) { ++bad; std::printf("nan mismatch %016llx\n", bits); } return; }
    if (got != to_b(want)) { ++bad; if (bad < 10) std::printf("mismatch %016llx got %016llx want %016llx\n", bits, got, to_b(want)); }
}
int main() {
    const Dp fixed[] = {0, 1ull << 63, 0x3ff0000000000000ull, 0x4000000000000000ull, 0x4010000000000000ull, 0x7ff0000000000000ull,
                        0xfff0000000000000ull, 0x7ff8000000000000ull, 0xbff0000000000000ull, 1, 2, 0xfffffull, 0x100000ull,
                        0x1fffffull, 0x8000000000000001ull, 0x000fffffffffffffull, 0x0010000000000000ull, 0x7fefffffffffffffull,
                        0x0000000100000000ull, 0x0000100000000000ull, 0x400fffffffffffffull, 0x3fefffffffffffffull};
    for (Dp b : fixed) check(b);
    for (unsigned e = 1; e < 2046; ++e) {   // just below / above each power of two
        check((static_cast<Dp>(e) << 52) - 1); check((static_cast<Dp>(e) << 52) + 1);
        check(((static_cast<Dp>(e) << 52) | 0xfffff00000000ull) - 1);
    }
    Dp s = 88172645463325252ull;
    for (int i = 0; i < 300000; ++i) {
        s ^= s << 13; s ^= s >> 7; s ^= s << 17;
        check(s);                         // any exponent, either sign
        check(s & 0x000fffffffffffffull); // subnormals, every leading-zero count
        check(s >> (s & 31) >> 12);       // more subnormal shapes
    }
    if (bad) return 1;
    std::puts("sqrt ok");
    return 0;
}
