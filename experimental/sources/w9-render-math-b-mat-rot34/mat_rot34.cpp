// Observed matRot__FP8Matrix34is at 0x00156cb0, extent 520. Rotates two rows of
// the matrix's first nine floats in place about an axis (the translation floats
// 9..11 are never touched). The angle is the low 16 bits of the third argument,
// sign-extended; the cosine (icos__Fi) is computed first and the sine (isin__Fi)
// second, both always, even when the axis selects nothing. With c = cos, s = sin
// and the nine floats viewed as rows of three:
//   axis 0: row1' =  c*row1 + s*row2,  row2' = -s*row1 + c*row2
//   axis 1: row0' =  c*row0 - s*row2,  row2' =  s*row0 + c*row2
//   axis 2: row0' =  c*row0 + s*row1,  row1' = -s*row0 + c*row1
// All new values are computed from the old ones. Any other axis value (negative
// or above 2) leaves the matrix unchanged. Returns nothing. This is the Matrix34
// twin of matRot__FP8Matrix33is with the same formulas on the same nine floats.
// Calling the groups of three "rows" is an inference from the offsets.
#include "../w9-render-math-b-common/math_types.hpp"

extern "C" float icos__Fi(int angle);
extern "C" float isin__Fi(int angle);

extern "C" void matRot__FP8Matrix34is(Matrix34* matrix, int axis, short angle) {
    const float c = icos__Fi(angle);
    const float s = isin__Fi(angle);
    float* m = matrix->m;
    if (axis == 0) {
        for (int i = 0; i < 3; ++i) {
            const float x = m[3 + i], y = m[6 + i];
            m[3 + i] = c * x + s * y;
            m[6 + i] = -s * x + c * y;
        }
    } else if (axis == 1) {
        for (int i = 0; i < 3; ++i) {
            const float x = m[i], y = m[6 + i];
            m[i] = c * x - s * y;
            m[6 + i] = s * x + c * y;
        }
    } else if (axis == 2) {
        for (int i = 0; i < 3; ++i) {
            const float x = m[i], y = m[3 + i];
            m[i] = c * x + s * y;
            m[3 + i] = -s * x + c * y;
        }
    }
}
