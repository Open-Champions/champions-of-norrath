// Host contract for atanf. Not a build-plan input.
//   c++ -std=c++17 -O0 -ffp-contract=off -o check host_contract.cpp && ./check
// Uses the existing byte views of math-atan-tables for the three tables and a local fabsf. Checks the
// result against the host double-precision arc tangent within 4e-7 over every interval and the
// documented special cases (odd symmetry, tiny argument returned unchanged, huge arguments and
// infinities giving the +/-pi/2 constant pair). The paired run against the pinned image is the exact
// comparison; this contract catches a wrong interval, wrong table index or sign.
#include <cmath>
#include <cstdio>
#include <cstring>
#include <initializer_list>
extern "C" float fabsf(float x) { return std::fabs(x); }
#include "atanf.cpp"
#include "../math-atan-tables/atan_tables.cpp"   // after the unit: its extern declarations come first
int main() {
    int bad = 0;
    for (double v = -40.0; v <= 40.0; v += 0.0137) {
        const float x = static_cast<float>(v);
        const double want = std::atan(static_cast<double>(x));
        const double got = atanf(x);
        if (std::fabs(got - want) > 4e-7) { std::printf("x=%g got %.9g want %.9g\n", x, got, want); if (++bad > 5) return 1; }
    }
    for (int k = -30; k < 30; ++k) {
        const float x = std::ldexp(1.0f, k);
        if (std::fabs(atanf(x) - static_cast<float>(std::atan(static_cast<double>(x)))) > 4e-7 * (1 + std::fabs(x))) { std::printf("pow %d\n", k); ++bad; }
        if (atanf(-x) != -atanf(x)) { std::printf("odd %d\n", k); ++bad; }
    }
    if (atanf(1e-20f) != 1e-20f || atanf(-1e-20f) != -1e-20f || atanf(0.0f) != 0.0f) { std::puts("tiny"); ++bad; }
    const float half_pi = static_cast<float>(1.5707963267948966);
    for (float big : {1e12f, 3e38f, HUGE_VALF}) {
        if (std::fabs(atanf(big) - half_pi) > 2e-7 || std::fabs(atanf(-big) + half_pi) > 2e-7) { std::printf("big %g\n", big); ++bad; }
    }
    return bad;
}
