// Host contract for icos__Fi. Not an EE plan input.
#include <cmath>
#include <cstdio>
#include <cstring>
extern "C" float icos__Fi(int);

int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    double worst = 0;
    int worst_angle = 0;
    for (int a = -70000; a < 140000; ++a) {
        const double want = std::cos(2.0 * M_PI * static_cast<short>(a) / 65536.0);
        const double got = icos__Fi(a);
        const double err = std::fabs(want - got);
        if (err > worst) { worst = err; worst_angle = a; }
    }
    std::printf("worst cosine error %.3g at %d\n", worst, worst_angle);
    expect(worst < 2e-6, "within 2e-6 of cos over every halfword, including wrapped arguments");
    expect(icos__Fi(0) == 1.0f && icos__Fi(65536) == 1.0f && icos__Fi(-131072) == 1.0f, "zero turn is exactly 1");
    expect(std::fabs(icos__Fi(16384)) < 2e-6f && std::fabs(icos__Fi(-16384)) < 2e-6f, "quarter turns are near zero");
    expect(icos__Fi(32768) == -1.0f && icos__Fi(-32768) == -1.0f, "half turn is exactly -1");
    for (int a = -32768; a < 32768; a += 7) {
        unsigned x, y;
        const float p = icos__Fi(a), q = icos__Fi(a + 65536 * 3);
        std::memcpy(&x, &p, 4); std::memcpy(&y, &q, 4);
        if (x != y) { expect(false, "high argument bits change the result"); break; }
        const float s = icos__Fi(-a);
        std::memcpy(&y, &s, 4);
        if (x != y) { expect(false, "cosine is even bit for bit"); break; }
    }
    if (failures) return 1;
    std::printf("icos contract passed\n");
    return 0;
}
