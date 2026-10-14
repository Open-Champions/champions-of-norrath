// Observed matTrans__FP8Matrix34fff at 0x00159550, extent 136. Takes a matrix and
// three floats (x, y, z). Treats floats 0..2, 3..5 and 6..8 of the matrix as three
// rows and moves the translation row (floats 9..11) by the combination
//   t += x * row0 + y * row1 + z * row2
// evaluated per component as ((row0*x + row1*y) + row2*z) and then added to the old
// translation. The first nine floats are not written. Returns nothing.
// Inferred: translation applied in the matrix's own (local) axes, as opposed to
// matMakeTrans which sets the translation outright.
#include "../w9-render-math-b-common/math_types.hpp"

extern "C" void matTrans__FP8Matrix34fff(Matrix34* matrix, float x, float y, float z) {
    float* m = matrix->m;
    const float tx = (m[0] * x + m[3] * y) + m[6] * z;
    const float ty = (m[1] * x + m[4] * y) + m[7] * z;
    const float tz = (m[2] * x + m[5] * y) + m[8] * z;
    m[9] = m[9] + tx;
    m[10] = m[10] + ty;
    m[11] = m[11] + tz;
}
