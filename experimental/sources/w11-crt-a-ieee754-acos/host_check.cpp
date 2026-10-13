// Host contract for __ieee754_acos. Not an EE plan input. The soft-double callees are modelled with
// host doubles (and count their calls); the result is compared with the host's acos. The
// fdlibm routine is accurate to about one unit in the last place, so the allowed difference is two
// units. Special cases are exact.
#include <cmath>
#include <cstdio>
#include <cstring>
#include "ieee754_acos.cpp"
static int calls, sqrt_calls;
static double D(unsigned long long b) { double d; std::memcpy(&d, &b, 8); return d; }
static unsigned long long B(double d) { unsigned long long b; std::memcpy(&b, &d, 8); return b; }
extern "C" Dp dpadd(Dp l, Dp r) { ++calls; return B(D(l) + D(r)); }
extern "C" Dp dpsub(Dp l, Dp r) { ++calls; return B(D(l) - D(r)); }
extern "C" Dp dpmul(Dp l, Dp r) { ++calls; return B(D(l) * D(r)); }
extern "C" Dp dpdiv(Dp l, Dp r) { ++calls; return B(D(l) / D(r)); }
extern "C" Dp __ieee754_sqrt(Dp v) { ++sqrt_calls; return B(std::sqrt(D(v))); }
static int failures;
static double acosd(double x) { return D(__ieee754_acos(B(x))); }
int main() {
    if (B(acosd(1.0)) != 0) { std::puts("acos(1)"); ++failures; }
    if (B(acosd(-1.0)) != 0x400921fb54442d18ull) { std::puts("acos(-1)"); ++failures; }
    if (B(acosd(0.0)) != 0x3ff921fb54442d18ull || B(acosd(-0.0)) != 0x3ff921fb54442d18ull) { std::puts("acos(0)"); ++failures; }
    if (B(acosd(1e-20)) != 0x3ff921fb54442d18ull) { std::puts("tiny"); ++failures; }
    if (calls != 0 || sqrt_calls != 0) { std::puts("special cases made calls"); ++failures; }
    if (!std::isnan(acosd(1.0000000000000002)) || !std::isnan(acosd(-2.0)) || !std::isnan(acosd(INFINITY))) { std::puts("|x|>1"); ++failures; }
    // one sqrt on the two outer ranges, none inside
    sqrt_calls = 0; (void)acosd(0.25); (void)acosd(-0.25);
    if (sqrt_calls != 0) { std::puts("sqrt inside"); ++failures; }
    (void)acosd(0.75); (void)acosd(-0.75); (void)acosd(0.5); (void)acosd(-0.5);
    if (sqrt_calls != 4) { std::puts("sqrt outside"); ++failures; }
    unsigned long long s = 88172645463325252ull;
    auto next = [&]() { s ^= s << 13; s ^= s >> 7; s ^= s << 17; return s; };
    double worst = 0;
    for (int i = 0; i < 300000; ++i) {
        double x = (static_cast<double>(next() >> 11) / 9007199254740992.0) * 2.0 - 1.0;
        if (i % 5 == 0) x *= D((static_cast<unsigned long long>(1023 - 1 - static_cast<int>(next() % 56)) << 52));
        if (std::fabs(x) >= 1.0) continue;
        const double got = acosd(x), want = std::acos(x);
        const double ulp = std::nextafter(want, 4.0) - want;
        const double err = std::fabs(got - want) / ulp;
        if (err > worst) worst = err;
        if (err > 2.0) { std::printf("acos(%.17g): %.17g want %.17g\n", x, got, want); if (++failures > 10) return 2; }
    }
    if (failures) return 2;
    std::printf("__ieee754_acos cases passed (worst %.2f ulp)\n", worst);
    return 0;
}
