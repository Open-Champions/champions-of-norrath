// Host contract for matMakeTransYaw__FP8Matrix34G6Point3s. Not an EE plan input.
// Build: c++ -std=c++17 -ffp-contract=off mat_make_trans_yaw.cpp \
//        ../w8-object-list-math-icos/icos.cpp ../w8-object-list-math-isin/isin.cpp host_check.cpp
#include <cmath>
#include <cstdio>
#include <cstring>
#include "../w8-object-list-math-common/matrix_types.hpp"
extern "C" void matMakeTransYaw__FP8Matrix34G6Point3s(Matrix34*, Point3, short);
extern "C" float icos__Fi(int);
extern "C" float isin__Fi(int);

static bool same_bits(float a, float b) { return std::memcmp(&a, &b, 4) == 0; }

int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    // Every halfword: cos and sin come out bit for bit as the two library routines compute them.
    int bad = 0;
    for (int yaw = -32768; yaw < 32768; ++yaw) {
        Matrix34 m;
        for (float& f : m.m) f = 123.0f;
        matMakeTransYaw__FP8Matrix34G6Point3s(&m, Point3{1.5f, -2.5f, 3.5f}, static_cast<short>(yaw));
        const float c = icos__Fi(yaw), s = isin__Fi(yaw);
        const float want[12] = {c, s, 0, -s, c, 0, 0, 0, 1, 1.5f, -2.5f, 3.5f};
        for (int i = 0; i < 12; ++i) if (!same_bits(m.m[i], want[i])) { ++bad; break; }
    }
    expect(bad == 0, "all twelve floats match for every yaw (rotation block, unit row, translation)");
    Matrix34 m;
    matMakeTransYaw__FP8Matrix34G6Point3s(&m, Point3{0, 0, 0}, 0);
    expect(m.m[0] == 1.0f && std::fabs(m.m[1]) < 2e-6f && std::fabs(m.m[3]) < 2e-6f && m.m[4] == 1.0f, "zero yaw is the identity block");
    matMakeTransYaw__FP8Matrix34G6Point3s(&m, Point3{0, 0, 0}, 16384);
    expect(std::fabs(m.m[0]) < 2e-6f && m.m[1] == 1.0f && m.m[3] == -1.0f, "quarter turn: s = 1 at float 1, -1 at float 3");
    if (failures) return 1;
    std::printf("matMakeTransYaw contract passed\n");
    return 0;
}
