// Host contract for __ieee754_atan2f. Not an EE plan input. atanf and fabsf are the host's own,
// behind stubs that count calls. Checks the special-case results exactly (bit patterns for the
// original's constants), the quadrant logic against the host atan2f within a few ulp, and the
// threshold cases (k >= 61 and k < -60 with negative x) the routine short-cuts.
#include <cmath>
#include <cstdio>
#include <cstring>
static int atan_calls, fabs_calls;
extern "C" float atanf(float v) { ++atan_calls; return static_cast<float>(std::atan(static_cast<double>(v))); }
extern "C" float fabsf(float v) { ++fabs_calls; return v < 0 ? -v : v; }
#include "ieee754_atan2f.cpp"
static unsigned bits(float f) { unsigned b; std::memcpy(&b, &f, 4); return b; }
static float make(unsigned b) { float f; std::memcpy(&f, &b, 4); return f; }
static int failures;
static void expect_bits(const char* what, float got, unsigned want) {
    if (bits(got) != want) { std::printf("%s: %08x want %08x\n", what, bits(got), want); ++failures; }
}
int main() {
    // x == 1.0 -> atanf(y) and nothing else
    atan_calls = 0;
    if (__ieee754_atan2f(0.5f, 1.0f) != static_cast<float>(std::atan(0.5)) || atan_calls != 1) { std::puts("x == 1"); ++failures; }
    // y zero (or subnormal): y itself for x > 0, +-pi for x < 0, no library calls
    atan_calls = fabs_calls = 0;
    expect_bits("+0,+x", __ieee754_atan2f(0.0f, 2.0f), 0x00000000u);
    expect_bits("-0,+x", __ieee754_atan2f(-0.0f, 2.0f), 0x80000000u);
    expect_bits("+0,-x", __ieee754_atan2f(0.0f, -2.0f), 0x40490fdau);
    expect_bits("-0,-x", __ieee754_atan2f(-0.0f, -2.0f), 0xc0490fdau);
    expect_bits("sub y,+x", __ieee754_atan2f(make(0x00400000u), 2.0f), 0x00400000u);
    expect_bits("sub -y,-x", __ieee754_atan2f(make(0x80000001u), -2.0f), 0xc0490fdau);
    // x zero (or subnormal), y nonzero
    expect_bits("y>0, x=0", __ieee754_atan2f(3.0f, 0.0f), 0x3fc90fdau);
    expect_bits("y<0, x=-0", __ieee754_atan2f(-3.0f, -0.0f), 0xbfc90fdau);
    expect_bits("y>0, x sub", __ieee754_atan2f(3.0f, make(0x00000007u)), 0x3fc90fdau);
    if (atan_calls != 0 || fabs_calls != 0) { std::puts("special cases called the library"); ++failures; }
    // ratio beyond 2^60: pi/2 without calling atanf; below 2^-60 with x < 0: -z - pi etc.
    expect_bits("huge ratio", __ieee754_atan2f(1e30f, 1e-30f), 0x3fc90fdau);
    expect_bits("huge ratio y<0", __ieee754_atan2f(-1e30f, 1e-30f), 0xbfc90fdau);
    if (atan_calls != 0) { std::puts("huge ratio called atanf"); ++failures; }
    // tiny ratio, x < 0: z = 0, so +pi - (0 - pi_lo) and -((0 - pi_lo) - pi) -> about +-pi
    atan_calls = 0;
    const float tinyp = __ieee754_atan2f(1e-30f, -1e30f), tinym = __ieee754_atan2f(-1e-30f, -1e30f);
    if (atan_calls != 0 || std::fabs(tinyp - 3.14159265f) > 2e-6f || std::fabs(tinym + 3.14159265f) > 2e-6f) { std::puts("tiny ratio"); ++failures; }
    // ordinary operands against the host atan2f: all four quadrants
    unsigned s = 12345u;
    auto next = [&]() { s = s * 1664525u + 1013904223u; return s; };
    for (int i = 0; i < 100000; ++i) {
        const float y = make((next() & 0x807fffffu) | ((100u + next() % 55u) << 23));
        const float x = make((next() & 0x807fffffu) | ((100u + next() % 55u) << 23));
        if (bits(x) == 0x3f800000u) continue;
        const float got = __ieee754_atan2f(y, x), want = static_cast<float>(std::atan2(static_cast<double>(y), static_cast<double>(x)));
        if (std::fabs(got - want) > 4e-7f * std::fabs(want) + 4e-7f) { std::printf("atan2f(%a,%a): %a want %a\n", y, x, got, want); if (++failures > 10) return 2; }
    }
    if (failures) return 2;
    std::puts("__ieee754_atan2f cases passed");
    return 0;
}
