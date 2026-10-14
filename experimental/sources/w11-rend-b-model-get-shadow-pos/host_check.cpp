// Host contract for modelGetShadowPos. Not an EE plan input. matTransform is a recording stub.
#include <cstdio>
#include <initializer_list>
#include <cstring>
#include "../w9-render-math-b-common/math_types.hpp"
static Point3 seen; static int calls; static const Matrix34* seenMat; static Point3* seenOut;
extern "C" {
Point3* matTransform__FP6Point3P8Matrix34T0(Point3* o, const Matrix34* m, const Point3* in) { seen = *in; ++calls; seenMat = m; seenOut = o; return o; }
void modelGetShadowPos__FP12_modelHeaderP8Matrix34iP6Point3Pf(const unsigned char*, const Matrix34*, int, Point3*, float*);
}
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    alignas(16) unsigned char buf[4096] = {};
    buf[36] = 3;
    unsigned char* table = buf + 48;
    int count = 100;
    std::memcpy(table + 4, &count, 4);
    Matrix34 mat = {};
    Point3 out; float v = 99.0f;
    for (int idx : {0, 1, 44, 45, 46, 90, 99}) {
        unsigned char* e = table + 32 + idx * 8 + (idx / 45) * 16;
        short h[4] = {static_cast<short>(16 * (idx + 1)), static_cast<short>(-8 * idx), static_cast<short>(idx), static_cast<short>(-idx - 5)};
        std::memcpy(e, h, 8);
        calls = 0; v = 99.0f;
        modelGetShadowPos__FP12_modelHeaderP8Matrix34iP6Point3Pf(buf, &mat, idx, &out, &v);
        expect(calls == 1 && seenMat == &mat && seenOut == &out, "one transform call");
        expect(seen.x == 1.0f * (idx + 1) && seen.y == -0.5f * idx && seen.z == idx * 0.0625f, "point scaled by 1/16");
        expect(v == static_cast<float>(-idx - 5), "float from halfword at +6");
    }
    calls = 0;
    modelGetShadowPos__FP12_modelHeaderP8Matrix34iP6Point3Pf(buf, &mat, 100, &out, &v);
    expect(calls == 1 && seen.x == 0 && seen.y == 0 && seen.z == 0 && v == 0.0f, "index at count gives zero point and 0.0");
    if (failures) return 1;
    std::printf("modelGetShadowPos contract passed\n");
    return 0;
}
