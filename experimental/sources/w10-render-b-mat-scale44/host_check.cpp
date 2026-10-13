// Host contract for matScale__FP8Matrix44f. Not an EE plan input. Build with -ffp-contract=off.
#include <cstdio>
#include <cstring>
#include "../w9-render-math-b-common/math_types.hpp"
extern "C" void matScale__FP8Matrix44f(Matrix44*, float);
static bool same(float a, float b) { return std::memcmp(&a, &b, 4) == 0; }
int main() {
    int failures = 0;
    Matrix44 m;
    for (int i = 0; i < 16; ++i) m.m[i] = static_cast<float>(i + 1) * 0.37f;
    const Matrix44 before = m;
    matScale__FP8Matrix44f(&m, 2.5f);
    for (int i = 0; i < 16; ++i) {
        const bool scaled = i < 12 && (i & 3) != 3;
        const float want = scaled ? before.m[i] * 2.5f : before.m[i];
        if (!same(m.m[i], want)) { std::fprintf(stderr, "fail: element %d\n", i); ++failures; }
    }
    if (failures) return 1;
    std::printf("matScale44 contract passed\n");
    return 0;
}
