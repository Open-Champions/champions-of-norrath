// Host comparison only. Not an EE build input. The kernel is driven through the authored caller
// __ieee754_rem_pio2f (w9-startup-crt-b-rem-pio2f), which supplies the scaled pieces and the
// 2/pi digit table, and checked against an independent double-precision reduction: x must equal
// n * pi/2 + y[0] + y[1] (mod 8 * pi/2), for arguments in the range where a double reduction is
// itself accurate. Large and tiny exponents are only exercised
// by the paired run against the image (see the notes), not here.
#define floorf norrath_host_floorf_decl
#define scalbnf norrath_host_scalbnf_decl
#define fabsf norrath_host_fabsf_decl
#include "kernel_rem_pio2f.cpp"
#include "../w9-startup-crt-b-rem-pio2f/rem_pio2f.cpp"
#undef floorf
#undef scalbnf
#undef fabsf
#include <cmath>
#include <cstdio>
extern "C" float norrath_host_floorf_decl(float v) { return std::floor(v); }
extern "C" float norrath_host_scalbnf_decl(float v, int e) { return std::scalbn(v, e); }
extern "C" float norrath_host_fabsf_decl(float v) { return std::fabs(v); }

int main() {
    int failures = 0;
    const double pio2 = 1.5707963267948966;
    unsigned seed = 99;
    for (int i = 0; i < 20000; ++i) {
        seed = seed * 1664525u + 1013904223u;
        float x = 202.0f + static_cast<float>(seed % 1000000u) + static_cast<float>((seed >> 8) & 255) / 256.0f;
        if (i & 1) x = -x;
        float y[2];
        int n = __ieee754_rem_pio2f(x, y);
        // n and y[0] + y[1] must satisfy x = n * pi/2 + y (mod 8 * pi/2) with y in (-pi/2, pi/2).
        // The image does not always pick the nearest multiple (for some arguments the fraction
        // test leaves y just above pi/4), so nearest is not required; the private paired run
        // against the image is what establishes the exact outputs.
        double got = static_cast<double>(y[0]) + static_cast<double>(y[1]);
        double residue = std::fmod(static_cast<double>(x) - n * pio2 - got, 8.0 * pio2);
        if (residue < 0) residue += 8.0 * pio2;
        bool congruent = residue < 1e-6 || 8.0 * pio2 - residue < 1e-6;
        if (!congruent || std::fabs(got) > pio2 || n < -7 || n > 7 || ((x < 0) != (n < 0) && n != 0)) {
            if (++failures < 8) std::printf("x=%.9g n=%d y=%.9g residue %.3g\n", x, n, got, residue);
        }
    }
    if (failures) { std::printf("%d failures\n", failures); return 2; }
    std::printf("kernel_rem_pio2f cases passed\n");
    return 0;
}
