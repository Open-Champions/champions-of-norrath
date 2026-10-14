// Host contract for matMul__FP8Matrix44P8Matrix34T0. Not an EE plan input.
// Build with -ffp-contract=off.
#include <cstdio>
#include <cstring>
#include "../w9-render-math-b-common/math_types.hpp"
extern "C" Matrix44* matMul__FP8Matrix44P8Matrix34T0(Matrix44*, const Matrix34*, const Matrix44*);
static bool same(float a, float b) { return std::memcmp(&a, &b, 4) == 0; }
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    Matrix34 a;
    Matrix44 b, out;
    for (int i = 0; i < 12; ++i) a.m[i] = 0.5f + i * 0.25f;
    for (int i = 0; i < 16; ++i) { b.m[i] = 2.0f - i * 0.125f; out.m[i] = 99.0f; }
    Matrix44* r = matMul__FP8Matrix44P8Matrix34T0(&out, &a, &b);
    expect(r == &out, "returns out");
    bool ok = true;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 4; ++j)
            ok = ok && same(out.m[4 * i + j], (a.m[3 * i] * b.m[j] + a.m[3 * i + 1] * b.m[4 + j]) + a.m[3 * i + 2] * b.m[8 + j]);
    for (int j = 0; j < 4; ++j)
        ok = ok && same(out.m[12 + j], ((a.m[9] * b.m[j] + a.m[10] * b.m[4 + j]) + a.m[11] * b.m[8 + j]) + b.m[12 + j]);
    expect(ok, "all sixteen results");
    Matrix44 want = out;
    Matrix44 b2 = b;
    matMul__FP8Matrix44P8Matrix34T0(&b2, &a, &b2);
    expect(std::memcmp(&b2, &want, sizeof want) == 0, "out may be b");
    Matrix34 id = {{1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0}};
    Matrix44 res;
    matMul__FP8Matrix44P8Matrix34T0(&res, &id, &b);
    bool idok = true;
    for (int k = 0; k < 16; ++k) idok = idok && same(res.m[k], b.m[k] + 0.0f);
    expect(idok, "identity a reproduces b (zero translation adds nothing)");
    Matrix34 order = {{16777216.0f, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
    Matrix44 ones = {{1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0}};
    matMul__FP8Matrix44P8Matrix34T0(&res, &order, &ones);
    expect(same(res.m[0], 16777216.0f), "left-to-right sum");
    if (failures) return 1;
    std::printf("matMul44 contract passed\n");
    return 0;
}
