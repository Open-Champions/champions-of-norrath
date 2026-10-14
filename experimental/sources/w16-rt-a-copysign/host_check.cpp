// Host check (not a plan input). Build:
//   c++ -O1 -c -Dcopysign=w16_copysign -o /tmp/w16_cs_src.o copysign.cpp
//   c++ -O1 -o /tmp/w16_copysign host_check.cpp /tmp/w16_cs_src.o && /tmp/w16_copysign
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <random>

extern "C" unsigned long long w16_copysign(unsigned long long magnitude, unsigned long long sign_source);

static unsigned long long bits(double d) { unsigned long long b; std::memcpy(&b, &d, 8); return b; }
static double flt(unsigned long long b) { double d; std::memcpy(&d, &b, 8); return d; }

int main() {
    unsigned cases = 0;
    const double specials[] = {0.0, -0.0, 1.0, -1.0, 1e300, -1e-300, INFINITY, -INFINITY, 5e-324, -5e-324};
    for (double m : specials)
        for (double s : specials) {
            if (w16_copysign(bits(m), bits(s)) != bits(std::copysign(m, s))) return 1;
            ++cases;
        }
    // Arbitrary bit patterns (NaN payloads included): only the sign bit moves.
    std::mt19937_64 rng(7);
    for (int i = 0; i < 100000; ++i) {
        const unsigned long long m = rng(), s = rng();
        const unsigned long long expected = (m & 0x7fffffffffffffffull) | (s & 0x8000000000000000ull);
        if (w16_copysign(m, s) != expected) { std::printf("mismatch %llx %llx\n", m, s); return 2; }
        ++cases;
    }
    std::printf("%u copysign cases passed\n", cases);
    (void)flt;
    return 0;
}
