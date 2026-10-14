// Host contract for quatToMat__FP8Matrix34P4Quat. Not an EE plan input.
#include <cmath>
#include <cstdio>
struct Quat { float x, y, z, w; };
extern "C" void quatToMat__FP8Matrix34P4Quat(float*, const Quat*);
int main() {
    float m[13];
    for (float& f : m) f = 7.0f;
    const float h = std::sqrt(0.5f);
    Quat qx{h, 0, 0, h};   // 90 degrees about x
    quatToMat__FP8Matrix34P4Quat(m, &qx);
    const float want[9] = {1, 0, 0, 0, 0, 1, 0, -1, 0};
    for (int i = 0; i < 9; ++i) if (std::fabs(m[i] - want[i]) > 1e-6f) return 1;
    for (int i = 9; i < 12; ++i) if (m[i] != 0.0f) return 2;
    if (m[12] != 7.0f) return 3;
    std::printf("quat to mat34 contract passed\n");
    return 0;
}
