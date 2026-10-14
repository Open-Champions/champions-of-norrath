#include "slerp.cpp"
#include <math.h>
#include <string.h>

int main() {
    // identical quaternions take the linear path: weights (1-t, t)
    float a[4] = {0.0f, 0.0f, 0.0f, 1.0f}, o[4];
    Slerp__6VIQuatRC6VIQuatT1f(o, a, a, 0.25f);
    float lin = a[3] * (1.0f - 0.25f) + a[3] * 0.25f;
    if (o[3] != lin || o[0] != 0.0f) return 1;
    // a quarter turn about z, half way: 45 degrees
    float h = sqrtf(0.5f);
    float b[4] = {0.0f, 0.0f, h, h};
    Slerp__6VIQuatRC6VIQuatT1f(o, a, b, 0.5f);
    float theta = acosf(a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3]);
    float inv = 1.0f / sinf(theta);
    float wa = sinf((1.0f - 0.5f) * theta) * inv, wb = sinf(0.5f * theta) * inv;
    for (int i = 0; i < 4; ++i)
        if (o[i] != a[i] * wa + b[i] * wb) return 2;
    if (fabsf(o[2] - sinf(0.3926990817f)) > 1e-5f) return 3;
    // opposite hemisphere: b is negated
    float nb[4] = {0.0f, 0.0f, -h, -h};
    float o2[4];
    Slerp__6VIQuatRC6VIQuatT1f(o2, a, nb, 0.5f);
    for (int i = 0; i < 4; ++i)
        if (fabsf(o2[i] - o[i]) > 1e-6f) return 4;
    // opposite and nearly equal: linear with -t
    float na[4] = {0.0f, 0.0f, 0.0f, -1.0f}, o3[4];
    Slerp__6VIQuatRC6VIQuatT1f(o3, a, na, 0.25f);
    if (o3[3] != a[3] * (1.0f - 0.25f) + na[3] * -0.25f) return 5;
    return 0;
}
