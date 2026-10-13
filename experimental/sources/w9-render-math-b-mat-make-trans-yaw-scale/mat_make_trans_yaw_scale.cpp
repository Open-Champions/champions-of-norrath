// Observed matMakeTransYawScale__FP8Matrix34G6Point3sf at 0x001577d0, extent 364.
// Same construction as matMakeTransYaw__FP8Matrix34G6Point3s with a scale float
// as the fourth argument. The Point3 arrives as a pointer to a caller copy and is
// copied first. The angle is the low 16 bits of the third argument. With c and s
// the cosine and sine polynomial values for the angle (inlined, same shared
// polynomial as icos__Fi and isin__Fi) and k the scale, the twelve floats become
//   [ c*k  s*k  0 ]  [ -(s*k)  c*k  0 ]  [ 0  0  k ]  and the translation x, y, z
// (floats 0,1,2 / 3,4,5 / 6,7,8 / 9,10,11). Each product is formed as
// (signed polynomial value) * k. Returns nothing.
// Inferred: yaw about the vertical axis with uniform scale of the rotation block
// and the unit row; the translation is not scaled. Unknown: the angle unit beyond
// the 65536-per-turn inference made for the shared polynomial.
#include "../w9-render-math-b-common/math_types.hpp"
#include "../w8-object-list-math-common/trig_core.hpp"

extern "C" void matMakeTransYawScale__FP8Matrix34G6Point3sf(Matrix34* matrix, Point3 translation, short yaw, float scale) {
    const float c = trig_core::evaluate(yaw);
    const float cs = c * scale;
    const float s = trig_core::evaluate(static_cast<short>(yaw - 16384));
    float* m = matrix->m;
    const float ss = s * scale;
    m[0] = cs;
    m[4] = cs;
    m[8] = scale;
    m[2] = 0.0f;
    m[5] = 0.0f;
    m[6] = 0.0f;
    m[7] = 0.0f;
    m[11] = translation.z;
    m[9] = translation.x;
    m[10] = translation.y;
    m[1] = ss;
    m[3] = -ss;
}
