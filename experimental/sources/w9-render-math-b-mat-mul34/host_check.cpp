// Host contract for matMul__FP8Matrix34N20. Not an EE plan input.
// Build with -ffp-contract=off.
#include <cstdio>
#include <cstring>
#include "../w9-render-math-b-common/math_types.hpp"
extern "C" Matrix34* matMul__FP8Matrix34N20(Matrix34*, const Matrix34*, const Matrix34*);
static bool same(float a, float b) { return std::memcmp(&a, &b, 4) == 0; }
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    Matrix34 a, b, out;
    for (int i = 0; i < 12; ++i) { a.m[i] = 0.5f + i * 0.25f; b.m[i] = 2.0f - i * 0.125f; out.m[i] = 99.0f; }
    Matrix34* r = matMul__FP8Matrix34N20(&out, &a, &b);
    expect(r == &out, "returns out");
    bool ok = true;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            ok = ok && same(out.m[3 * i + j], (a.m[3 * i] * b.m[j] + a.m[3 * i + 1] * b.m[3 + j]) + a.m[3 * i + 2] * b.m[6 + j]);
    for (int j = 0; j < 3; ++j)
        ok = ok && same(out.m[9 + j], ((a.m[9] * b.m[j] + a.m[10] * b.m[3 + j]) + a.m[11] * b.m[6 + j]) + b.m[9 + j]);
    expect(ok, "all twelve results");
    // aliasing: out == a and out == b
    Matrix34 want = out;
    Matrix34 a2 = a, b2 = b;
    matMul__FP8Matrix34N20(&a2, &a2, &b2);
    expect(std::memcmp(&a2, &want, sizeof want) == 0, "out may be a");
    a2 = a; b2 = b;
    matMul__FP8Matrix34N20(&b2, &a2, &b2);
    expect(std::memcmp(&b2, &want, sizeof want) == 0, "out may be b");
    Matrix34 id = {{1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0}};
    Matrix34 tr = {{1, 0, 0, 0, 1, 0, 0, 0, 1, 3, 4, 5}}, res;
    matMul__FP8Matrix34N20(&res, &id, &tr);
    expect(res.m[9] == 3 && res.m[10] == 4 && res.m[11] == 5, "translation of b is added once");
    matMul__FP8Matrix34N20(&res, &tr, &id);
    expect(res.m[9] == 3 && res.m[10] == 4 && res.m[11] == 5, "translation of a is carried");
    Matrix34 order = {{16777216.0f, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
    Matrix34 ones = {{1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0}};
    matMul__FP8Matrix34N20(&res, &order, &ones);
    expect(same(res.m[0], 16777216.0f), "left-to-right sum");
    if (failures) return 1;
    std::printf("matMul34 contract passed\n");
    return 0;
}
