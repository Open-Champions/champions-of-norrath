// Host check only; not an EE build input. Build the source with -Dfloor=w15_floor -Ddpadd=w15_dpadd
// -Ddpcmp=w15_dpcmp; this file is compiled without them. dpadd/dpcmp are stubbed with host doubles and the
// result is compared bit for bit with the host floor() on boundary and random values.
#include <cmath>
#include <cstdio>
#include <cstring>
typedef unsigned long long Dp;
static double to_d(Dp b) { double d; std::memcpy(&d, &b, 8); return d; }
static Dp to_b(double d) { Dp b; std::memcpy(&b, &d, 8); return b; }
extern "C" Dp w15_dpadd(Dp a, Dp b) { return to_b(to_d(a) + to_d(b)); }
extern "C" int w15_dpcmp(Dp a, Dp b) { double x = to_d(a), y = to_d(b); return x < y ? -1 : x > y ? 1 : 0; }
extern "C" Dp w15_floor(Dp x);
static int bad;
static void check(double v) {
    const Dp got = w15_floor(to_b(v));
    const double want = std::floor(v);
    if (std::isnan(v)) { if (!std::isnan(to_d(got))) ++bad; return; }
    if (got != to_b(want)) { ++bad; std::printf("mismatch %.17g got %.17g want %.17g\n", v, to_d(got), want); }
}
int main() {
    const double fixed[] = {0.0, 1.0, 0.5, 1.5, 2.0, 3.999, 1e300, 4503599627370496.0, 4503599627370497.0,
                            2251799813685248.5, 1048575.5, 1048576.0, 1e-5, 5e-324, 1048577.25, 1e15 + 0.5};
    for (double v : fixed) { check(v); check(-v); }
    check(INFINITY); check(-INFINITY); check(NAN);
    unsigned long long s = 88172645463325252ull;   // xorshift
    for (int i = 0; i < 200000; ++i) {
        s ^= s << 13; s ^= s >> 7; s ^= s << 17;
        const int e = static_cast<int>(s % 80) - 10;
        const double frac = static_cast<double>(s >> 11) / 9007199254740992.0;
        check(std::ldexp(frac, e)); check(-std::ldexp(frac, e));
        check(to_d(s));   // random bit patterns reach every exponent
    }
    // Carry from the low word on a negative value.
    check(-4294967295.5); check(-4294967296.5); check(-9007199254740991.0 / 3.0);
    if (bad) return 1;
    std::puts("floor ok");
    return 0;
}
