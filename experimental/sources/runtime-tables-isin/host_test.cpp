// Host comparison only. Not an EE build input.
#include "isin.cpp"

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>

float norrath_isin_scale = [] {
    float value;
    const unsigned bits = 0x38490fdau;
    std::memcpy(&value, &bits, 4);
    return value;
}();
float norrath_isin_cubic = [] {
    float value;
    const unsigned bits = 0x3e2aaa57u;
    std::memcpy(&value, &bits, 4);
    return value;
}();
float norrath_isin_quintic = [] {
    float value;
    const unsigned bits = 0x3c086b24u;
    std::memcpy(&value, &bits, 4);
    return value;
}();
float norrath_isin_septic = [] {
    float value;
    const unsigned bits = 0x39458017u;
    std::memcpy(&value, &bits, 4);
    return value;
}();

static int check_exact(int angle, unsigned want_bits, const char* what) {
    const float got = isin__Fi(angle);
    unsigned got_bits;
    std::memcpy(&got_bits, &got, 4);
    if (got_bits != want_bits) {
        std::printf("isin(%d) bits %08x want %08x (%s)\n", angle, got_bits, want_bits, what);
        return 1;
    }
    return 0;
}

int main() {
    // A quarter turn shifts to zero, so the result is exactly 1.
    if (check_exact(16384, 0x3f800000u, "sin(pi/2)")) return 1;
    // A negative quarter turn wraps through the far half, so exactly -1.
    if (check_exact(-16384, 0xbf800000u, "sin(-pi/2)")) return 1;
    // Wide inputs alias the low halfword after the quarter-turn shift:
    // 65536 + 16384 shifts back to zero, so exactly 1.
    if (check_exact(65536 + 16384, 0x3f800000u, "wrap")) return 1;
    const double turn = 6.283185307179586476925286766559;
    double worst = 0.0;
    for (int angle = -65536; angle < 65536; ++angle) {
        const int reduced = static_cast<short>(angle);
        const double radians = static_cast<double>(reduced) * turn / 65536.0;
        const double want = std::sin(radians);
        const double got = static_cast<double>(isin__Fi(angle));
        const double error = std::fabs(got - want);
        if (error > worst) worst = error;
        if (!(error < 5.0e-7)) {
            std::printf("isin(%d) got %.10g want %.10g\n", angle, got, want);
            return 1;
        }
    }
    if (!(worst > 1.0e-9)) {
        std::printf("suspiciously exact worst %g\n", worst);
        return 1;
    }
    // Zero must be near zero: sin(0) through the quarter-turn shift.
    if (!(std::fabs(static_cast<double>(isin__Fi(0))) < 1.0e-7)) {
        std::printf("isin(0) not near zero\n");
        return 1;
    }
    return 0;
}
