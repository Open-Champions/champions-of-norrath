// Observed modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f
// at 0x00168a38, extent 464. Arguments, in order: model header, a Point3 by value
// (arrives as a pointer to a caller copy; copied to the stack first), a 16-bit
// angle (low halfword of the third argument, sign-extended), an animation state
// (may be null), an index, two output Point3 pointers (either may be null) and a
// float scale. Returns nothing.
//
// Header use: the byte at header+34 times 16 is added to the header address to
// reach a table of 52-byte records; the record for the index is at 52 * index.
// In that record the word at +0 is passed to animGetBoneMatrix, the three floats
// at +16 are a direction and the three floats at +40 are a point.
//
// Steps, in this order:
//  1. A Matrix34 on the stack is filled by animGetBoneMatrix(state, record word,
//     &matrix) when the state is non-null, else by matIdent.
//  2. If the first output is non-null: matTransform(out1, &matrix, record+40).
//  3. If the second output is non-null: matTransform34Vec(out2, &matrix, record+16).
//  4. c = icos(angle - 16384) and s = isin(angle - 16384). The subtraction is on
//     the 32-bit sign-extended angle and is not wrapped back to 16 bits.
//  5. First output, if non-null, with X = out1.x*scale, Y = out1.y*scale:
//       x = ((-X)*c + Y*s) + point.x
//       y = ((-X)*s - Y*c) + point.y
//       z = out1.z*scale + point.z
//  6. Second output, if non-null (no scale, no offset, z unchanged):
//       x = (-out2.x)*c + out2.y*s
//       y = (-out2.x)*s - out2.y*c
// Inferred: an attachment (bone-relative point and direction) turned about the
// vertical axis by the angle and moved to the caller's point; the record's role
// as an attachment table is a guess from the function name. Unknown: the record's
// other fields and the meaning of the record word at +0.
#include "../w9-render-math-b-common/math_types.hpp"

struct AnimationState;
extern "C" {
void animGetBoneMatrix__FP14AnimationStateiP8Matrix34(AnimationState*, int, Matrix34*);
void matIdent__FP8Matrix34(Matrix34*);
Point3* matTransform__FP6Point3P8Matrix34T0(Point3*, const Matrix34*, const Point3*);
Point3* matTransform34Vec__FP6Point3P8Matrix34T0(Point3*, const Matrix34*, const Point3*);
float icos__Fi(int);
float isin__Fi(int);

void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
    const unsigned char* header, Point3 point, short angle, AnimationState* state, int index, Point3* out1, Point3* out2,
    float scale) {
    const unsigned char* records = header + header[34] * 16;
    const unsigned char* record = records + index * 52;
    Matrix34 matrix;
    if (state != nullptr) {
        animGetBoneMatrix__FP14AnimationStateiP8Matrix34(state, *reinterpret_cast<const int*>(record), &matrix);
    } else {
        matIdent__FP8Matrix34(&matrix);
    }
    if (out1 != nullptr) {
        matTransform__FP6Point3P8Matrix34T0(out1, &matrix, reinterpret_cast<const Point3*>(record + 40));
    }
    if (out2 != nullptr) {
        matTransform34Vec__FP6Point3P8Matrix34T0(out2, &matrix, reinterpret_cast<const Point3*>(record + 16));
    }
    const int turned = static_cast<int>(angle) - 16384;
    const float c = icos__Fi(turned);
    const float s = isin__Fi(turned);
    if (out1 != nullptr) {
        const float x = out1->x * scale;
        const float y = out1->y * scale;
        const float z = out1->z * scale;
        const float nx = -x;
        const float wz = z + point.z;
        out1->z = wz;
        const float ry = (nx * s - y * c) + point.y;
        const float rx = (nx * c + y * s) + point.x;
        out1->y = ry;
        out1->x = rx;
    }
    if (out2 != nullptr) {
        const float nx = -out2->x;
        const float y = out2->y;
        const float ry = nx * s - y * c;
        const float rx = nx * c + y * s;
        out2->y = ry;
        out2->x = rx;
    }
}
}
