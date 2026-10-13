// Observed findClosestShadowBall__13MissileWeaponP10GameObject at 0x00147278, 468 bytes. Arguments: the
// missile object M and another object T (may be null). Returns an int index. Let model be the result of the
// table entry at offset 128 of T's table (the table address is the word at T+72) called on T adjusted by the
// entry's halfword, and count be modelGetNmShadowPoints(model) when model is non-null, else 0.
//  1. Null T, null model or a count of 0 return 0.
//  2. Pass one, for i = 0 .. count-1: modelGetShadowPos(model, T+288, a copy of the position at T+16, the
//     signed halfword at T+56, i, a Point3 output, a float output, null, the float at T+224) is called; the
//     largest float output seen, starting from 0.0, is kept as top.
//  3. Pass two, for i = 0 .. count-1: the same call is made again. An index is skipped when its float output is
//     below top * 0.75. Otherwise the distance from M's position (floats at M+16..M+24) to the Point3 output
//     is computed, and when it is below the best distance so far (initially 1.0e8, the float at 0x0047fd34
//     in the image, which no symbol names) the index and the distance are kept.
//  4. The kept index is returned (0 when none was kept).
// Inferred: the call returns the shadow-point index of T nearest to M among those whose float output is near
// the maximum. What the float output is, and what the shadow points mean, are unknown. The matrix argument is
// passed as null. Nothing here ran on the EE.
#include "../w11-actor-a-common/access.hpp"

extern "C" {
int modelGetNmShadowPoints__FP12_modelHeader(void* model);
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
    void* model, void* animation, Point3 position, short heading, int index, Point3* outPoint, float* outValue, void* matrix, float scale);

int findClosestShadowBall__13MissileWeaponP10GameObject(void* self, void* other) {
    using namespace w11a;
    if (other == nullptr) return 0;
    unsigned char* const m = static_cast<unsigned char*>(self);
    unsigned char* const t = static_cast<unsigned char*>(other);
    void* const model = reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(call_entry(t, 128)));
    int count = 0;
    if (model != nullptr) count = modelGetNmShadowPoints__FP12_modelHeader(model);
    if (count == 0) return 0;
    float top = 0.0f;
    int result = 0;
    float best = bits(0x4cbebc20u);
    Point3 point;
    float value;
    for (int i = 0; i < count; ++i) {
        modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
            model, t + 288, *reinterpret_cast<Point3*>(t + 16), half(t, 56), i, &point, &value, nullptr, real(t, 224));
        if (top < value) top = value;
    }
    for (int i = 0; i < count; ++i) {
        modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
            model, t + 288, *reinterpret_cast<Point3*>(t + 16), half(t, 56), i, &point, &value, nullptr, real(t, 224));
        if (value < top * bits(0x3f400000u)) continue;
        const float dx = real(m, 16) - point.x;
        const float dy = real(m, 20) - point.y;
        const float dz = real(m, 24) - point.z;
        const float distance = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
        if (distance < best) {
            result = i;
            best = distance;
        }
    }
    return result;
}
}
