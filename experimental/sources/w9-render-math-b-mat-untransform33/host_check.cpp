// Host contract for matUntransform__FP6Point3P8Matrix33T0. Not an EE plan input.
// Build with -ffp-contract=off.
#include <cstdio>
#include <cstring>
#include "../w9-render-math-b-common/math_types.hpp"
extern "C" Point3* matUntransform__FP6Point3P8Matrix33T0(Point3*, const Matrix33*, const Point3*);
static bool same(float a, float b) { return std::memcmp(&a, &b, 4) == 0; }
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    Matrix33 mat = {{1.5f, 2.0f, 4.0f, 8.0f, 16.0f, 32.0f, 64.0f, 128.0f, 256.0f}};
    Point3 in = {1.0f, 0.5f, 3.0f}, out = {-1, -1, -1};
    Point3* r = matUntransform__FP6Point3P8Matrix33T0(&out, &mat, &in);
    expect(r == &out, "returns the output pointer");
    expect(same(out.x, 1.5f * 1.0f + 2.0f * 0.5f + 4.0f * 3.0f), "x row is m0,m1,m2");
    expect(same(out.y, 8.0f * 1.0f + 16.0f * 0.5f + 32.0f * 3.0f), "y row is m3,m4,m5");
    expect(same(out.z, 64.0f * 1.0f + 128.0f * 0.5f + 256.0f * 3.0f), "z row is m6,m7,m8");
    Point3 v = {1, 2, 3};
    Matrix33 swap = {{0, 1, 0, 0, 0, 1, 1, 0, 0}};
    matUntransform__FP6Point3P8Matrix33T0(&v, &swap, &v);
    expect(v.x == 2 && v.y == 3 && v.z == 1, "output may alias input");
    Matrix33 order = {{1, 1, 1, 0, 0, 0, 0, 0, 0}};
    Point3 big = {16777216.0f, 1.0f, 1.0f}, o;
    matUntransform__FP6Point3P8Matrix33T0(&o, &order, &big);
    expect(same(o.x, 16777216.0f), "left-to-right sum");
    if (failures) return 1;
    std::printf("matUntransform contract passed\n");
    return 0;
}
