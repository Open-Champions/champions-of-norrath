// Host contract for worldCanPass; not an EE input.
#include "world_can_pass.cpp"
#include <cmath>
#include <cstdio>
#include <vector>
static int heights, clips; static long long blockMask; static std::vector<float> lengths; static std::vector<float> xs;
float worldFindStandHeight__FP12_worldHeaderG6Point3f(void*, const float*, float) { ++heights; return 5.0f; }
float worldClipMotion__FP12_worldHeaderG6Point3T1fiP6Point3(void*, const float* pos, const float* motion, float, int, float* result) {
    if (result) return -1.0f;
    lengths.push_back(std::sqrt(motion[0] * motion[0] + motion[1] * motion[1])); xs.push_back(pos[0]);
    int n = clips++;
    return (blockMask >> n) & 1LL ? 0.25f : 1.0f;
}
static int run(float dist, float radius, long long mask, int flag = 0) {
    float a[3] = {100.0f, 50.0f, 0.0f}, b[3] = {100.0f + dist, 50.0f, 0.0f};
    heights = clips = 0; blockMask = mask; lengths.clear(); xs.clear();
    return worldCanPass__FP12_worldHeaderG6Point3T1fi(nullptr, a, b, radius, flag);
}
int main() {
    int bad = 0;
    if (run(12.0f, 5.0f, ~0LL) != 1 || clips != 0 || heights != 0) { std::puts("fail: short"); bad = 1; }
    if (run(12.5f, 5.0f, 0) != 1 || clips != 1 || heights != 1 || std::fabs(lengths[0] - 10.0f) > 1e-3f) { std::puts("fail: one step clamped by radius"); bad = 1; }
    if (run(90.0f, 20.0f, 0) != 1 || clips != 4 || std::fabs(lengths[0] - 24.0f) > 1e-3f || std::fabs(lengths[3] - 18.0f) > 1e-3f || std::fabs(xs[1] - 124.0f) > 1e-3f) { std::puts("fail: long free walk"); bad = 1; }
    if (run(100.0f, 20.0f, 1) != 1 || clips != 5 || std::fabs(lengths[1] - 3.0f) > 1e-3f) { std::puts("fail: retry after a block"); bad = 1; }
    if (run(100.0f, 20.0f, 3) != 0 || clips != 2) { std::puts("fail: second block"); bad = 1; }
    // a block, then 20 free short steps wrap the phase to long steps
    if (run(1000.0f, 20.0f, 1) != 1 || std::fabs(lengths[1] - 3.0f) > 1e-3f || std::fabs(lengths[20] - 3.0f) > 1e-3f || std::fabs(lengths[21] - 24.0f) > 1e-3f) { std::puts("fail: phase wrap"); bad = 1; }
    if (run(1000.0f, 20.0f, 1LL | (1LL << 20)) != 0 ) { std::puts("fail: block while short"); bad = 1; }
    if (run(1000.0f, 20.0f, 1LL | (1LL << 21)) != 1 ) { std::puts("fail: block after wrap retries"); bad = 1; }
    if (!bad) std::puts("worldCanPass ok");
    return bad;
}
