// Observed matMakeTrans__FP8Matrix34G6Point3 at 0x0015a248, extent 104. The
// Point3 argument arrives as a pointer to a caller copy; it is copied to the stack
// before the matrix is written. The twelve floats (byte offsets 0..44) become the
// identity rotation 1,0,0 / 0,1,0 / 0,0,1 followed by the point x, y, z at byte
// offsets 36, 40, 44. Returns nothing.
// Inferred: a pure translation matrix. The stores are in a fixed order the caller
// cannot observe.
#include "../w9-render-math-b-common/math_types.hpp"

extern "C" void matMakeTrans__FP8Matrix34G6Point3(Matrix34* matrix, Point3 translation) {
    float* m = matrix->m;
    m[0] = 1.0f;
    m[8] = 1.0f;
    m[11] = translation.z;
    m[9] = translation.x;
    m[10] = translation.y;
    m[1] = 0.0f;
    m[2] = 0.0f;
    m[3] = 0.0f;
    m[4] = 1.0f;
    m[5] = 0.0f;
    m[6] = 0.0f;
    m[7] = 0.0f;
}
