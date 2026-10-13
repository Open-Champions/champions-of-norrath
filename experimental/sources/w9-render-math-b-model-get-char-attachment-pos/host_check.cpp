// Host contract for modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f.
// Not an EE plan input. Build with -ffp-contract=off. The callees are replaced by
// host stand-ins that record their arguments and return fixed values.
#include <cstdio>
#include <cstring>
#include "../w9-render-math-b-common/math_types.hpp"
struct AnimationState { int tag; };
static int callLog[16], nLog = 0;
static const void* seenState; static int seenBone; static const unsigned char* seenIn1; static const unsigned char* seenIn2;
static int seenAngle[2];
static Matrix34 boneMatrix;
extern "C" {
void animGetBoneMatrix__FP14AnimationStateiP8Matrix34(AnimationState* s, int bone, Matrix34* m) { callLog[nLog++] = 1; seenState = s; seenBone = bone; *m = boneMatrix; }
void matIdent__FP8Matrix34(Matrix34* m) { callLog[nLog++] = 2; for (float& f : m->m) f = 0; m->m[0] = m->m[4] = m->m[8] = 1; }
Point3* matTransform__FP6Point3P8Matrix34T0(Point3* out, const Matrix34*, const Point3* in) { callLog[nLog++] = 3; seenIn1 = (const unsigned char*)in; *out = Point3{in->x + 1.0f, in->y + 2.0f, in->z + 3.0f}; return out; }
Point3* matTransform34Vec__FP6Point3P8Matrix34T0(Point3* out, const Matrix34*, const Point3* in) { callLog[nLog++] = 4; seenIn2 = (const unsigned char*)in; *out = Point3{in->x * 2.0f, in->y * 2.0f, in->z * 2.0f}; return out; }
float icos__Fi(int a) { callLog[nLog++] = 5; seenAngle[0] = a; return 0.75f; }
float isin__Fi(int a) { callLog[nLog++] = 6; seenAngle[1] = a; return 0.25f; }
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(const unsigned char*, Point3, short, AnimationState*, int, Point3*, Point3*, float);
}
static bool same(float a, float b) { return std::memcmp(&a, &b, 4) == 0; }
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    alignas(16) unsigned char model[16 * 4 + 52 * 8 + 64] = {};
    model[34] = 3;  // 48 bytes before the records
    unsigned char* rec = model + 48 + 52 * 2;
    int boneWord = 77;
    std::memcpy(rec, &boneWord, 4);
    Point3 dir = {1, 2, 3}, pt = {4, 5, 6};
    std::memcpy(rec + 16, &dir, 12);
    std::memcpy(rec + 40, &pt, 12);
    AnimationState st{9};
    Point3 o1 = {0, 0, 0}, o2 = {0, 0, 0};
    const Point3 base = {10, 20, 30};
    nLog = 0;
    modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(model, base, -20000, &st, 2, &o1, &o2, 0.5f);
    const int wantLog[] = {1, 3, 4, 5, 6};
    bool logOk = nLog == 5;
    for (int i = 0; logOk && i < 5; ++i) logOk = callLog[i] == wantLog[i];
    expect(logOk, "bone matrix, point transform, direction transform, cosine, sine");
    expect(seenState == &st && seenBone == 77, "bone call gets the state and the record word");
    expect(seenIn1 == rec + 40 && seenIn2 == rec + 16, "record offsets 40 (point) and 16 (direction)");
    expect(seenAngle[0] == -20000 - 16384 && seenAngle[1] == -20000 - 16384, "angle - 16384 is not wrapped to 16 bits");
    const float c = 0.75f, s = 0.25f;
    const float X = 5.0f * 0.5f, Y = 7.0f * 0.5f, Z = 9.0f * 0.5f;
    expect(same(o1.x, ((-X) * c + Y * s) + 10.0f), "first output x");
    expect(same(o1.y, ((-X) * s - Y * c) + 20.0f), "first output y");
    expect(same(o1.z, Z + 30.0f), "first output z");
    expect(same(o2.x, (-2.0f) * c + 4.0f * s), "second output x");
    expect(same(o2.y, (-2.0f) * s - 4.0f * c), "second output y");
    expect(same(o2.z, 6.0f), "second output z unchanged");
    // null state uses the identity helper; null outputs skip their branches.
    nLog = 0;
    modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(model, base, 0, nullptr, 2, nullptr, &o2, 1.0f);
    const int want2[] = {2, 4, 5, 6};
    logOk = nLog == 4;
    for (int i = 0; logOk && i < 4; ++i) logOk = callLog[i] == want2[i];
    expect(logOk, "identity helper, one transform, then both trig calls");
    nLog = 0;
    modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(model, base, 0, &st, 2, nullptr, nullptr, 1.0f);
    expect(nLog == 3 && callLog[0] == 1 && callLog[1] == 5 && callLog[2] == 6, "no outputs: matrix and trig only");
    if (failures) return 1;
    std::printf("modelGetCharAttachmentPos contract passed\n");
    return 0;
}
