// Host contract for quatToMat__FP8Matrix33P4Quat. Not an EE plan input.
#include <cmath>
#include <cstdio>
struct Quat { float x, y, z, w; };
extern "C" void quatToMat__FP8Matrix33P4Quat(float*, const Quat*);
int main() {
    float m[9];
    Quat id{0, 0, 0, 1};
    quatToMat__FP8Matrix33P4Quat(m, &id);
    for (int i = 0; i < 9; ++i) if (m[i] != ((i % 4) == 0 ? 1.0f : 0.0f)) return 1;
    const float h = std::sqrt(0.5f);
    Quat qz{0, 0, h, h};   // 90 degrees about z
    quatToMat__FP8Matrix33P4Quat(m, &qz);
    const float want[9] = {0, 1, 0, -1, 0, 0, 0, 0, 1};
    for (int i = 0; i < 9; ++i) if (std::fabs(m[i] - want[i]) > 1e-6f) return 2;
    Quat big{0, 0, 2 * h, 2 * h};   // scale cancels through the 2/n factor
    quatToMat__FP8Matrix33P4Quat(m, &big);
    for (int i = 0; i < 9; ++i) if (std::fabs(m[i] - want[i]) > 1e-6f) return 3;
    std::printf("quat to mat33 contract passed\n");
    return 0;
}
