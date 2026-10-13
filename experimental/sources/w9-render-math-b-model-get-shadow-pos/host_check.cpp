// Host contract for modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f.
// Not an EE plan input. Build with -ffp-contract=off. The callees are replaced by
// host stand-ins that record their arguments.
#include <cstdint>
#include <initializer_list>
#include <cstdio>
#include <cstring>
#include "../w9-render-math-b-common/math_types.hpp"
struct AnimationState { int tag; };
static int callLog[16], nLog = 0;
static int seenBone, seenPostAxis, seenAngles[2];
static short seenPostAngle;
static Matrix33* seenPostMatrix;
static Matrix34 boneMatrix;
extern "C" {
void animGetBoneMatrix__FP14AnimationStateiP8Matrix34(AnimationState*, int bone, Matrix34* m) { callLog[nLog++] = 1; seenBone = bone; *m = boneMatrix; }
void matIdent__FP8Matrix34(Matrix34* m) { callLog[nLog++] = 2; for (float& f : m->m) f = 0; m->m[0] = m->m[4] = m->m[8] = 1; }
void matIdent__FP8Matrix33(Matrix33* m) { callLog[nLog++] = 3; for (float& f : m->m) f = 0; m->m[0] = m->m[4] = m->m[8] = 1; }
Point3* matTransform__FP6Point3P8Matrix34T0(Point3* out, const Matrix34* m, const Point3* in) { callLog[nLog++] = 4; *out = Point3{in->x + m->m[9], in->y + m->m[10], in->z + m->m[11]}; return out; }
void matPostRot__FP8Matrix33is(Matrix33* m, int axis, short angle) { callLog[nLog++] = 5; seenPostMatrix = m; seenPostAxis = axis; seenPostAngle = angle; }
float icos__Fi(int a) { callLog[nLog++] = 6; seenAngles[0] = a; return 0.75f; }
float isin__Fi(int a) { callLog[nLog++] = 7; seenAngles[1] = a; return 0.25f; }
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(const unsigned char*, AnimationState*, Point3, short, int, Point3*, float*, Matrix33*, float);
}
static bool same(float a, float b) { return std::memcmp(&a, &b, 4) == 0; }
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    for (int i = 0; i < 12; ++i) boneMatrix.m[i] = 0.5f * (i + 1);
    alignas(16) static unsigned char model[16 * 4 + 32 + 8 * 120 + 16 * 4 + 64];
    model[36] = 3;                       // table at model + 48
    unsigned char* table = model + 48;
    const int count = 100;
    std::memcpy(table + 4, &count, 4);
    auto recordAt = [&](int index) { return table + 32 + index * 8 + (index / 45) * 16; };
    // records 3 and 50 (different 45-chunks) get distinct halfwords
    for (int index : {3, 50, 99}) {
        short* h = reinterpret_cast<short*>(recordAt(index));
        h[0] = static_cast<short>(160 + index); h[1] = static_cast<short>(-32 - index); h[2] = 48;
        h[3] = static_cast<short>(0x0405 + 4 * index);
    }
    AnimationState st{1};
    const Point3 base = {10, 20, 30};
    const float c = 0.75f, s = 0.25f;
    for (int index : {3, 50, 99}) {
        Point3 o1 = {-1, -1, -1};
        float o2 = -1;
        Matrix33 o3;
        for (float& f : o3.m) f = -5;
        nLog = 0;
        modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(model, &st, base, -20000, index, &o1, &o2, &o3, 2.0f);
        const int wantLog[] = {1, 4, 5, 6, 7};
        bool logOk = nLog == 5;
        for (int i = 0; logOk && i < 5; ++i) logOk = callLog[i] == wantLog[i];
        expect(logOk, "bone, transform, post-rotation, cosine, sine");
        const short* h = reinterpret_cast<short*>(recordAt(index));
        expect(seenBone == (recordAt(index)[6] >> 2), "bone number is the low byte of halfword 3, shifted right by two");
        expect(seenPostAxis == 2 && seenPostAngle == static_cast<short>(-20000 - 16384) && seenPostMatrix == &o3, "post-rotation about axis 2 with the wrapped angle");
        expect(seenAngles[0] == -20000 - 16384 && seenAngles[1] == -20000 - 16384, "trig angle is not wrapped");
        bool rotCopied = true;
        for (int i = 0; i < 9; ++i) rotCopied = rotCopied && same(o3.m[i], boneMatrix.m[i]);
        expect(rotCopied, "output matrix holds the first nine bone floats");
        const Point3 local = {h[0] * 0.0625f + boneMatrix.m[9], h[1] * 0.0625f + boneMatrix.m[10], h[2] * 0.0625f + boneMatrix.m[11]};
        const float nx = -local.x;
        const float ry = nx * s - local.y * c, rx = nx * c + local.y * s;
        expect(same(o1.x, rx * 2.0f + 10.0f) && same(o1.y, ry * 2.0f + 20.0f) && same(o1.z, local.z * 2.0f + 30.0f), "output position");
        expect(same(o2, static_cast<float>(h[3]) * 2.0f), "output float is the signed halfword times the scale");
    }
    // index at or beyond the count, null state: zero position, zero float, identity matrix.
    {
        Point3 o1 = {-1, -1, -1};
        float o2 = -1;
        Matrix33 o3;
        for (float& f : o3.m) f = -5;
        nLog = 0;
        modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(model, nullptr, base, 0, count, &o1, &o2, &o3, 2.0f);
        const int wantLog[] = {3, 6, 7};
        bool logOk = nLog == 3;
        for (int i = 0; logOk && i < 3; ++i) logOk = callLog[i] == wantLog[i];
        expect(logOk, "identity matrix helper, then the trig calls");
        expect(o2 == 0.0f && o3.m[0] == 1.0f && o3.m[1] == 0.0f, "zero float and identity matrix");
        expect(same(o1.x, (-0.0f * c + 0.0f * s) * 2.0f + 10.0f) && o1.y == 20.0f && o1.z == 30.0f, "position is the offset");
    }
    // index below the count with null state: identity bone matrix helper; null outputs skipped.
    {
        Point3 o1;
        nLog = 0;
        modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(model, nullptr, base, 0, 3, &o1, nullptr, nullptr, 1.0f);
        const int wantLog[] = {2, 4, 6, 7};
        bool logOk = nLog == 4;
        for (int i = 0; logOk && i < 4; ++i) logOk = callLog[i] == wantLog[i];
        expect(logOk, "identity bone helper, transform, trig");
    }
    // negative index uses signed division and still counts as below the count.
    {
        Point3 o1;
        nLog = 0;
        modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(model, nullptr, base, 0, -1, &o1, nullptr, nullptr, 1.0f);
        expect(nLog == 4 && callLog[1] == 4, "negative index takes the record path");
    }
    if (failures) return 1;
    std::printf("modelGetShadowPos contract passed\n");
    return 0;
}
