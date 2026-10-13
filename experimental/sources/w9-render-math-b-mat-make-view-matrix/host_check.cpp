// Host contract for matMakeViewMatrix__FP8Matrix44fffffff. Not an EE plan input.
// Build with -ffp-contract=off.
#include <cmath>
#include <cstdio>
#include <cstring>
#include "../w9-render-math-b-common/math_types.hpp"
extern "C" void matMakeViewMatrix__FP8Matrix44fffffff(Matrix44*, float, float, float, float, float, float, float);
static bool same(float a, float b) { return std::memcmp(&a, &b, 4) == 0; }
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    struct { float before[2]; Matrix44 matrix; float after[2]; } box;
    for (float& f : box.matrix.m) f = 7.5f;
    box.before[0] = box.before[1] = box.after[0] = box.after[1] = -3.0f;
    const float a = 0.75f, b = 1.25f, c = 2.0f, d = 0.5f, e = 100.0f, f = 3.0f, g = 11.0f;
    matMakeViewMatrix__FP8Matrix44fffffff(&box.matrix, a, b, c, d, e, f, g);
    const float* m = box.matrix.m;
    expect(same(m[0], c * a) && same(m[5], c * b), "diagonal scales are c*a and c*b");
    expect(m[11] == 1.0f, "float 11 is 1");
    expect(same(m[10], e - ((e - d) * g) / (g - f)), "float 10");
    expect(same(m[14], (((e - d) * g) * f) / (g - f)), "float 14");
    const int zeros[] = {1, 2, 3, 4, 6, 7, 8, 9, 12, 13, 15};
    for (int i : zeros) expect(m[i] == 0.0f && !std::signbit(m[i]), "zero floats");
    expect(box.before[0] == -3.0f && box.before[1] == -3.0f && box.after[0] == -3.0f && box.after[1] == -3.0f, "no write outside sixteen floats");
    matMakeViewMatrix__FP8Matrix44fffffff(&box.matrix, a, b, c, d, e, 5.0f, 5.0f);
    expect(std::isinf(box.matrix.m[10]) || std::isnan(box.matrix.m[10]), "g == f is unguarded");
    if (failures) return 1;
    std::printf("matMakeViewMatrix contract passed\n");
    return 0;
}
