// Observed matMul__FP8Matrix34N20 at 0x001564b8, extent 344. (out, a, b) -> out,
// with all three Matrix34. The twelve results are built in a stack temporary and
// copied to out in 48 bytes at the end, so out may be a or b. With a and b read as
// 3x3 rotation blocks (floats 0..8) plus a translation row (floats 9..11):
//   rows 0..2, columns j = 0..2:
//     t[3i+j] = (a[3i]*b[j] + a[3i+1]*b[3+j]) + a[3i+2]*b[6+j]
//   translation row, j = 0..2:
//     t[9+j]  = ((a[9]*b[j] + a[10]*b[3+j]) + a[11]*b[6+j]) + b[9+j]
// Each sum is evaluated left to right as written. Returns the out pointer.
// Inferred: composition of two row-vector affine transforms (a applied first).
// Unknown: whether any caller relies on that reading.
#include "../w9-render-math-b-common/math_types.hpp"

extern "C" Matrix34* matMul__FP8Matrix34N20(Matrix34* out, const Matrix34* a, const Matrix34* b) {
    float t[12];
    for (int i = 0; i < 3; ++i) {
        const float* row = a->m + 3 * i;
        for (int j = 0; j < 3; ++j) {
            t[3 * i + j] = (row[0] * b->m[j] + row[1] * b->m[3 + j]) + row[2] * b->m[6 + j];
        }
    }
    const float* row = a->m + 9;
    for (int j = 0; j < 3; ++j) {
        t[9 + j] = ((row[0] * b->m[j] + row[1] * b->m[3 + j]) + row[2] * b->m[6 + j]) + b->m[9 + j];
    }
    for (int k = 0; k < 12; ++k) out->m[k] = t[k];
    return out;
}
