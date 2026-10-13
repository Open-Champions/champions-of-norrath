// Observed matPostRot__FP8Matrix33is at 0x00156aa0, extent 524.
// (matrix, axis, angle). The short angle is sign-extended and passed to icos__Fi
// then isin__Fi (cosine is called first); c and s are the float results. For
// axis 0, 1 or 2 the two matrix columns other than the axis column are rotated in
// every row r (base index 3r), as a, b = the two entries:
//   axis 0 (columns 1, 2):  new a = c*a - s*b,  new b = s*a + c*b
//   axis 1 (columns 0, 2):  new a = c*a + s*b,  new b = (-s)*a + c*b
//   axis 2 (columns 0, 1):  new a = c*a - s*b,  new b = s*a + c*b
// Any other axis value leaves the matrix unchanged (the trig routines still run).
// The routine multiplies on the right by the rotation, so the source rows are
// updated in place and each row uses its own old a and b.
#include "../w9-render-math-b-common/math_types.hpp"

extern "C" float icos__Fi(int angle);
extern "C" float isin__Fi(int angle);

extern "C" void matPostRot__FP8Matrix33is(Matrix33* matrix, int axis, short angle) {
    float* m = matrix->m;
    const float c = icos__Fi(angle);
    const float s = isin__Fi(angle);
    if (axis == 1) {
        const float ns = -s;
        for (int r = 0; r < 9; r += 3) {
            const float a = m[r + 0];
            const float b = m[r + 2];
            m[r + 0] = c * a + s * b;
            m[r + 2] = ns * a + c * b;
        }
    } else if (axis == 0) {
        for (int r = 0; r < 9; r += 3) {
            const float a = m[r + 1];
            const float b = m[r + 2];
            m[r + 1] = c * a - s * b;
            m[r + 2] = s * a + c * b;
        }
    } else if (axis == 2) {
        for (int r = 0; r < 9; r += 3) {
            const float a = m[r + 0];
            const float b = m[r + 1];
            m[r + 0] = c * a - s * b;
            m[r + 1] = s * a + c * b;
        }
    }
}
