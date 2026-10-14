#include "transform.cpp"
#include <math.h>
#include <string.h>

int main() {
    float m[16], t[3] = {7.0f, 8.0f, 9.0f}, sc[3] = {2.0f, 3.0f, 4.0f};
    float id[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    Transform__10VIMatrix44RC6VIQuatRC7VIVect3T2(m, id, sc, t);
    float want[16] = {2, 0, 0, 0, 0, 3, 0, 0, 0, 0, 4, 0, 7, 8, 9, 1};
    if (memcmp(m, want, sizeof m) != 0) return 1;
    float h = sqrtf(0.5f);
    float qz[4] = {0.0f, 0.0f, h, h};
    Transform__10VIMatrix44RC6VIQuatRC7VIVect3T2(m, qz, sc, t);
    // quarter turn about z: first row scaled by sc[0], second row by sc[1]
    if (fabsf(m[0]) > 1e-6f || fabsf(m[1] - 2.0f) > 1e-5f || fabsf(m[4] + 3.0f) > 1e-5f || fabsf(m[5]) > 1e-6f) return 2;
    if (fabsf(m[10] - 4.0f) > 1e-5f) return 3;
    if (m[3] != 0.0f || m[7] != 0.0f || m[11] != 0.0f || m[15] != 1.0f) return 4;
    if (m[12] != 7.0f || m[13] != 8.0f || m[14] != 9.0f) return 5;
    return 0;
}
