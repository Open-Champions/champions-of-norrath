// Observed matMakeViewMatrix__FP8Matrix44fffffff at 0x00159390, extent 128. Takes a
// matrix and seven floats a, b, c, d, e, f, g (first float arguments in order), and
// writes all sixteen floats:
//   m[0]  = c * a      m[5]  = c * b      m[11] = 1.0
//   m[10] = e - (((e - d) * g) / (g - f))
//   m[14] = (((e - d) * g) * f) / (g - f)
//   every other float (1,2,3,4,6,7,8,9,12,13,15) = 0.0
// The two divisions use the observed operation order: (e-d)*g first, divided by
// g-f; the second reuses that product, multiplies by f, divides by g-f. A zero
// g - f gives the hardware's infinity or NaN result; there is no guard. Returns
// nothing.
// Inferred: this has the shape of a perspective projection with a depth range
// (d, e) and a plane distance pair (f, g). The role of a, b and c is not
// established; the given name is the executable's, not a verified meaning.
#include "../w9-render-math-b-common/math_types.hpp"

extern "C" void matMakeViewMatrix__FP8Matrix44fffffff(Matrix44* matrix, float a, float b, float c, float d, float e, float f, float g) {
    float* m = matrix->m;
    const float range = e - d;
    const float span = g - f;
    m[15] = 0.0f;
    m[11] = 1.0f;
    const float cb = c * b;
    const float ca = c * a;
    m[1] = 0.0f;
    const float scaled = range * g;
    m[2] = 0.0f;
    m[3] = 0.0f;
    m[5] = cb;
    m[0] = ca;
    const float near_term = scaled / span;
    m[4] = 0.0f;
    m[6] = 0.0f;
    m[7] = 0.0f;
    m[8] = 0.0f;
    m[9] = 0.0f;
    m[12] = 0.0f;
    const float far_term = (scaled * f) / span;
    m[13] = 0.0f;
    m[10] = e - near_term;
    m[14] = far_term;
}
