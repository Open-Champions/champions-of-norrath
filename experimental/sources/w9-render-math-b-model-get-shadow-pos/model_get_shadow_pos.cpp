// Observed modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f
// at 0x0016c238, extent 780. Arguments, in order: model header, animation state
// (may be null), a Point3 by value (arrives as a pointer to a caller copy and is
// copied to the stack first), a 16-bit angle (sign-extended low halfword), an
// index, an output Point3 (always written, never tested for null), an optional
// output float, an optional output Matrix33 and a float scale. Returns nothing.
//
// Header use: with t = header + 16 * (unsigned byte at header+36), the word at
// t+4 is a count and is compared signed with the index. Records are 8 bytes:
// record = t + 32 + 8 * index + 16 * (index / 45), the division signed and
// truncating. Record fields: three signed halfwords at +0, +2, +4 (a position in
// sixteenths: each converted to float and multiplied by 0.0625), and a halfword
// at +6 whose low byte shifted right by two is the bone number and whose signed
// value converted to float is a size-like value.
//
// index < count:
//   bone matrix (Matrix34): animGetBoneMatrix(state, bone, &m) when state is
//   non-null, else matIdent34. pos = the three position floats, then transformed
//   in place by matTransform (Matrix34 version). If out2: *out2 = (float)(signed
//   halfword at +6) * scale. If out3: its nine floats are the first nine floats
//   of the bone matrix, then matPostRot(out3, 2, (short)(angle - 16384)).
// otherwise (index >= count): pos = (0, 0, 0); if out2: *out2 = 0.0f; if out3:
//   matIdent33(out3).
// Then, for both: c = icos(angle - 16384), s = isin(angle - 16384) (32-bit
// subtraction, not wrapped), and
//   (pos.x, pos.y) = ((-pos.x)*c + pos.y*s, (-pos.x)*s - pos.y*c)
//   pos = pos * scale componentwise, then pos += point, and *out1 = pos.
// Inferred: a shadow position for an index into a model's shadow-point table,
// the count function being modelGetNmShadowPoints. Unknown: the meaning of the
// halfword at +6 beyond the two reads described, and the 45-record chunking.
#include "../w9-render-math-b-common/math_types.hpp"

struct AnimationState;
extern "C" {
void animGetBoneMatrix__FP14AnimationStateiP8Matrix34(AnimationState*, int, Matrix34*);
void matIdent__FP8Matrix34(Matrix34*);
void matIdent__FP8Matrix33(Matrix33*);
Point3* matTransform__FP6Point3P8Matrix34T0(Point3*, const Matrix34*, const Point3*);
void matPostRot__FP8Matrix33is(Matrix33*, int, short);
float icos__Fi(int);
float isin__Fi(int);

void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
    const unsigned char* header, AnimationState* state, Point3 point, short angle, int index, Point3* out1, float* out2,
    Matrix33* out3, float scale) {
    const unsigned char* table = header + header[36] * 16;
    const int count = *reinterpret_cast<const int*>(table + 4);
    Point3 pos;
    if (index < count) {
        const unsigned char* record = table + 32 + index * 8 + (index / 45) * 16;
        Matrix34 bone;
        if (state != nullptr) {
            animGetBoneMatrix__FP14AnimationStateiP8Matrix34(state, record[6] >> 2, &bone);
        } else {
            matIdent__FP8Matrix34(&bone);
        }
        const short* halves = reinterpret_cast<const short*>(record);
        const float px = static_cast<float>(halves[0]) * 0.0625f;
        const float py = static_cast<float>(halves[1]) * 0.0625f;
        const float pz = static_cast<float>(halves[2]) * 0.0625f;
        pos.x = px;
        pos.y = py;
        pos.z = pz;
        matTransform__FP6Point3P8Matrix34T0(&pos, &bone, &pos);
        if (out2 != nullptr) {
            *out2 = static_cast<float>(halves[3]) * scale;
        }
        if (out3 != nullptr) {
            for (int i = 0; i < 9; ++i) out3->m[i] = bone.m[i];
            matPostRot__FP8Matrix33is(out3, 2, static_cast<short>(angle - 16384));
        }
    } else {
        pos.x = 0.0f;
        pos.y = 0.0f;
        pos.z = 0.0f;
        if (out2 != nullptr) {
            *out2 = 0.0f;
        }
        if (out3 != nullptr) {
            matIdent__FP8Matrix33(out3);
        }
    }
    const int turned = static_cast<int>(angle) - 16384;
    const float c = icos__Fi(turned);
    const float s = isin__Fi(turned);
    const float nx = -pos.x;
    const float y = pos.y;
    const float ry = nx * s - y * c;
    const float rx = nx * c + y * s;
    pos.y = ry;
    pos.x = rx;
    pos.x = pos.x * scale;
    pos.y = pos.y * scale;
    pos.z = pos.z * scale;
    pos.x = pos.x + point.x;
    pos.y = pos.y + point.y;
    pos.z = pos.z + point.z;
    *out1 = pos;
}
}
