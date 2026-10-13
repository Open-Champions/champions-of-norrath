// Host contract for __ieee754_fmodf. Not an EE plan input. For every finite operand pair the
// routine must give the exact remainder, which the host's fmodf gives too (same bits, including the
// sign of a zero result); the cases where the original differs from the published routine (zero or
// subnormal y, infinite or NaN x) are checked separately. The host keeps subnormals, so cases that
// depend on the EE FPU flushing them are written for the host behaviour.
#include <cmath>
#include <cstdio>
#include <cstring>
#include <initializer_list>
#define __asm__(...)
#include "ieee754_fmodf.cpp"
static unsigned bits(float f) { unsigned b; std::memcpy(&b, &f, 4); return b; }
static float make(unsigned b) { float f; std::memcpy(&f, &b, 4); return f; }
int main() {
    int failures = 0;
    unsigned s = 2463534242u;
    auto next = [&]() { s ^= s << 13; s ^= s >> 17; s ^= s << 5; return s; };
    auto check = [&](unsigned xb, unsigned yb) {
        const float x = make(xb), y = make(yb);
        const float got = __ieee754_fmodf(x, y), want = std::fmod(x, y);
        if (bits(got) != bits(want)) { std::printf("fmodf(%08x,%08x): %08x want %08x\n", xb, yb, bits(got), bits(want)); ++failures; }
    };
    const unsigned fixed[] = {0x3f800000, 0xbf800000, 0x40200000, 0x00800000, 0x00800001, 0x007fffff, 0x00000001,
                              0x7f7fffff, 0xff7fffff, 0x41200000, 0x3dcccccd, 0x80000000, 0x00000000, 0x4b800000};
    for (unsigned a : fixed)
        for (unsigned b : fixed) {
            if ((b & 0x7fffffff) <= 0x007fffff) continue;     // zero / subnormal y: NaN here
            check(a, b);
        }
    for (int i = 0; i < 200000; ++i) {
        unsigned xb = next(), yb = next();
        if (i % 4 == 0) yb = (yb & 0x807fffff) | (((xb >> 23) & 0xff) - (next() & 7)) << 23;   // y close to x in size
        if (i % 7 == 0) xb &= 0x807fffff | ((next() & 0x1f) << 23);                           // small x exponents
        if (((xb >> 23) & 0xff) == 0xff) xb &= ~(1u << 30);                                    // keep x finite
        if ((yb & 0x7fffffff) <= 0x007fffff || ((yb >> 23) & 0xff) == 0xff) continue;
        check(xb, yb);
    }
    // zero y gives NaN from (x*y)/(x*y); a subnormal y gives the same expression, which is 1 on a host that
    // keeps subnormals (the EE FPU flushes them, so there the product is zero and the result NaN)
    for (unsigned yb : {0x00000000u, 0x80000000u})
        if (!std::isnan(__ieee754_fmodf(1.5f, make(yb)))) { std::printf("y=%08x not NaN\n", yb); ++failures; }
    for (unsigned yb : {0x00000001u, 0x007fffffu, 0x807fffffu})
        if (bits(__ieee754_fmodf(1.5f, make(yb))) != bits(1.0f)) { std::printf("y=%08x not 1\n", yb); ++failures; }
    if (!std::isnan(__ieee754_fmodf(0.0f, make(0x00000001u)))) { std::puts("0 * subnormal"); ++failures; }
    // equal magnitudes: zero with the sign of x
    if (bits(__ieee754_fmodf(-3.0f, 3.0f)) != 0x80000000u || bits(__ieee754_fmodf(3.0f, -3.0f)) != 0u) { std::puts("equal magnitude"); ++failures; }
    // |x| < |y| returns x unchanged, also for a subnormal or negative x
    if (bits(__ieee754_fmodf(make(0x80000005u), 2.0f)) != 0x80000005u) { std::puts("small x"); ++failures; }
    // observed in the paired runs: an infinite x is not rejected (the loop runs with exponent 128)
    if (std::isnan(__ieee754_fmodf(make(0x7f800000u), 3.0f)) || std::isinf(__ieee754_fmodf(make(0x7f800000u), 3.0f))) { std::puts("inf x unexpectedly rejected"); ++failures; }
    if (failures) return 2;
    std::puts("__ieee754_fmodf cases passed");
    return 0;
}
