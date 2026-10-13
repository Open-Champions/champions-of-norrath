#include "../w11-rend-a-common/rend.hpp"
// Observed DrawWebSplay__FG6Point3iT0iiifP10GameObjectP12_modelHeaderP14AnimationState at
// 0x0020d520, 1864 bytes. Arguments: a point p0 and an int a, a point p1 and an int b (points are
// passed by value, so the callee gets pointers to copies), ints count and sub, the float step0 (in
// f12), a GameObject pointer obj, a _modelHeader pointer model, and an AnimationState pointer anim
// (the ninth integer-class argument, on the stack). Words used: obj+16 (three floats) and the signed
// halfword obj+56.
// Nothing happens when a and b are both zero, or count is zero (a negative count draws nothing too).
//  1. With count >= 2: d = p1 - p0; the sideways step w = (-d.y, d.x, d.z) is scaled to length one by
//     1.0f / sqrt(|w|^2) (the compiler's rsqrt.s) and then multiplied by step0 / (float)(count - 1).
//     With a smaller count w stays zero in this source (the image leaves the stack slot unwritten).
//  2. The shadow-point count n is 0, or modelGetNmShadowPoints__FP12_modelHeader(model) when obj and
//     model are both nonzero. The random state starts as obj's own address.
//  3. For s = 0 .. count - 1 an end point A is chosen. With obj and n nonzero:
//     modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(model, anim,
//     the three floats at obj+16, (short)obj[56], (state >> 16) % n (unsigned), &A, &scratch, 0, 1.0f),
//     then state = state * 0x19660d + 0x3c6ef35f. Otherwise A = p1 + w * (float)((s + 1) / 2) times
//     -1.0f for even s and +1.0f for odd s.
//  4. P_Line(p0, A, 255, 255, 255, a & 255, 255, 255, 255, b & 255, 0.0f).
//  5. With s != 0 and sub != 0, a bundle of sub extra lines is drawn between A and the end point of two
//     steps earlier, Q (the end point of step 0 stands in for step 1): e1 = (A - p0) / sub and
//     e2 = (Q - p0) / sub (each as multiplication by 1.0f / (float)sub), then A -= e1 * 0.25 and
//     Q -= e2 * 0.25. Then sub times: e1 and e2 are multiplied by the float constant 0x1.99999p-1
//     (0.8 truncated, read from the image), A -= e1, Q -= e2 and
//     P_Line(A, Q, 255, 255, 255, m, 255, 255, 255, m, 0.0f) with m = ((max(a, b)) / 4) & 255
//     (the image rounds toward zero by adding three to a negative value before the shift).
// Nothing is returned.
// Inferred: a web spray fanning between two points, alternating sides and optionally following
// shadow points of a model. a and b act as the fourth colour byte at the two ends of each line.
// Unknown: what the first four integers and the float mean to callers, and the GameObject and
// _modelHeader layouts beyond obj+16 and obj+56. Nothing here was executed on the EE.
extern "C" {
unsigned modelGetNmShadowPoints__FP12_modelHeader(unsigned char* model);
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
    unsigned char* model, unsigned char* anim, const float* position, short angle, int index, float* out,
    float* outFloat, unsigned char* matrix, float scale);
void P_Line__FG6Point3T0UcUcUcUcUcUcUcUcf(const float* from, const float* to, unsigned char a, unsigned char b,
                                         unsigned char c, unsigned char d, unsigned char e, unsigned char f,
                                         unsigned char g, unsigned char h, float width);
void DrawWebSplay__FG6Point3iT0iiifP10GameObjectP12_modelHeaderP14AnimationState(
    const float* start, int a, const float* end, int b, int count, int sub, float step0, unsigned char* obj,
    unsigned char* model, unsigned char* anim) {
    using namespace rend;
    float p0[3] = {start[0], start[1], start[2]};
    float p1[3] = {end[0], end[1], end[2]};
    if (a == 0 && b == 0) return;
    if (count == 0) return;
    float w[3] = {0.0f, 0.0f, 0.0f};
    if (count >= 2) {
        const float d0 = p1[0] - p0[0];
        const float d1 = p1[1] - p0[1];
        const float d2 = p1[2] - p0[2];
        const float step = step0 / static_cast<float>(count - 1);
        w[0] = -d1;
        w[1] = d0;
        w[2] = d2;
        const float r = 1.0f / __builtin_sqrtf(w[0] * w[0] + w[1] * w[1] + w[2] * w[2]);
        w[0] = w[0] * r;
        w[1] = w[1] * r;
        w[2] = w[2] * r;
        w[0] = w[0] * step;
        w[1] = w[1] * step;
        w[2] = w[2] * step;
    }
    unsigned points = 0;
    unsigned state;
#ifdef NORRATH_HOST_ARENA
    state = obj == nullptr ? 0u : norrath_host_address(obj);
#else
    state = reinterpret_cast<unsigned>(obj);
#endif
    if (obj != nullptr && model != nullptr) points = modelGetNmShadowPoints__FP12_modelHeader(model);
    if (count <= 0) return;
    const unsigned char alphaA = static_cast<unsigned char>(a);
    const unsigned char alphaB = static_cast<unsigned char>(b);
    float prev1[3] = {0.0f, 0.0f, 0.0f};
    float prev2[3] = {0.0f, 0.0f, 0.0f};
    int s = 0;
    do {
        float A[3];
        const int next = s + 1;
        if (obj != nullptr && points != 0) {
            const float position[3] = {at<f32>(obj, 16), at<f32>(obj, 20), at<f32>(obj, 24)};
            float scratch;
            modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
                model, anim, position, at<i16>(obj, 56), static_cast<int>((state >> 16) % points), A, &scratch, nullptr,
                1.0f);
            state = state * 0x19660du + 0x3c6ef35fu;
        } else {
            const float k = static_cast<float>(next / 2);
            const float u0 = w[0] * k;
            const float u1 = w[1] * k;
            const float u2 = w[2] * k;
            float sign = 1.0f;
            if ((s & 1) == 0) sign = -1.0f;
            A[0] = p1[0] + u0 * sign;
            A[1] = p1[1] + u1 * sign;
            A[2] = p1[2] + u2 * sign;
        }
        const float cur[3] = {A[0], A[1], A[2]};
        if (s == 0) {
            for (int i = 0; i < 3; ++i) prev1[i] = A[i];
            for (int i = 0; i < 3; ++i) prev2[i] = prev1[i];
        }
        P_Line__FG6Point3T0UcUcUcUcUcUcUcUcf(p0, A, 255, 255, 255, alphaA, 255, 255, 255, alphaB, 0.0f);
        if (s != 0 && sub != 0) {
            float q[3] = {prev2[0], prev2[1], prev2[2]};
            const float inv = 1.0f / static_cast<float>(sub);
            float e1[3] = {A[0] - p0[0], A[1] - p0[1], A[2] - p0[2]};
            float e2[3] = {q[0] - p0[0], q[1] - p0[1], q[2] - p0[2]};
            for (int i = 0; i < 3; ++i) e1[i] = e1[i] * inv;
            for (int i = 0; i < 3; ++i) e2[i] = e2[i] * inv;
            float t1[3] = {e1[0] * 0.25f, e1[1] * 0.25f, e1[2] * 0.25f};
            for (int i = 0; i < 3; ++i) A[i] = A[i] - t1[i];
            float t2[3] = {e2[0] * 0.25f, e2[1] * 0.25f, e2[2] * 0.25f};
            for (int i = 0; i < 3; ++i) q[i] = q[i] - t2[i];
            if (sub > 0) {
                int m = b < a ? a : b;
                if (m < 0) m += 3;
                const unsigned char level = static_cast<unsigned char>(m >> 2);
                int n = sub;
                do {
                    for (int i = 0; i < 3; ++i) e1[i] = e1[i] * 0x1.999998p-1f;
                    for (int i = 0; i < 3; ++i) e2[i] = e2[i] * 0x1.999998p-1f;
                    for (int i = 0; i < 3; ++i) A[i] = A[i] - e1[i];
                    for (int i = 0; i < 3; ++i) q[i] = q[i] - e2[i];
                    P_Line__FG6Point3T0UcUcUcUcUcUcUcUcf(A, q, 255, 255, 255, level, 255, 255, 255, level, 0.0f);
                    --n;
                } while (n != 0);
            }
        }
        for (int i = 0; i < 3; ++i) prev2[i] = prev1[i];
        for (int i = 0; i < 3; ++i) prev1[i] = cur[i];
        s = next;
    } while (s < count);
}
}
