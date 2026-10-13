#include "calc_frustum.cpp"
#include <string.h>

static int order[40], n;
static float seen_corner[8][4];
static float seen_buf[8][4];
static float* seen_out[8];
static const float* seen_mat[8];
static const float* seen_inv[8];
static float* seen_result[8];
static int corner_i, mul_i, planes_calls;
static void* planes_arg;
extern "C" void __ml__FRC7VIVect4RC10VIMatrix44(float* r, const float* v, const float* m) {
    order[n++] = 1;
    memcpy(seen_corner[corner_i], v, 16);
    seen_inv[corner_i] = m; seen_result[corner_i] = r;
    // a result whose w is not one, so the divide is visible
    r[0] = v[0] * 2.0f; r[1] = v[1] * 3.0f; r[2] = v[2] * 5.0f; r[3] = 2.0f + corner_i;
    ++corner_i;
}
extern "C" void Mul__C10VIMatrix44RC7VIVect4R7VIVect3(const float* m, const float* v, float* out) {
    order[n++] = 2;
    memcpy(seen_buf[mul_i], v, 16); seen_out[mul_i] = out; seen_mat[mul_i] = m;
    out[0] = v[0] + 100; out[1] = v[1] + 200; out[2] = v[2] + 300;
    ++mul_i;
}
extern "C" void CalcPlanes__9VIFrustum(void* f) { order[n++] = 3; planes_arg = f; ++planes_calls; }

int main() {
    alignas(16) static unsigned char camera[512];
    alignas(16) static float inverse[16], matrix[16];
    memset(camera, 0x5a, sizeof camera);
    if (CalcFrustum__8VICameraRC10VIMatrix44T1(camera, inverse, matrix) != 0) return 1;
    if (n != 17 || planes_calls != 1 || planes_arg != camera + 304 || order[16] != 3) return 2;
    for (int i = 0; i < 8; ++i) if (order[2 * i] != 1 || order[2 * i + 1] != 2) return 3;
    const int s[8][3] = {{-1, 1, -1}, {1, 1, -1}, {1, -1, -1}, {-1, -1, -1}, {-1, 1, 1}, {1, 1, 1}, {1, -1, 1}, {-1, -1, 1}};
    for (int i = 0; i < 8; ++i) {
        if (seen_corner[i][0] != s[i][0] || seen_corner[i][1] != s[i][1] || seen_corner[i][2] != s[i][2] || seen_corner[i][3] != 1.0f) return 10 + i;
        if (seen_inv[i] != inverse || seen_mat[i] != matrix) return 20 + i;
        float w = 2.0f + i, r = 1.0f / w;
        float want[4] = {s[i][0] * 2.0f * r, s[i][1] * 3.0f * r, s[i][2] * 5.0f * r, w * r};
        for (int k = 0; k < 4; ++k) if (seen_buf[i][k] != want[k]) return 30 + 4 * i + k;
        if (seen_out[i] != reinterpret_cast<float*>(camera + 304 + 12 * i)) return 70 + i;
        float* o = reinterpret_cast<float*>(camera + 304 + 12 * i);
        if (o[0] != want[0] + 100 || o[1] != want[1] + 200 || o[2] != want[2] + 300) return 80 + i;
    }
    if (camera[303] != 0x5a || camera[400] != 0x5a) return 90;
    return 0;
}
