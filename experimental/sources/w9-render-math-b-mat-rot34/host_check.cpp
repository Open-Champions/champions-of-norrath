// Host contract for matRot__FP8Matrix34is. Not an EE plan input.
// Build with -ffp-contract=off. icos__Fi and isin__Fi are replaced here by host
// stand-ins that return fixed, distinct values and record call order.
#include <cstdio>
#include <cstring>
#include "../w9-render-math-b-common/math_types.hpp"
static int order[4], calls = 0, lastAngle[4];
extern "C" float icos__Fi(int a) { lastAngle[calls] = a; order[calls++] = 1; return 0.75f; }
extern "C" float isin__Fi(int a) { lastAngle[calls] = a; order[calls++] = 2; return 0.25f; }
extern "C" void matRot__FP8Matrix34is(Matrix34*, int, short);
static bool same(float a, float b) { return std::memcmp(&a, &b, 4) == 0; }
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    const float c = 0.75f, s = 0.25f;
    for (int axis = -1; axis <= 3; ++axis) {
        Matrix34 m, o;
        for (int i = 0; i < 12; ++i) o.m[i] = m.m[i] = 1.0f + 0.5f * i;
        calls = 0;
        matRot__FP8Matrix34is(&m, axis, static_cast<short>(-12345));
        expect(calls == 2 && order[0] == 1 && order[1] == 2, "cosine first, sine second, always called");
        expect(lastAngle[0] == -12345 && lastAngle[1] == -12345, "angle passed sign-extended");
        float want[12];
        std::memcpy(want, o.m, sizeof want);
        auto pair = [&](int ra, int rb, int mode) {
            for (int i = 0; i < 3; ++i) {
                const float x = o.m[ra + i], y = o.m[rb + i];
                if (mode == 0) { want[ra + i] = c * x + s * y; want[rb + i] = -s * x + c * y; }
                else { want[ra + i] = c * x - s * y; want[rb + i] = s * x + c * y; }
            }
        };
        if (axis == 0) pair(3, 6, 0);
        if (axis == 1) pair(0, 6, 1);
        if (axis == 2) pair(0, 3, 0);
        bool ok = true;
        for (int i = 0; i < 12; ++i) ok = ok && same(m.m[i], want[i]);
        expect(ok, "matrix result per axis (axis outside 0..2 unchanged; translation never changes)");
    }
    if (failures) return 1;
    std::printf("matRot34 contract passed\n");
    return 0;
}
