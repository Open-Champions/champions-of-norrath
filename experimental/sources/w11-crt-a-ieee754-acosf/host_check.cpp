// Host contract for __ieee754_acosf. Not an EE plan input. __ieee754_sqrtf is a stub with the host
// square root (correctly rounded) and a call counter. Special cases are exact; the rest is compared
// with the host's double-precision acos: this routine's constants are the fdlibm ones, whose pi is
// one float step below the nearest float, so a few ulp of difference are expected, never more.
#include <cmath>
#include <cstdio>
#include <cstring>
static int sqrt_calls;
extern "C" float __ieee754_sqrtf(float v) { ++sqrt_calls; return static_cast<float>(std::sqrt(static_cast<double>(v))); }
#define __asm__(...)
#include "ieee754_acosf.cpp"
static unsigned bits(float f) { unsigned b; std::memcpy(&b, &f, 4); return b; }
static float make(unsigned b) { float f; std::memcpy(&f, &b, 4); return f; }
static int failures;
static void expect_bits(const char* what, float got, unsigned want) {
    if (bits(got) != want) { std::printf("%s: %08x want %08x\n", what, bits(got), want); ++failures; }
}
int main() {
    expect_bits("acos(1)", __ieee754_acosf(1.0f), 0x00000000u);
    expect_bits("acos(-1)", __ieee754_acosf(-1.0f), 0x40490fdau);
    expect_bits("acos(0)", __ieee754_acosf(0.0f), 0x3fc90fdau);
    expect_bits("acos(tiny)", __ieee754_acosf(1e-20f), 0x3fc90fdau);        // |x| <= 2^-29 (bits 0x23000000)
    expect_bits("acos(-tiny)", __ieee754_acosf(-1e-20f), 0x3fc90fdau);
    expect_bits("limit", __ieee754_acosf(make(0x23000000u)), 0x3fc90fdau);
    if (!std::isnan(__ieee754_acosf(1.5f)) || !std::isnan(__ieee754_acosf(-2.0f)) || !std::isnan(__ieee754_acosf(make(0x7f800000u)))) { std::puts("|x|>1 not NaN"); ++failures; }
    if (sqrt_calls != 0) { std::puts("sqrt called on the small / special paths"); ++failures; }
    // 0.5 <= |x| < 1 uses sqrt once; |x| < 0.5 does not
    sqrt_calls = 0;
    (void)__ieee754_acosf(0.25f);
    if (sqrt_calls != 0) { std::puts("sqrt on |x| < 0.5"); ++failures; }
    (void)__ieee754_acosf(0.75f);
    (void)__ieee754_acosf(-0.75f);
    (void)__ieee754_acosf(0.5f);
    if (sqrt_calls != 3) { std::puts("sqrt count"); ++failures; }
    unsigned s = 987654321u;
    auto next = [&]() { s = s * 1664525u + 1013904223u; return s; };
    double worst = 0;
    for (int i = 0; i < 200000; ++i) {
        float x = static_cast<float>((static_cast<double>(next()) / 4294967296.0) * 2.0 - 1.0);
        if (i % 5 == 0) x *= make((100u + next() % 27u) << 23);                 // small |x|
        if (std::fabs(x) >= 1.0f) continue;
        const double want = std::acos(static_cast<double>(x));
        const double err = std::fabs(static_cast<double>(__ieee754_acosf(x)) - want);
        if (err > worst) worst = err;
        if (err > 3e-7 + 2.5e-7 * std::fabs(want)) { std::printf("acosf(%a): err %g\n", x, err); if (++failures > 10) return 2; }
    }
    if (failures) return 2;
    std::printf("__ieee754_acosf cases passed (worst error %g)\n", worst);
    return 0;
}
