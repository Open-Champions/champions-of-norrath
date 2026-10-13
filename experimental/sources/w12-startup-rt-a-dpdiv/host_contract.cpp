// Host contract for dpdiv. Not a build-plan input.
//   c++ -std=c++17 -o check host_contract.cpp ../w10-startup-a-unpack-d/unpack_d.cpp ../w10-startup-a-pack-d/pack_d.cpp && ./check
// Links the authored __unpack_d and __pack_d. Normal quotients must be within 2 units in the last
// place of the host division (this routine truncates), the specials must give the documented
// classes, and the xor of the signs must be applied.
#include <cmath>
#include <cstdio>
#include <cstring>
extern "C" {
alignas(8) const unsigned char norrath_player_softdouble_nan_parts_view[24] = {};
}
#include "dpdiv.cpp"
static unsigned long long bits(double d) { unsigned long long u; std::memcpy(&u, &d, 8); return u; }
static double dbl(unsigned long long u) { double d; std::memcpy(&d, &u, 8); return d; }
int main() {
    int bad = 0;
    const double values[] = {1.0, -1.0, 3.0, 0.1, 7.5e10, -2.25e-9, 1e300, 1e-300, 123456.789, 0.5, 2.0};
    for (double a : values) for (double b : values) {
        const double got = dbl(dpdiv(bits(a), bits(b)));
        const double want = a / b;
        if (std::isfinite(want) && std::fabs(want) < 2.2250738585072014e-308) {   // __pack_d flushes subnormals
            if (got != 0.0 || std::signbit(got) != std::signbit(want)) { std::printf("%g/%g flush %.17g\n", a, b, got); ++bad; }
        } else if (std::isfinite(want) && want != 0.0) {
            const double err = std::fabs(got - want) / std::fabs(want);
            if (err > 1e-15 || std::signbit(got) != std::signbit(want)) { std::printf("%g/%g = %.17g want %.17g\n", a, b, got, want); ++bad; }
        }
    }
    const double inf = HUGE_VAL;
    if (!std::isinf(dbl(dpdiv(bits(1.0), bits(0.0)))) ) { std::puts("x/0"); ++bad; }
    if (dbl(dpdiv(bits(-1.0), bits(0.0))) != -inf) { std::puts("-x/0"); ++bad; }
    if (dbl(dpdiv(bits(1.0), bits(inf))) != 0.0) { std::puts("x/inf"); ++bad; }
    if (dbl(dpdiv(bits(inf), bits(2.0))) != inf) { std::puts("inf/x"); ++bad; }
    if (dbl(dpdiv(bits(-inf), bits(2.0)) ) != -inf) { std::puts("-inf/x"); ++bad; }
    if (!std::isnan(dbl(dpdiv(bits(inf), bits(inf))))) { std::puts("inf/inf"); ++bad; }
    if (!std::isnan(dbl(dpdiv(bits(0.0), bits(-0.0))))) { std::puts("0/0"); ++bad; }
    if (dbl(dpdiv(bits(0.0), bits(5.0))) != 0.0) { std::puts("0/x"); ++bad; }
    if (!std::isnan(dbl(dpdiv(0x7ff8000000000000ull, bits(2.0))))) { std::puts("nan/x"); ++bad; }
    if (!std::isnan(dbl(dpdiv(bits(2.0), 0x7ff8000000000000ull)))) { std::puts("x/nan"); ++bad; }
    return bad;
}
