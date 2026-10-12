// Host contract for matTransform34Vec__FP6Point3P8Matrix34T0. Not an EE plan input.
// Build: c++ -std=c++17 -ffp-contract=off <source>.cpp host_check.cpp (no fused multiply-add on the host).
#include <cmath>
#include <cstdio>
#include <cstring>
#include "../w8-object-list-math-common/matrix_types.hpp"
extern "C" Point3* matTransform34Vec__FP6Point3P8Matrix34T0(Point3*, const Matrix34*, const Point3*);

static bool same_bits(float a, float b) { return std::memcmp(&a, &b, 4) == 0; }

int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    Matrix34 matrix = {{1.5f, 2.0f, 4.0f, 8.0f, 16.0f, 32.0f, 64.0f, 128.0f, 256.0f, NAN, NAN, NAN}};
    Point3 in = {1.0f, 1.0f / 1024.0f, 3.0f};
    Point3 out = {-1, -1, -1};
    Point3* result = matTransform34Vec__FP6Point3P8Matrix34T0(&out, &matrix, &in);
    expect(result == &out, "returns the output pointer");
    const float x = 1.5f * 1.0f + 8.0f * (1.0f / 1024.0f) + 64.0f * 3.0f;
    const float y = 2.0f * 1.0f + 16.0f * (1.0f / 1024.0f) + 128.0f * 3.0f;
    const float z = 4.0f * 1.0f + 32.0f * (1.0f / 1024.0f) + 256.0f * 3.0f;
    expect(same_bits(out.x, x) && same_bits(out.y, y) && same_bits(out.z, z), "columns are m0/m3/m6, m1/m4/m7, m2/m5/m8");
    expect(!std::isnan(out.x) && !std::isnan(out.y) && !std::isnan(out.z), "the trailing floats are not read");
    Point3 v = {1.0f, 2.0f, 3.0f};
    Matrix34 swap = {{0, 1, 0, 0, 0, 1, 1, 0, 0, 9, 9, 9}};
    matTransform34Vec__FP6Point3P8Matrix34T0(&v, &swap, &v);
    expect(v.x == 3.0f && v.y == 1.0f && v.z == 2.0f, "output may alias input");
    Matrix34 order = {{1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0}};
    Point3 big = {16777216.0f, 1.0f, 1.0f}, r;
    matTransform34Vec__FP6Point3P8Matrix34T0(&r, &order, &big);
    expect(r.x == 16777216.0f, "(2^24 + 1) + 1 stays at 2^24");
    if (failures) return 1;
    std::printf("matTransform34Vec contract passed\n");
    return 0;
}
