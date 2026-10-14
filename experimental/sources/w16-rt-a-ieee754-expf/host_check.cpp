// Host check (not a plan input). Build:
//   c++ -O1 -std=c++17 -ffp-contract=off -c -D__ieee754_expf=w16_expf -o /tmp/w16_expf_src.o ieee754_expf.cpp
//   c++ -O1 -std=c++17 -o /tmp/w16_expf host_check.cpp /tmp/w16_expf_src.o && /tmp/w16_expf
// Compares with the host expf: the results must agree to within 3 units in the last place
// wherever the true result is a normal float, and the three saturation arms are exact.
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <random>

extern "C" float w16_expf(float x);

static std::int32_t bits(float f) { std::int32_t b; std::memcpy(&b, &f, 4); return b; }
static float flt(std::uint32_t b) { float f; std::memcpy(&f, &b, 4); return f; }

int main() {
    unsigned cases = 0;
    std::mt19937 rng(21);
    std::uniform_real_distribution<float> wide(-87.0f, 88.5f), narrow(-2.0f, 2.0f), tiny(-1e-6f, 1e-6f);
    std::int32_t worst = 0;
    for (int i = 0; i < 300000; ++i) {
        const float x = i % 3 == 0 ? wide(rng) : i % 3 == 1 ? narrow(rng) : tiny(rng);
        const float expected = std::exp(x);
        const float actual = w16_expf(x);
        if (!(expected >= 1.2e-38f) || std::isinf(expected)) continue;
        const std::int32_t diff = std::abs(bits(actual) - bits(expected));
        if (diff > worst) worst = diff;
        if (diff > 3) { std::printf("x=%a actual=%a expected=%a\n", x, actual, expected); return 1; }
        ++cases;
    }
    if (w16_expf(0.0f) != 1.0f) return 2;
    if (bits(w16_expf(100.0f)) != 0x7f7fffff) return 3;
    if (bits(w16_expf(89.5f)) != 0x7f7fffff) return 4;
    if (bits(w16_expf(-104.0f)) != 0 || bits(w16_expf(-1000.0f)) != 0) return 5;
    if (bits(w16_expf(flt(0x7f800000u))) != 0x7f7fffff) return 6;
    if (bits(w16_expf(flt(0xff800000u))) != 0) return 7;
    // Boundary arms: just below the reduction threshold, at the small-argument cut, and the
    // k = -126 path (results near the bottom of the normal range).
    const float edges[] = {flt(0x3eb17218u), flt(0x3eb17219u), flt(0x3f851591u), flt(0x3f851592u),
                           flt(0x317fffffu), flt(0x31800000u), flt(0xb17fffffu), flt(0xc2aeac50u), flt(0xc2aeac4fu),
                           -86.0f, -87.3f, -88.0f};
    for (float x : edges) {
        const float expected = std::exp(x), actual = w16_expf(x);
        if (expected > 1.2e-38f && std::abs(bits(actual) - bits(expected)) > 3) {
            std::printf("edge x=%a actual=%a expected=%a\n", x, actual, expected);
            return 8;
        }
        ++cases;
    }
    std::printf("%u expf cases passed (worst %d ulp)\n", cases, worst);
    return 0;
}
