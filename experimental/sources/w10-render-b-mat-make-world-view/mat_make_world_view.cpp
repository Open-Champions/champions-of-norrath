// Observed matMakeWorldViewMatrix__FP8Matrix44fffffff at 0x001594b0, extent 160.
// (matrix, p1..p7), seven floats passed in the first seven float argument registers.
// The function body is the same instruction words as the sibling matMakeCharViewMatrix.
//   span = p5 - p4
//   den = p7 - p6
//   m[0]  = p3 * p1,  m[5] = p3 * p2,  m[11] = 1.0,  m[15] = 0
//   m[10] = (p5 - (span * p7) / den) * 16 * 2^46
//   m[14] = ((span * p7 * p6) / den) * 16 * 2^46
// and the other ten floats (1..4, 6..9, 12, 13) are set to zero. 2^46 is the exact
// constant 0x56800000. The meaning of the parameters is not established.
#include "../w9-render-math-b-common/math_types.hpp"

extern "C" void matMakeWorldViewMatrix__FP8Matrix44fffffff(Matrix44* matrix, float p1, float p2, float p3, float p4,
                                                  float p5, float p6, float p7) {
    float* m = matrix->m;
    const float span = p5 - p4;
    const float den = p7 - p6;
    m[11] = 1.0f;
    const float y = p3 * p2;
    const float x = p3 * p1;
    const float scaled = span * p7;
    m[15] = 0.0f;
    m[1] = 0.0f;
    m[5] = y;
    m[0] = x;
    const float q = scaled / den;
    m[2] = 0.0f;
    const float top = scaled * p6;
    m[3] = 0.0f;
    m[4] = 0.0f;
    m[6] = 0.0f;
    m[7] = 0.0f;
    m[8] = 0.0f;
    m[9] = 0.0f;
    const float r = top / den;
    m[12] = 0.0f;
    m[13] = 0.0f;
    m[10] = ((p5 - q) * 16.0f) * 0x1p46f;
    m[14] = (r * 16.0f) * 0x1p46f;
}
