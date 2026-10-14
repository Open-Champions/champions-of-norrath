// Host comparison only. Not an EE build input. Unpacks host doubles with an independent
// decomposition and checks the result against the host's own ordering (NaN gives 1, and the
// two zeros compare equal).
#include <cstdio>
#include <cstring>
#include <cmath>
#include "fpcmp_parts_d.cpp"
static W10UnpackedDouble unpack(double d) {
    W10UnpackedDouble r = {};
    unsigned long long bits;
    std::memcpy(&bits, &d, 8);
    r.sign = static_cast<int>(bits >> 63);
    if (std::isnan(d)) { r.cls = 1; return r; }
    if (std::isinf(d)) { r.cls = 4; return r; }
    if (d == 0) { r.cls = 2; return r; }
    int e; double m = std::frexp(std::fabs(d), &e);
    r.cls = 3;
    r.power = e - 1;
    r.fraction = static_cast<unsigned long long>(std::ldexp(m, 53)) << 8;
    return r;
}
int main() {
    const double v[] = {-1e300, -1000.5, -2.0, -1.5, -1.0, -1e-300, -0.0, 0.0, 1e-300, 1.0, 1.25, 1.5, 2.0, 1000.5, 1e300,
                        INFINITY, -INFINITY, NAN};
    int failures = 0;
    for (double a : v) for (double b : v) {
        W10UnpackedDouble ua = unpack(a), ub = unpack(b);
        int want = (std::isnan(a) || std::isnan(b)) ? 1 : a < b ? -1 : a > b ? 1 : 0;
        if (a == INFINITY && b == INFINITY) want = 0;
        if (a == -INFINITY && b == -INFINITY) want = 0;
        int got = __fpcmp_parts_d(&ua, &ub);
        if (got != want) { std::printf("%g vs %g: %d want %d\n", a, b, got, want); ++failures; }
    }
    if (failures) return 2;
    std::printf("fpcmp_parts_d cases passed\n");
    return 0;
}
