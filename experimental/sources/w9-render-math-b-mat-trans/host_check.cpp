// Host contract for matTrans__FP8Matrix34fff. Not an EE plan input.
// Build with -ffp-contract=off so the host does not fuse multiply-adds.
#include <cstdio>
#include <cstring>
#include "../w9-render-math-b-common/math_types.hpp"
extern "C" void matTrans__FP8Matrix34fff(Matrix34*, float, float, float);
static bool same(float a, float b) { return std::memcmp(&a, &b, 4) == 0; }
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    Matrix34 a = {{1, 2, 4, 8, 16, 32, 64, 128, 256, 1000, 2000, 3000}};
    const Matrix34 keep = a;
    matTrans__FP8Matrix34fff(&a, 1.0f, 0.5f, 0.25f);
    expect(same(a.m[9], 1000.0f + (1.0f * 1 + 8.0f * 0.5f + 64.0f * 0.25f)), "x translation uses m0,m3,m6");
    expect(same(a.m[10], 2000.0f + (2.0f + 16.0f * 0.5f + 128.0f * 0.25f)), "y translation uses m1,m4,m7");
    expect(same(a.m[11], 3000.0f + (4.0f + 32.0f * 0.5f + 256.0f * 0.25f)), "z translation uses m2,m5,m8");
    bool rotKept = true;
    for (int i = 0; i < 9; ++i) rotKept = rotKept && same(a.m[i], keep.m[i]);
    expect(rotKept, "rotation floats untouched");
    // addition order: (2^24*1 + 1*1) + 1*1 must stay 2^24, while 2^24 + (1+1) would not.
    Matrix34 o = {{16777216.0f, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0}};
    matTrans__FP8Matrix34fff(&o, 1.0f, 1.0f, 1.0f);
    expect(same(o.m[9], 16777216.0f), "left-to-right sum, then added to the old translation");
    if (failures) return 1;
    std::printf("matTrans contract passed\n");
    return 0;
}
