// Host contract for matMakeWorldViewMatrix__FP8Matrix44fffffff. Not an EE plan input.
// Build with -ffp-contract=off.
#include <cstdio>
#include <initializer_list>
#include <cstring>
#include "../w9-render-math-b-common/math_types.hpp"
extern "C" void matMakeWorldViewMatrix__FP8Matrix44fffffff(Matrix44*, float, float, float, float, float, float, float);
static bool same(float a, float b) { return std::memcmp(&a, &b, 4) == 0; }
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    Matrix44 m;
    for (int i = 0; i < 16; ++i) m.m[i] = 7.0f;
    const float p1 = 1.5f, p2 = 0.75f, p3 = 0.5f, p4 = 2.0f, p5 = 10.0f, p6 = 1.0f, p7 = 100.0f;
    matMakeWorldViewMatrix__FP8Matrix44fffffff(&m, p1, p2, p3, p4, p5, p6, p7);
    const float span = p5 - p4, den = p7 - p6, scale = 16.0f * 70368744177664.0f;
    for (int i : {1, 2, 3, 4, 6, 7, 8, 9, 12, 13, 15}) expect(m.m[i] == 0.0f, "zero entries");
    expect(same(m.m[0], p3 * p1), "m[0]");
    expect(same(m.m[5], p3 * p2), "m[5]");
    expect(m.m[11] == 1.0f, "m[11]");
    expect(same(m.m[10], (p5 - (span * p7) / den) * 16.0f * 70368744177664.0f), "m[10]");
    expect(same(m.m[14], ((span * p7 * p6) / den) * 16.0f * 70368744177664.0f), "m[14]");
    (void)scale;
    if (failures) return 1;
    std::printf("matMakeWorldViewMatrix contract passed\n");
    return 0;
}
