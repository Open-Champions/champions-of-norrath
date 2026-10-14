// Host contract for matMakeYPRScale34_ReallySlow. Not an EE plan input. matRot is a recording stub.
#include <cstdio>
#include <cstring>
#include "../w9-render-math-b-common/math_types.hpp"
static int axes[3], n; static short angles[3]; static float before[3][12];
extern "C" {
void matRot__FP8Matrix34is(Matrix34* m, int axis, short a) {
    axes[n] = axis; angles[n] = a; std::memcpy(before[n], m->m, 48); ++n;
    m->m[0] += 2.0f;  // make the later scale visible
}
void matMakeYPRScale34_ReallySlow__FP8Matrix34G6Point3sssf(Matrix34*, Point3, short, short, short, float);
}
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    Matrix34 m;
    for (float& f : m.m) f = 77.0f;
    matMakeYPRScale34_ReallySlow__FP8Matrix34G6Point3sssf(&m, Point3{1.5f, -2.5f, 3.5f}, 0x7fff, 11, -12, 0.5f);
    expect(n == 3 && axes[0] == 2 && axes[1] == 1 && axes[2] == 0, "axes 2, 1, 0 in order");
    expect(angles[0] == static_cast<short>(0x7fff + 0x4000) && angles[1] == 11 && angles[2] == -12, "angles");
    const float want[12] = {1, 0, 0, 0, 1, 0, 0, 0, 1, 1.5f, -2.5f, 3.5f};
    for (int i = 0; i < 12; ++i) expect(before[0][i] == want[i], "identity plus translation before the first rotation");
    expect(m.m[0] == (1.0f + 2.0f * 3) * 0.5f, "rotation floats scaled after the rotations");
    expect(m.m[4] == 0.5f && m.m[8] == 0.5f && m.m[9] == 1.5f && m.m[11] == 3.5f, "diagonal scaled, translation not");
    if (failures) return 1;
    std::printf("matMakeYPRScale34_ReallySlow contract passed\n");
    return 0;
}
