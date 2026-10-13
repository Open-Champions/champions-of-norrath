// Observed matInvert__FP8Matrix34T0 at 0x00157d78, extent 168.
// (output, input), both Matrix34. The twelve input floats are copied into the four
// rows of a sixteen-float stack matrix (row stride 16 bytes on the stack, 12 bytes
// in the source); the fourth column is set to 0, 0, 0, 1.0 and the 4x4 inverse
// matInvert__FP8Matrix44T0 is called in place on that temporary. Its first twelve
// floats' rows are then copied back to the output the same way. Output and input
// may alias because the input is fully copied before the call.
// matInvert__FP8Matrix44T0 (0x00157a90) is not part of this unit.
#include "../w9-render-math-b-common/math_types.hpp"

extern "C" void matInvert__FP8Matrix44T0(Matrix44* out, const Matrix44* in);

extern "C" void matInvert__FP8Matrix34T0(Matrix34* out, const Matrix34* in) {
    Matrix44 tmp;
    const float* src = in->m;
    float* dst = tmp.m;
    for (int row = 3; row >= 0; --row) {
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
        src += 3;
        dst += 4;
    }
    tmp.m[3] = 0.0f;
    tmp.m[7] = 0.0f;
    tmp.m[11] = 0.0f;
    tmp.m[15] = 1.0f;
    matInvert__FP8Matrix44T0(&tmp, &tmp);
    src = tmp.m;
    dst = out->m;
    for (int row = 3; row >= 0; --row) {
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
        src += 4;
        dst += 3;
    }
}
