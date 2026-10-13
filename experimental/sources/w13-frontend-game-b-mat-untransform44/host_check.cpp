// Host contract for matUntransform__FP6Point3P8Matrix44T0. Not an EE plan input.
// Build with -ffp-contract=off.
#include <cstdio>
#include <cstring>
#include "../w9-render-math-b-common/math_types.hpp"
extern "C" float matUntransform__FP6Point3P8Matrix44T0(Point3*, const Matrix44*, const Point3*);
static bool same(float a, float b) { return std::memcmp(&a, &b, 4) == 0; }
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    Matrix44 mat;
    for (int i = 0; i < 16; ++i) mat.m[i] = float(1 << i) * 0.5f;
    const float* m = mat.m;
    Point3 in = {1.0f, 0.5f, 3.0f}, out = {-1, -1, -1};
    float w = matUntransform__FP6Point3P8Matrix44T0(&out, &mat, &in);
    expect(same(out.x, m[0] * 1.0f + m[1] * 0.5f + m[2] * 3.0f + m[3]), "x row is m0..m3");
    expect(same(out.y, m[4] * 1.0f + m[5] * 0.5f + m[6] * 3.0f + m[7]), "y row is m4..m7");
    expect(same(out.z, m[8] * 1.0f + m[9] * 0.5f + m[10] * 3.0f + m[11]), "z row is m8..m11");
    expect(same(w, m[3] * 1.0f + m[7] * 0.5f + m[11] * 3.0f + m[15]), "returned sum reads m3, m7, m11, m15");
    Point3 v = {1, 2, 3};
    matUntransform__FP6Point3P8Matrix44T0(&v, &mat, &v);
    expect(same(v.x, m[0] * 1.0f + m[1] * 2.0f + m[2] * 3.0f + m[3]) && same(v.y, m[4] * 1.0f + m[5] * 2.0f + m[6] * 3.0f + m[7]), "output may alias input");
    if (failures) return 1;
    std::printf("matUntransform44 contract passed\n");
    return 0;
}
