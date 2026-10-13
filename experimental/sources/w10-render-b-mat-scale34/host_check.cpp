// Host contract for matScale__FP8Matrix34f. Not an EE plan input. Build with -ffp-contract=off.
#include <cstdio>
#include <cstring>
#include "../w9-render-math-b-common/math_types.hpp"
extern "C" void matScale__FP8Matrix34f(Matrix34*, float);
static bool same(float a, float b) { return std::memcmp(&a, &b, 4) == 0; }
int main() {
    int failures = 0;
    Matrix34 m;
    for (int i = 0; i < 12; ++i) m.m[i] = static_cast<float>(i + 1) * 0.37f;
    const Matrix34 before = m;
    matScale__FP8Matrix34f(&m, 2.5f);
    for (int i = 0; i < 9; ++i)
        if (!same(m.m[i], before.m[i] * 2.5f)) { std::fprintf(stderr, "fail: element %d\n", i); ++failures; }
    for (int i = 9; i < 12; ++i)
        if (!same(m.m[i], before.m[i])) { std::fprintf(stderr, "fail: translation %d changed\n", i); ++failures; }
    if (failures) return 1;
    std::printf("matScale34 contract passed\n");
    return 0;
}
