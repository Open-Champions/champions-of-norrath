#include "transform.cpp"
#include <math.h>
#include <string.h>

int main() {
    float m[16], t[3] = {7.0f, 8.0f, 9.0f};
    float id[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    Transform__10VIMatrix44RC6VIQuatfRC7VIVect3(m, id, 2.0f, t);
    float want[16] = {2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 7, 8, 9, 1};
    if (memcmp(m, want, sizeof m) != 0) return 1;
    // quarter turn about z: x axis row -> (0, s, 0), y axis row -> (-s, 0, 0)
    float h = sqrtf(0.5f);
    float qz[4] = {0.0f, 0.0f, h, h};
    Transform__10VIMatrix44RC6VIQuatfRC7VIVect3(m, qz, 1.0f, t);
    if (fabsf(m[0]) > 1e-6f || fabsf(m[1] - 1.0f) > 1e-6f || fabsf(m[4] + 1.0f) > 1e-6f || fabsf(m[5]) > 1e-6f) return 2;
    if (m[10] != 1.0f - (0.5f * 2 * 0.0f + 0.5f * 2 * 0.0f) * 0 && fabsf(m[10] - 1.0f) > 1e-6f) return 3;
    if (m[3] != 0.0f || m[7] != 0.0f || m[11] != 0.0f || m[15] != 1.0f) return 4;
    if (m[12] != 7.0f || m[13] != 8.0f || m[14] != 9.0f) return 5;
    // the quaternion for a rotation about x: y row -> (0, c, s)
    float qx[4] = {h, 0.0f, 0.0f, h};
    Transform__10VIMatrix44RC6VIQuatfRC7VIVect3(m, qx, 1.0f, t);
    if (fabsf(m[5]) > 1e-6f || fabsf(m[6] - 1.0f) > 1e-6f || fabsf(m[9] + 1.0f) > 1e-6f) return 6;
    return 0;
}
