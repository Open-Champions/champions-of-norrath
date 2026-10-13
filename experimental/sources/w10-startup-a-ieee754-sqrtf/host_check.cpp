// Host comparison only. Not an EE build input. The empty asm statements are removed for the
// host compiler. Checks exact roots of squares, at most one unit in the last place from the host
// square root over a range of ordinary values, pass-through of zeros and subnormals, and NaN for
// negative input.
#include <cmath>
#include <cstdio>
#include <cstring>
#define __asm__(...)
#include "ieee754_sqrtf.cpp"
static unsigned bitsOf(float f) { unsigned u; std::memcpy(&u, &f, 4); return u; }
int main() {
    int failures = 0;
    for (int i = 1; i < 2000; ++i) {
        float sq = static_cast<float>(i) * static_cast<float>(i);
        if (__ieee754_sqrtf(sq) != static_cast<float>(i)) { std::printf("exact %d\n", i); ++failures; }
    }
    unsigned seed = 12345;
    for (int i = 0; i < 200000; ++i) {
        seed = seed * 1664525u + 1013904223u;
        unsigned b = (seed & 0x007fffffu) | ((((seed >> 23) % 200u) + 27u) << 23);
        float x; std::memcpy(&x, &b, 4);
        float got = __ieee754_sqrtf(x), want = std::sqrt(x);
        int d = static_cast<int>(bitsOf(got)) - static_cast<int>(bitsOf(want));
        if (d < -1 || d > 1) { std::printf("%a: %a want %a\n", x, got, want); if (++failures > 10) return 2; }
    }
    float zero = 0.0f, neg0 = -0.0f, sub; unsigned sb = 0x00400000u; std::memcpy(&sub, &sb, 4);
    if (bitsOf(__ieee754_sqrtf(zero)) != 0 || bitsOf(__ieee754_sqrtf(neg0)) != 0x80000000u || bitsOf(__ieee754_sqrtf(sub)) != sb) { std::printf("passthrough\n"); ++failures; }
    if (!std::isnan(__ieee754_sqrtf(-4.0f))) { std::printf("negative\n"); ++failures; }
    if (failures) return 2;
    std::printf("ieee754_sqrtf cases passed\n");
    return 0;
}
