// Host contract for modelGetAttachmentMatrix. Not an EE plan input.
#include <cstdio>
#include <cstring>
struct Matrix34 { float m[12]; };
struct AnimationState;
struct _modelHeader;
static int log_[16], nlog;
static int boneSeen;
extern "C" void animGetBoneMatrix__FP14AnimationStateiP8Matrix34(AnimationState*, int bone, Matrix34* o) { log_[nlog++] = 1; boneSeen = bone; for (int i = 0; i < 12; i++) o->m[i] = 100 + i; }
extern "C" void matIdent__FP8Matrix34(Matrix34* m) { log_[nlog++] = 2; for (int i = 0; i < 12; i++) m->m[i] = i == 0 || i == 4 || i == 8; }
extern "C" void matScale__FP8Matrix34f(Matrix34* m, float s) { log_[nlog++] = 3; m->m[0] = s; }
extern "C" void matMul__FP8Matrix34N20(Matrix34* d, const Matrix34* a, const Matrix34* b) { log_[nlog++] = 4; d->m[0] = a->m[0] + b->m[0]; d->m[1] = a->m[1] * 1000 + b->m[1]; }
extern "C" void modelGetAttachmentMatrix__FP12_modelHeaderP14AnimationStateiP8Matrix34f(_modelHeader*, AnimationState*, int, Matrix34*, float);
int main() {
    alignas(16) static unsigned char buf[512];
    memset(buf, 0, sizeof buf);
    buf[34] = 2;                       // table at +32
    unsigned char* e = buf + 32 + 52 * 3;
    int word = 77; memcpy(e, &word, 4);
    float f0 = 5, f1 = 2; memcpy(e + 4, &f0, 4); memcpy(e + 8, &f1, 4);
    Matrix34 out; memset(&out, 0, sizeof out);
    modelGetAttachmentMatrix__FP12_modelHeaderP14AnimationStateiP8Matrix34f((_modelHeader*)buf, (AnimationState*)buf, 3, &out, 4.0f);
    int want[] = {1, 4, 2, 3, 4};
    if (nlog != 5) return 1;
    for (int i = 0; i < 5; i++) if (log_[i] != want[i]) return 2;
    if (boneSeen != 77) return 3;
    // first matMul: out0 = 5 + 100 = 105, out1 = 2000 + 101; second: out0 = 105 + 4, out1 = 2101000 + 0
    if (out.m[0] != 109.0f || out.m[1] != 2101000.0f) return 4;
    nlog = 0;
    modelGetAttachmentMatrix__FP12_modelHeaderP14AnimationStateiP8Matrix34f((_modelHeader*)buf, 0, 3, &out, 1.0f);
    if (nlog != 5 || log_[0] != 2 || log_[1] != 4) return 5;
    std::printf("modelGetAttachmentMatrix contract passed\n");
    return 0;
}
