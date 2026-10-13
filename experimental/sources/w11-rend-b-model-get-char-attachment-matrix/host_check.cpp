// Host contract for modelGetCharAttachmentMatrix. Not an EE plan input. Callees are recording stubs.
#include <cstdio>
#include <cstring>
#include "../w9-render-math-b-common/math_types.hpp"
struct AnimationState { int tag; };
static int log_[16], n;
static int seenBone; static const void* seenRec; static Point3 seenPoint; static short seenAngle; static float seenScale;
static Matrix34* mulOut[2]; static Matrix34* mulA[2]; static Matrix34* mulB[2];
static Matrix34* second; static Matrix34* local;
extern "C" {
void animGetBoneMatrix__FP14AnimationStateiP8Matrix34(AnimationState*, int bone, Matrix34* m) { log_[n++] = 1; seenBone = bone; local = m; for (int i = 0; i < 12; ++i) m->m[i] = 1.0f; }
void matIdent__FP8Matrix34(Matrix34* m) { log_[n++] = 2; local = m; for (int i = 0; i < 12; ++i) m->m[i] = 1.0f; }
void matMul__FP8Matrix34N20(Matrix34* o, Matrix34* a, Matrix34* b) {
    int k = n == 1 ? 0 : 1; log_[n++] = 3; mulOut[k] = o; mulA[k] = a; mulB[k] = b;
    if (k == 0) for (int i = 0; i < 12; ++i) o->m[i] = static_cast<float>(i + 1);
}
void matMakeTransYawScale__FP8Matrix34G6Point3sf(Matrix34* m, Point3 p, short a, float s) { log_[n++] = 4; second = m; seenPoint = p; seenAngle = a; seenScale = s; }
void modelGetCharAttachmentMatrix__FP12_modelHeaderG6Point3sP14AnimationStateiP8Matrix34f(const unsigned char*, Point3, short, AnimationState*, int, Matrix34*, float);
}
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    alignas(16) unsigned char model[16 * 4 + 52 * 4] = {};
    model[34] = 4;
    unsigned char* rec = model + 64 + 52 * 2;
    int bone = 31; std::memcpy(rec, &bone, 4);
    AnimationState st{1};
    Matrix34 out = {};
    n = 0;
    modelGetCharAttachmentMatrix__FP12_modelHeaderG6Point3sP14AnimationStateiP8Matrix34f(model, Point3{1, 2, 3}, 0x7fff, &st, 2, &out, 4.0f);
    expect(n == 4 && log_[0] == 1 && log_[1] == 3 && log_[2] == 4 && log_[3] == 3, "call order");
    expect(seenBone == 31, "bone word");
    expect(mulOut[0] == &out && reinterpret_cast<unsigned char*>(mulA[0]) == rec + 4 && mulB[0] == local, "first multiply operands");
    expect(mulOut[1] == &out && mulA[1] == &out && mulB[1] == second, "second multiply operands");
    expect(seenPoint.x == 1 && seenPoint.y == 2 && seenPoint.z == 3, "point passed");
    expect(seenAngle == static_cast<short>(0x7fff + 0x4000) && seenScale == 4.0f, "angle plus quarter turn, scale");
    for (int i = 0; i < 9; ++i) expect(out.m[i] == (i + 1) * 0.25f, "rotation rows scaled by 1/scale");
    for (int i = 9; i < 12; ++i) expect(out.m[i] == static_cast<float>(i + 1), "translation row untouched");
    n = 0;
    modelGetCharAttachmentMatrix__FP12_modelHeaderG6Point3sP14AnimationStateiP8Matrix34f(model, Point3{0, 0, 0}, 0, nullptr, 2, &out, 1.0f);
    expect(log_[0] == 2 && n == 4, "identity when no state");
    if (failures) return 1;
    std::printf("modelGetCharAttachmentMatrix contract passed\n");
    return 0;
}
