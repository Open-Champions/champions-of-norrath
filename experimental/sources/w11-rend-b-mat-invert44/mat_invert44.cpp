// Observed matInvert__FP8Matrix44T0 at 0x00157a90, extent 744. Arguments: output
// Matrix44 and input Matrix44 (sixteen floats each, row-major with a row stride of four).
// Returns nothing. The input is copied to a stack work matrix first, and the output is set
// to the identity, so the output may alias the input.
//
// The algorithm is Gauss-Jordan elimination over COLUMNS, with the pivot chosen along a
// row. For i = 0 to 3:
//  * the pivot is work[i][i]; each work[i][j] with j > i replaces it when its magnitude is
//    strictly greater, remembering j;
//  * if the pivot column j is not i, columns i and j are swapped in both the work matrix
//    and the output;
//  * if the pivot's magnitude is at most 0.0001f (float bits 0x38d1b717, a literal read at
//    0x0047fef4) the output is set to the identity and the function returns;
//  * column i of both matrices is multiplied by 1.0f / pivot;
//  * for every other column j, with f = work[i][j], column j of both matrices has
//    f times column i subtracted from it, one row at a time.
// When the work matrix is the one passed in, the output is its inverse, apart from the
// singular case which gives the identity. Nothing else is touched.
// Inferred: that this computes the inverse (the name and structure say so).
// Unknown: whether callers rely on the singular-case identity.
#include "../w9-render-math-b-common/math_types.hpp"

extern "C" {
void matInvert__FP8Matrix44T0(Matrix44* out, const Matrix44* in) {
    float work[16];
    float* o = out->m;
    for (int i = 0; i < 16; ++i) {
        work[i] = in->m[i];
    }
    for (int i = 0; i < 16; ++i) {
        o[i] = (i % 5 == 0) ? 1.0f : 0.0f;
    }
    for (int i = 0; i < 4; ++i) {
        int pick = i;
        float pivot = work[i * 5];
        float magnitude = __builtin_fabsf(pivot);
        for (int j = i + 1; j < 4; ++j) {
            const float candidate = work[i * 4 + j];
            const float size = __builtin_fabsf(candidate);
            if (magnitude < size) {
                pivot = candidate;
                magnitude = size;
                pick = j;
            }
        }
        if (pick != i) {
            for (int row = 0; row < 4; ++row) {
                const float a = o[row * 4 + i];
                const float b = o[row * 4 + pick];
                o[row * 4 + i] = b;
                o[row * 4 + pick] = a;
                const float c = work[row * 4 + i];
                const float d = work[row * 4 + pick];
                work[row * 4 + i] = d;
                work[row * 4 + pick] = c;
            }
        }
        if (magnitude <= 0.0001f) {
            for (int k = 0; k < 16; ++k) {
                o[k] = (k % 5 == 0) ? 1.0f : 0.0f;
            }
            return;
        }
        const float inverse = 1.0f / pivot;
        for (int row = 0; row < 4; ++row) {
            work[row * 4 + i] = work[row * 4 + i] * inverse;
            o[row * 4 + i] = o[row * 4 + i] * inverse;
        }
        for (int j = 0; j < 4; ++j) {
            if (j == i) continue;
            const float f = work[i * 4 + j];
            for (int row = 0; row < 4; ++row) {
                const float t = work[row * 4 + i] * f;
                const float u = o[row * 4 + i] * f;
                work[row * 4 + j] = work[row * 4 + j] - t;
                o[row * 4 + j] = o[row * 4 + j] - u;
            }
        }
    }
}
}
