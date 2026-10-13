// Observed modelGetAttachmentPos__FP12_modelHeaderP14AnimationStateiP6Point3T3 at
// 0x0016cb00, extent 212. Arguments: model header, animation state (may be null), a
// record index, and two output Point3 pointers (either may be null). Returns nothing.
//
// Header use (same layout as modelGetCharAttachmentPos): the byte at header+34 times 16
// is added to the header address to reach a table of 52-byte records; the record for
// the index is at 52 * index. In it the word at +0 is passed to animGetBoneMatrix, the
// three floats at +16 are a direction and the three floats at +40 are a point.
//
// Steps: a Matrix34 on the stack is filled by animGetBoneMatrix(state, record word,
// &matrix) when the state is non-null, else by matIdent. Then, if the first output is
// non-null, matTransform(out1, &matrix, record+40); then, if the second output is
// non-null, matTransform34Vec(out2, &matrix, record+16). There is no angle or scale.
// Inferred: this returns the attachment point and direction in model space; the
// record's role as an attachment table is a guess from the name. Unknown: the other
// record fields.
#include "../w9-render-math-b-common/math_types.hpp"

struct AnimationState;
extern "C" {
void animGetBoneMatrix__FP14AnimationStateiP8Matrix34(AnimationState*, int, Matrix34*);
void matIdent__FP8Matrix34(Matrix34*);
Point3* matTransform__FP6Point3P8Matrix34T0(Point3*, const Matrix34*, const Point3*);
Point3* matTransform34Vec__FP6Point3P8Matrix34T0(Point3*, const Matrix34*, const Point3*);

void modelGetAttachmentPos__FP12_modelHeaderP14AnimationStateiP6Point3T3(const unsigned char* header,
                                                                          AnimationState* state, int index,
                                                                          Point3* out1, Point3* out2) {
    const unsigned char* record = header + header[34] * 16 + index * 52;
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
}
}
