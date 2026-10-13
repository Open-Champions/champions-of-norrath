// Host contract for modelGetAttachmentPos. Not an EE plan input. The callees are recording stubs.
#include <cstdio>
#include <cstring>
#include "../w9-render-math-b-common/math_types.hpp"
struct AnimationState { int tag; };
static int callLog[16], nLog = 0;
static const void* seenState; static int seenBone; static const unsigned char *seenIn1, *seenIn2;
extern "C" {
void animGetBoneMatrix__FP14AnimationStateiP8Matrix34(AnimationState* s, int bone, Matrix34*) { callLog[nLog++] = 1; seenState = s; seenBone = bone; }
void matIdent__FP8Matrix34(Matrix34*) { callLog[nLog++] = 2; }
Point3* matTransform__FP6Point3P8Matrix34T0(Point3* o, const Matrix34*, const Point3* in) { callLog[nLog++] = 3; seenIn1 = (const unsigned char*)in; return o; }
Point3* matTransform34Vec__FP6Point3P8Matrix34T0(Point3* o, const Matrix34*, const Point3* in) { callLog[nLog++] = 4; seenIn2 = (const unsigned char*)in; return o; }
void modelGetAttachmentPos__FP12_modelHeaderP14AnimationStateiP6Point3T3(const unsigned char*, AnimationState*, int, Point3*, Point3*);
}
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    alignas(16) unsigned char model[16 * 4 + 52 * 8 + 64] = {};
    model[34] = 3;
    unsigned char* rec = model + 48 + 52 * 2;
    int boneWord = 77;
    std::memcpy(rec, &boneWord, 4);
    AnimationState st{9};
    Point3 o1, o2;
    nLog = 0;
    modelGetAttachmentPos__FP12_modelHeaderP14AnimationStateiP6Point3T3(model, &st, 2, &o1, &o2);
    expect(nLog == 3 && callLog[0] == 1 && callLog[1] == 3 && callLog[2] == 4, "bone matrix, point, direction");
    expect(seenState == &st && seenBone == 77, "bone call arguments");
    expect(seenIn1 == rec + 40 && seenIn2 == rec + 16, "record offsets 40 and 16");
    nLog = 0;
    modelGetAttachmentPos__FP12_modelHeaderP14AnimationStateiP6Point3T3(model, nullptr, 2, nullptr, &o2);
    expect(nLog == 2 && callLog[0] == 2 && callLog[1] == 4, "identity, direction only");
    nLog = 0;
    modelGetAttachmentPos__FP12_modelHeaderP14AnimationStateiP6Point3T3(model, &st, 2, &o1, nullptr);
    expect(nLog == 2 && callLog[0] == 1 && callLog[1] == 3, "matrix, point only");
    if (failures) return 1;
    std::printf("modelGetAttachmentPos contract passed\n");
    return 0;
}
