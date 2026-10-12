// Host contract for isin__Fi. Not an EE plan input.
#include <cmath>
#include <cstdio>
#include <cstring>
extern "C" float isin__Fi(int);

int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    double worst = 0;
    int worst_angle = 0;
    for (int a = -70000; a < 140000; ++a) {
        const double want = std::sin(2.0 * M_PI * static_cast<short>(a) / 65536.0);
        const double got = isin__Fi(a);
        const double err = std::fabs(want - got);
        if (err > worst) { worst = err; worst_angle = a; }
    }
    std::printf("worst sine error %.3g at %d\n", worst, worst_angle);
    expect(worst < 2e-6, "within 2e-6 of sin over every halfword, including wrapped arguments");
    expect(isin__Fi(16384) == 1.0f && isin__Fi(-49152) == 1.0f, "quarter turn is exactly 1");
    expect(isin__Fi(-16384) == -1.0f && isin__Fi(49152) == -1.0f, "three quarters is exactly -1");
    expect(std::fabs(isin__Fi(0)) < 2e-6f && std::fabs(isin__Fi(32768)) < 2e-6f, "zero and half turn are near zero");
    for (int a = -32768; a < 32768; a += 5) {
        unsigned x, y;
        const float p = isin__Fi(a), q = isin__Fi(a + 65536 * 2);
        std::memcpy(&x, &p, 4); std::memcpy(&y, &q, 4);
        if (x != y) { expect(false, "high argument bits change the result"); break; }
    }
    if (failures) return 1;
    std::printf("isin contract passed\n");
    return 0;
}
