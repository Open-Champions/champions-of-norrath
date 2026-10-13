// Observed matTrans__FP8Matrix44fff at 0x00159648, extent 176.
// (matrix, x, y, z). Treats the displacement as a point (x, y, z, 1) in the row
// convention used by matTransform: for each column j the translation-row entry
// becomes
//   m[12+j] + (((m[j]*x + m[4+j]*y) + m[8+j]*z))
// All four columns are updated (12, 13, 14 and 15). Rows 0..2 are only read.
// The original computes the row 3 sum first and adds the old entry last.
#include "../w9-render-math-b-common/math_types.hpp"

extern "C" void matTrans__FP8Matrix44fff(Matrix44* matrix, float x, float y, float z) {
    float* m = matrix->m;
    for (int j = 0; j < 4; ++j) m[12 + j] = m[12 + j] + ((m[j] * x + m[4 + j] * y) + m[8 + j] * z);
}
