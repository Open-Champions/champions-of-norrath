// Observed matToQuaternion__FP4QuatP8Matrix34 at 0x157e20, extent 584.
// Converts the upper 3x3 of a Matrix34 (element [r][c] at float index 3*r + c)
// to a quaternion x, y, z, w by the usual trace method.
//   trace = m00 + m11 + m22 (summed left to right). If 0 < trace:
//     r = sqrt(trace + 1); w = r * 0.5; k = 0.5 / r;
//     x = (m12 - m21) * k, y = (m20 - m02) * k, z = (m01 - m10) * k.
//   Otherwise i is the largest diagonal element's index (i = 0; i = 1 if
//   m00 < m11; i = 2 if the chosen diagonal < m22), j and k follow the table
//   {1, 2, 0} (the three ints at 0x463138 in the image):
//     r = sqrt((m[i][i] - (m[j][j] + m[k][k])) + 1); q[i] = r * 0.5;
//     if r != 0, k = 0.5 / r (else k keeps r, which is zero);
//     w = (m[j][k] - m[k][j]) * k, q[j] = (m[i][j] + m[j][i]) * k,
//     q[k] = (m[i][k] + m[k][i]) * k.
// The positive-trace branch stores w first and then x, y, z; the other branch
// builds the result in a temporary and stores x, w, y, z at the end. A NaN
// trace takes the second branch. There is no normalization.
#include "../w8-object-list-math-common/matrix_types.hpp"

namespace {
// The original uses the sqrt.s instruction directly, with no errno fallback call.
float square_root(float value) {
#if defined(__mips__)
    float result;
    __asm__("sqrt.s %0, %1" : "=f"(result) : "f"(value));
    return result;
#else
    return __builtin_sqrtf(value);
#endif
}
}  // namespace

extern "C" void matToQuaternion__FP4QuatP8Matrix34(Quat* out, const Matrix34* matrix) {
    static const int next_axis[3] = {1, 2, 0};
    const float* m = matrix->m;
    const float trace = m[0] + m[4] + m[8];
    if (0.0f < trace) {
        const float root = square_root(trace + 1.0f);
        const float scale = 0.5f / root;
        out->w = root * 0.5f;
        out->x = (m[5] - m[7]) * scale;
        out->y = (m[6] - m[2]) * scale;
        out->z = (m[1] - m[3]) * scale;
        return;
    }
    int i = 0;
    if (m[0] < m[4]) i = 1;
    if (m[i * 4] < m[8]) i = 2;
    const int j = next_axis[i];
    const int k = next_axis[j];
    float q[4];
    float root = square_root((m[i * 4] - (m[j * 4] + m[k * 4])) + 1.0f);
    q[i] = root * 0.5f;
    if (root != 0.0f) root = 0.5f / root;
    q[3] = (m[j * 3 + k] - m[k * 3 + j]) * root;
    q[j] = (m[i * 3 + j] + m[j * 3 + i]) * root;
    q[k] = (m[i * 3 + k] + m[k * 3 + i]) * root;
    out->x = q[0];
    out->w = q[3];
    out->y = q[1];
    out->z = q[2];
}
