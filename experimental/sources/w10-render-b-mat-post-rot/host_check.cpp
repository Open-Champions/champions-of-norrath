// Host contract for matPostRot__FP8Matrix33is with stand-in trig routines
// returning fixed values. Not an EE plan input. Build with -ffp-contract=off.
#include <cstdio>
#include <cstring>
#include "../w9-render-math-b-common/math_types.hpp"
extern "C" void matPostRot__FP8Matrix33is(Matrix33*, int, short);
static int cosArg = 12345, sinArg = 12345, order, cosAt, sinAt;
extern "C" float icos__Fi(int a) { cosArg = a; cosAt = ++order; return 0.6f; }
extern "C" float isin__Fi(int a) { sinArg = a; sinAt = ++order; return 0.8f; }
static bool same(float a, float b) { return std::memcmp(&a, &b, 4) == 0; }
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    const float c = 0.6f, s = 0.8f;
    const Matrix33 base = {{1, 2, 3, 4, 5, 6, 7, 8, 9}};
    for (int axis = 0; axis < 4; ++axis) {
        Matrix33 m = base;
        order = 0;
        matPostRot__FP8Matrix33is(&m, axis, static_cast<short>(-3));
        expect(cosArg == -3 && sinArg == -3 && cosAt == 1 && sinAt == 2, "angle sign-extended; cos then sin");
        for (int r = 0; r < 9; r += 3) {
            int p = axis == 1 ? 0 : axis == 0 ? 1 : 0, q = axis == 1 ? 2 : axis == 0 ? 2 : 1;
            if (axis == 3) { for (int i = 0; i < 3; ++i) expect(m.m[r + i] == base.m[r + i], "unknown axis unchanged"); continue; }
            const float a = base.m[r + p], b = base.m[r + q];
            const float na = axis == 1 ? c * a + s * b : c * a - s * b;
            const float nb = axis == 1 ? -s * a + c * b : s * a + c * b;
            expect(same(m.m[r + p], na) && same(m.m[r + q], nb), "rotated pair");
            for (int i = 0; i < 3; ++i)
                if (i != p && i != q) expect(m.m[r + i] == base.m[r + i], "axis column unchanged");
        }
    }
    if (failures) return 1;
    std::printf("matPostRot contract passed\n");
    return 0;
}
