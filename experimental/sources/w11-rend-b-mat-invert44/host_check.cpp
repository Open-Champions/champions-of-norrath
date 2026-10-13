// Host contract for matInvert__FP8Matrix44T0. Not an EE plan input. Build with -ffp-contract=off.
#include <cmath>
#include <cstdio>
#include "../w9-render-math-b-common/math_types.hpp"
extern "C" void matInvert__FP8Matrix44T0(Matrix44*, const Matrix44*);
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    Matrix44 a = {{2, 1, 0, 0, 0, 3, 1, 0, 1, 0, 4, 0, 5, 6, 7, 1}};
    Matrix44 inv;
    matInvert__FP8Matrix44T0(&inv, &a);
    double worst = 0;
    for (int r = 0; r < 4; ++r)
        for (int c = 0; c < 4; ++c) {
            double s = 0;
            for (int k = 0; k < 4; ++k) s += double(a.m[r * 4 + k]) * inv.m[k * 4 + c];
            worst = std::fmax(worst, std::fabs(s - (r == c ? 1.0 : 0.0)));
        }
    expect(worst < 1e-5, "product with the input is the identity");
    Matrix44 same = a;
    matInvert__FP8Matrix44T0(&same, &same);
    bool equal = true;
    for (int i = 0; i < 16; ++i) equal = equal && same.m[i] == inv.m[i];
    expect(equal, "in place gives the same result");
    Matrix44 zero = {};
    Matrix44 out = a;
    matInvert__FP8Matrix44T0(&out, &zero);
    bool ident = true;
    for (int i = 0; i < 16; ++i) ident = ident && out.m[i] == (i % 5 == 0 ? 1.0f : 0.0f);
    expect(ident, "singular input gives the identity");
    if (failures) return 1;
    std::printf("matInvert44 contract passed\n");
    return 0;
}
