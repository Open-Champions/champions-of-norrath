// Host contract for matTrans__FP8Matrix44fff. Not an EE plan input. Build with -ffp-contract=off.
#include <cstdio>
#include <cstring>
#include "../w9-render-math-b-common/math_types.hpp"
extern "C" void matTrans__FP8Matrix44fff(Matrix44*, float, float, float);
static bool same(float a, float b) { return std::memcmp(&a, &b, 4) == 0; }
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    Matrix44 m;
    for (int i = 0; i < 16; ++i) m.m[i] = static_cast<float>(i + 1) * 0.37f;
    const Matrix44 b = m;
    matTrans__FP8Matrix44fff(&m, 1.5f, -2.0f, 0.75f);
    for (int i = 0; i < 12; ++i) expect(same(m.m[i], b.m[i]), "rows 0..2 unchanged");
    for (int j = 0; j < 4; ++j)
        expect(same(m.m[12 + j], b.m[12 + j] + ((b.m[j] * 1.5f + b.m[4 + j] * -2.0f) + b.m[8 + j] * 0.75f)),
               "translation row entry");
    Matrix44 id = {{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 5, 6, 7, 1}};
    matTrans__FP8Matrix44fff(&id, 1, 2, 3);
    expect(id.m[12] == 6 && id.m[13] == 8 && id.m[14] == 10 && id.m[15] == 1, "identity adds displacement");
    if (failures) return 1;
    std::printf("matTrans contract passed\n");
    return 0;
}
