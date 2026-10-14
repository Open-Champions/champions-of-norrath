// Observed matRot__FP8Matrix33is at 0x156610, extent 520. Rotates two rows of
// the matrix in place about an axis. The angle is the low 16 bits of the third
// argument, sign-extended; the cosine (icos__Fi) is computed first and the sine
// (isin__Fi) second, both always, even when the axis selects nothing. With
// c = cos and s = sin and the nine floats viewed as rows of three:
//   axis 0: row1' = c*row1 + s*row2,  row2' = -s*row1 + c*row2
//   axis 1: row0' = c*row0 - s*row2,  row2' =  s*row0 + c*row2
//   axis 2: row0' = c*row0 + s*row1,  row1' = -s*row0 + c*row1
// All new values are computed from the old ones before any store is observable
// to the caller. Any other axis value leaves the matrix unchanged. Returns
// nothing. The element order and the plus/minus pattern are observed; calling
// the first operand of each pair a row is an inference.
#include "../w8-object-list-math-common/matrix_types.hpp"

extern "C" float icos__Fi(int angle);
extern "C" float isin__Fi(int angle);

namespace {
// First = c*a + s*b, second = -s*a + c*b over matching elements of two rows.
void rotate_rows(float* a, float* b, float c, float s) {
    for (int i = 0; i < 3; ++i) {
        const float x = a[i], y = b[i];
        a[i] = c * x + s * y;
        b[i] = -s * x + c * y;
    }
}
}  // namespace

extern "C" void matRot__FP8Matrix33is(Matrix33* matrix, int axis, short angle) {
    const float c = icos__Fi(angle);
    const float s = isin__Fi(angle);
    float* m = matrix->m;
    switch (axis) {
    case 0:
        rotate_rows(m + 3, m + 6, c, s);
        break;
    case 1:
        // rotate_rows(row2, row0): row2' = c*row2 + s*row0 and row0' = -s*row2 + c*row0.
        rotate_rows(m + 6, m + 0, c, s);
        break;
    case 2:
        rotate_rows(m + 0, m + 3, c, s);
        break;
    default:
        break;
    }
}
