// Host comparison only. Not an EE build input.
#include "icos.cpp"

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>

float norrath_icos_scale = [] {
    float value;
    const unsigned bits = 0x38490fdau;
    std::memcpy(&value, &bits, 4);
    return value;
}();
float norrath_icos_cubic = [] {
    float value;
    const unsigned bits = 0x3e2aaa57u;
    std::memcpy(&value, &bits, 4);
    return value;
}();
float norrath_icos_quintic = [] {
    float value;
    const unsigned bits = 0x3c086b24u;
    std::memcpy(&value, &bits, 4);
    return value;
}();
float norrath_icos_septic = [] {
    float value;
    const unsigned bits = 0x39458017u;
    std::memcpy(&value, &bits, 4);
    return value;
}();

static int check_exact(int angle, unsigned want_bits, const char* what) {
    const float got = icos__Fi(angle);
    unsigned got_bits;
    std::memcpy(&got_bits, &got, 4);
    if (got_bits != want_bits) {
        std::printf("icos(%d) bits %08x want %08x (%s)\n", angle, got_bits, want_bits, what);
        return 1;
    }
    return 0;
}

int main() {
    // Zero angle reduces to a zero polynomial, so the result is exactly 1.
    if (check_exact(0, 0x3f800000u, "cos(0)")) return 1;
    // Half turn wraps to zero with a negative sign, so exactly -1.
    if (check_exact(32768, 0xbf800000u, "cos(pi)")) return 1;
    if (check_exact(-32768, 0xbf800000u, "cos(-pi)")) return 1;
    // Arguments are 16-bit units; wider inputs alias the low halfword.
    if (check_exact(65536, 0x3f800000u, "wrap")) return 1;
    if (check_exact(98304, 0xbf800000u, "wrap half")) return 1;
    const double turn = 6.283185307179586476925286766559;
    double worst = 0.0;
    for (int angle = -65536; angle < 65536; ++angle) {
        const int reduced = static_cast<short>(angle);
        const double radians = static_cast<double>(reduced) * turn / 65536.0;
        const double want = std::cos(radians);
        const double got = static_cast<double>(icos__Fi(angle));
        const double error = std::fabs(got - want);
        if (error > worst) worst = error;
        if (!(error < 5.0e-7)) {
            std::printf("icos(%d) got %.10g want %.10g\n", angle, got, want);
            return 1;
        }
    }
    // The polynomial is an approximation, not libm cos: the worst error over
    // the full turn must stay well above zero and below the bound above.
    if (!(worst > 1.0e-9)) {
        std::printf("suspiciously exact worst %g\n", worst);
        return 1;
    }
    return 0;
}
