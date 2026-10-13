// Observed matScale__FP8Matrix44f at 0x001595d8, extent 112.
// (matrix, scale). Multiplies the upper-left three-by-three of the sixteen floats
// (indices 0..2, 4..6 and 8..10) in place by the scale. Column three (3, 7, 11)
// and the translation row (12..15) are not read or written.
#include "../w9-render-math-b-common/math_types.hpp"

extern "C" void matScale__FP8Matrix44f(Matrix44* matrix, float scale) {
    float* m = matrix->m;
    for (int row = 0; row < 12; row += 4)
        for (int i = 0; i < 3; ++i) m[row + i] = m[row + i] * scale;
}
