// Host contract for matMakeTransYawScale__FP8Matrix34G6Point3sf. Not an EE plan input.
// Build with -ffp-contract=off. The expected cos and sin come from the shared
// polynomial used by the authored icos__Fi and isin__Fi.
#include <cstdio>
#include <cstring>
#include "../w9-render-math-b-common/math_types.hpp"
#include "../w8-object-list-math-common/trig_core.hpp"
extern "C" void matMakeTransYawScale__FP8Matrix34G6Point3sf(Matrix34*, Point3, short, float);
static bool same(float a, float b) { return std::memcmp(&a, &b, 4) == 0; }
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    const float scales[] = {1.0f, 2.5f, -0.75f, 0.0f};
    int bad = 0;
    for (float k : scales) {
        for (int yaw = -32768; yaw < 32768; ++yaw) {
            Matrix34 m;
            for (float& f : m.m) f = 123.0f;
            matMakeTransYawScale__FP8Matrix34G6Point3sf(&m, Point3{1.5f, -2.5f, 3.5f}, static_cast<short>(yaw), k);
            const float c = trig_core::evaluate(static_cast<short>(yaw)), s = trig_core::evaluate(static_cast<short>(yaw - 16384));
            const float want[12] = {c * k, s * k, 0, -(s * k), c * k, 0, 0, 0, k, 1.5f, -2.5f, 3.5f};
            for (int i = 0; i < 12; ++i) if (!same(m.m[i], want[i])) { ++bad; break; }
        }
    }
    expect(bad == 0, "all twelve floats match for every yaw and scale");
    Matrix34 m;
    matMakeTransYawScale__FP8Matrix34G6Point3sf(&m, Point3{0, 0, 0}, 16384, 2.0f);
    expect(m.m[1] == 2.0f && m.m[3] == -2.0f && m.m[8] == 2.0f, "quarter turn, scale 2");
    if (failures) return 1;
    std::printf("matMakeTransYawScale contract passed\n");
    return 0;
}
