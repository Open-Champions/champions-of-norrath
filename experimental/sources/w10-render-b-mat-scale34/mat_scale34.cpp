// Observed matScale__FP8Matrix34f at 0x00159700, extent 112.
// (matrix, scale). Multiplies the first nine floats (the three rows of three) in
// place by the scale. The translation row (floats 9..11) is not read or written.
#include "../w9-render-math-b-common/math_types.hpp"

extern "C" void matScale__FP8Matrix34f(Matrix34* matrix, float scale) {
    float* m = matrix->m;
    for (int i = 0; i < 9; ++i) m[i] = m[i] * scale;
}
