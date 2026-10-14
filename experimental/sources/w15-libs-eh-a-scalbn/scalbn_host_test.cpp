// Host check only; not an EE build input. Build the source with -Dscalbn=w15_scalbn -Ddpmul=w15_dpmul
// -Ddpadd=w15_dpadd -Dcopysign=w15_copysign; this file is compiled without them. The helpers are stubbed with
// host doubles. The result must equal the host scalbn bit for bit (non-NaN) for exponents up to +-50000,
// where the 2^54 / 2^-54 intermediate steps and the overflow tests all give the correctly rounded result.
#include <cmath>
#include <cstdio>
#include <cstring>
typedef unsigned long long Dp;
static double to_d(Dp b) { double d; std::memcpy(&d, &b, 8); return d; }
static Dp to_b(double d) { Dp b; std::memcpy(&b, &d, 8); return b; }
extern "C" Dp w15_dpmul(Dp a, Dp b) { return to_b(to_d(a) * to_d(b)); }
extern "C" Dp w15_dpadd(Dp a, Dp b) { return to_b(to_d(a) + to_d(b)); }
extern "C" Dp w15_copysign(Dp m, Dp s) { return to_b(std::copysign(to_d(m), to_d(s))); }
extern "C" Dp w15_scalbn(Dp x, int n);
static int bad;
static void check(Dp bits, int n) {
    const double want = std::scalbn(to_d(bits), n);
    const Dp got = w15_scalbn(bits, n);
    if (std::isnan(want)) { if (!std::isnan(to_d(got))) ++bad; return; }
    if (got != to_b(want)) { ++bad; if (bad < 10) std::printf("mismatch %016llx n=%d got %016llx want %016llx\n", bits, n, got, to_b(want)); }
}
int main() {
    const Dp fixed[] = {0, 1ull << 63, 1, 0x8000000000000001ull, 0xfffffull, 0x000fffffffffffffull, 0x0010000000000000ull,
                        0x3ff0000000000000ull, 0xbff0000000000000ull, 0x7ff0000000000000ull, 0xfff0000000000000ull,
                        0x7ff8000000000000ull, 0x7fefffffffffffffull, 0xffefffffffffffffull};
    const int ns[] = {0, 1, -1, 52, 53, 54, 55, -53, -54, -55, 1023, -1023, 1024, -1024, 1074, -1074, 2046, -2046, 3000, -3000, 50000, -50000};
    for (Dp b : fixed) for (int n : ns) check(b, n);
    Dp s = 88172645463325252ull;
    for (int i = 0; i < 300000; ++i) {
        s ^= s << 13; s ^= s >> 7; s ^= s << 17;
        const int n = static_cast<int>(s % 4400) - 2200;
        check(s, n);
        check(s & 0x800fffffffffffffull, n);          // subnormals
        check(s, n * 20);                              // large counts
    }
    if (bad) return 1;
    std::puts("scalbn ok");
    return 0;
}
