// Observed matMakeYPRScale34_ReallySlow__FP8Matrix34G6Point3sssf at 0x00157940, extent
// 336. Arguments: output Matrix34, a Point3 (arrives as a pointer to a caller copy that
// is copied to the stack first), three signed 16-bit angles and a float scale. Returns
// nothing.
//
// Steps: the matrix is set to the identity rotation (floats 0, 4 and 8 are 1.0f, the
// other rotation floats 0.0f) with the translation row (floats 9 to 11) set to the point.
// Then matRot(matrix, 2, first + 0x4000), matRot(matrix, 1, second) and
// matRot(matrix, 0, third) are called in that order, where the first angle is
// (short)(first + 0x4000). Finally the first nine floats are each multiplied by the scale;
// the translation row is left alone.
// Inferred: the name says yaw, pitch and roll; the axis numbers 2, 1, 0 are what the
// calls pass. The name's "ReallySlow" is a label, not measured here.
// Unknown: which of the three angles the callers treat as which angle.
#include "../w9-render-math-b-common/math_types.hpp"

extern "C" {
void matRot__FP8Matrix34is(Matrix34*, int, short);

void matMakeYPRScale34_ReallySlow__FP8Matrix34G6Point3sssf(Matrix34* out, Point3 point, short first, short second,
                                                            short third, float scale) {
    const Point3 p = point;
    float* m = out->m;
    m[7] = 0.0f;
    m[8] = 1.0f;
    m[9] = p.x;
    m[10] = p.y;
    m[11] = p.z;
    m[0] = 1.0f;
    m[1] = 0.0f;
    m[2] = 0.0f;
    m[3] = 0.0f;
    m[4] = 1.0f;
    m[5] = 0.0f;
    m[6] = 0.0f;
    matRot__FP8Matrix34is(out, 2, static_cast<short>(first + 0x4000));
    matRot__FP8Matrix34is(out, 1, second);
    matRot__FP8Matrix34is(out, 0, third);
    for (int i = 0; i < 9; ++i) {
        m[i] = m[i] * scale;
    }
}
}
