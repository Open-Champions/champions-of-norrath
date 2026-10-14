// Host contract for matTransform__FP6Point3P8Matrix44T0. Not an EE plan input.
// Build with -ffp-contract=off.
#include <cstdio>
#include <cstring>
#include "../w9-render-math-b-common/math_types.hpp"
extern "C" float matTransform__FP6Point3P8Matrix44T0(Point3*, const Matrix44*, const Point3*);
static bool same(float a, float b) { return std::memcmp(&a, &b, 4) == 0; }
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    Matrix44 mat;
    for (int i = 0; i < 16; ++i) mat.m[i] = static_cast<float>(1 << i) * 0.5f;
    Point3 in = {1.0f, 0.5f, 3.0f}, out = {-1, -1, -1};
    const float w = matTransform__FP6Point3P8Matrix44T0(&out, &mat, &in);
    auto col = [&](int j) { return ((mat.m[j] * in.x + mat.m[4 + j] * in.y) + mat.m[8 + j] * in.z) + mat.m[12 + j]; };
    expect(same(out.x, col(0)), "x column 0 with translation row 12");
    expect(same(out.y, col(1)), "y column 1 with translation row 13");
    expect(same(out.z, col(2)), "z column 2 with translation row 14");
    expect(same(w, col(3)), "fourth column returned, not stored");
    Point3 v = {1, 2, 3};
    Matrix44 swap = {{0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 5, 6, 7, 1}};
    matTransform__FP6Point3P8Matrix44T0(&v, &swap, &v);
    expect(v.x == 3 + 5 && v.y == 1 + 6 && v.z == 2 + 7, "output may alias input");
    Matrix44 order = {{1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0}};
    Point3 big = {16777216.0f, 1.0f, 1.0f}, o;
    matTransform__FP6Point3P8Matrix44T0(&o, &order, &big);
    expect(same(o.x, 16777216.0f), "left-to-right sum, translation last");
    if (failures) return 1;
    std::printf("matTransform44 contract passed\n");
    return 0;
}
