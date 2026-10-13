// Host contract for quatNormalize__FP4Quat. Not an EE plan input.
#include <cmath>
#include <cstdio>
struct Quat { float x, y, z, w; };
extern "C" void quatNormalize__FP4Quat(Quat*);
int main() {
    Quat q{3.0f, 0.0f, 4.0f, 0.0f};
    quatNormalize__FP4Quat(&q);
    if (std::fabs(q.x - 0.6f) > 1e-6f || q.y != 0.0f || std::fabs(q.z - 0.8f) > 1e-6f || q.w != 0.0f) return 1;
    Quat r{0.5f, 0.5f, 0.5f, 0.5f};
    quatNormalize__FP4Quat(&r);
    if (r.x != 0.5f || r.w != 0.5f) return 2;
    Quat z{0, 0, 0, 0};
    quatNormalize__FP4Quat(&z);
    if (!std::isnan(z.x)) return 3;   // 0 * inf
    std::printf("quat normalize contract passed\n");
    return 0;
}
