// Observed modelGetCharAttachmentMatrix__FP12_modelHeaderG6Point3sP14AnimationStateiP8Matrix34f
// at 0x001688c8, extent 368. Arguments: model header, a Point3 (arrives as a pointer to
// a caller copy), a signed 16-bit angle, an animation state (may be null), an index, an
// output Matrix34 and a float scale. Returns nothing.
//
// Header use (same layout as modelGetAttachmentPos): the byte at header+34 times 16 is
// added to the header address; the 52-byte record for the index is at 52 * index in that
// table. The word at record+0 is the bone number, and the twelve floats at record+4 are
// a Matrix34.
//
// Steps: a local Matrix34 is filled by animGetBoneMatrix(state, bone, &local) when the
// state is non-null, else by matIdent. matMul(out, record+4, &local) follows. The first
// nine floats of the result (three rows of three; the translation row is left alone) are
// each multiplied by 1.0f / scale. A second Matrix34 is made by
// matMakeTransYawScale(&second, &point, (short)(angle + 0x4000), scale) and the result
// is matMul(out, out, &second).
// Inferred: the name says this builds the matrix at which an attachment hangs off a
// character. The 0x4000 offset is an angle quarter turn; its meaning is unknown.
// Unknown: what the record fields beyond the bone number and matrix are.
#include "../w9-render-math-b-common/math_types.hpp"

struct AnimationState;
extern "C" {
void animGetBoneMatrix__FP14AnimationStateiP8Matrix34(AnimationState*, int, Matrix34*);
void matIdent__FP8Matrix34(Matrix34*);
void matMul__FP8Matrix34N20(Matrix34*, Matrix34*, Matrix34*);
void matMakeTransYawScale__FP8Matrix34G6Point3sf(Matrix34*, Point3, short, float);

void modelGetCharAttachmentMatrix__FP12_modelHeaderG6Point3sP14AnimationStateiP8Matrix34f(
    const unsigned char* header, Point3 point, short angle, AnimationState* state, int index, Matrix34* out,
    float scale) {
    const unsigned char* record = header + header[34] * 16 + index * 52;
    Matrix34 local;
    if (state != nullptr) {
        animGetBoneMatrix__FP14AnimationStateiP8Matrix34(state, *reinterpret_cast<const int*>(record), &local);
    } else {
        matIdent__FP8Matrix34(&local);
    }
    matMul__FP8Matrix34N20(out, reinterpret_cast<Matrix34*>(const_cast<unsigned char*>(record + 4)), &local);
    const float inverse = 1.0f / scale;
    for (int i = 0; i < 9; ++i) {
        out->m[i] = out->m[i] * inverse;
    }
    Matrix34 second;
    matMakeTransYawScale__FP8Matrix34G6Point3sf(&second, point, static_cast<short>(angle + 0x4000), scale);
    matMul__FP8Matrix34N20(out, out, &second);
}
}
