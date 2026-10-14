// Host contract for modelProcessGlows. Not an EE plan input.
#include <cstdio>
#include <cstring>
struct Point3 { float x, y, z; };
struct Matrix34 { float m[12]; };
struct AnimationState; struct _modelHeader;
float worldCameraMatrix[12];
static int ev[64], nev; static Point3 seen[64]; static int bones[8], nb;
extern "C" void matIdent__FP8Matrix34(Matrix34*) { ev[nev++] = 1; }
extern "C" void animGetBoneMatrix__FP14AnimationStateiP8Matrix34(AnimationState*, int bone, Matrix34*) { ev[nev++] = 2; bones[nb++] = bone; }
extern "C" Point3* matTransform34Vec__FP6Point3P8Matrix34T0(Point3* o, const Matrix34*, const Point3* i) { seen[nev] = *i; ev[nev++] = 3; *o = *i; return o; }
extern "C" void matTransform__FP6Point3P8Matrix34T0(Point3* o, const Matrix34*, const Point3* i) { seen[nev] = *i; ev[nev++] = 4; *o = *i; }
extern "C" void modelProcessGlows__FP12_modelHeaderP14AnimationStateP8Matrix34(_modelHeader*, AnimationState*, Matrix34*);
int main() {
    alignas(16) static unsigned char h[256];
    memset(h, 0, sizeof h);
    Matrix34 w;
    modelProcessGlows__FP12_modelHeaderP14AnimationStateP8Matrix34((_modelHeader*)h, 0, &w);   // flag clear
    if (nev != 0) return 1;
    h[16] = 0x40; h[35] = 4;                       // records at +64
    memset(h + 64, 0, 64);
    h[64 + 0] = 0; short q[3] = {32, -16, 160}; memcpy(h + 64 + 2, q, 6);
    h[64 + 8] = 64; h[64 + 9] = (unsigned char)-64; h[64 + 10] = 128 - 128; h[64 + 15] = 7;
    h[64 + 16] = (unsigned char)-1;                // terminator
    worldCameraMatrix[2] = 1; worldCameraMatrix[5] = 0; worldCameraMatrix[8] = 0;
    modelProcessGlows__FP12_modelHeaderP14AnimationStateP8Matrix34((_modelHeader*)h, (AnimationState*)h, &w);
    int want[] = {1, 2, 3, 3, 4, 4};
    if (nev != 6) return 2;
    for (int i = 0; i < 6; i++) if (ev[i] != want[i]) return 3;
    if (bones[0] != 7) return 4;
    if (seen[2].x != 0.5f || seen[2].y != -0.5f) return 5;
    if (seen[4].x != 2.0f || seen[4].y != -1.0f || seen[4].z != 10.0f) return 6;
    // facing test: negative x with camera weight 1 -> no q transforms
    nev = 0; h[64 + 8] = (unsigned char)-64;
    modelProcessGlows__FP12_modelHeaderP14AnimationStateP8Matrix34((_modelHeader*)h, 0, &w);
    if (nev != 2 || ev[1] != 3) return 7;
    std::printf("modelProcessGlows contract passed\n");
    return 0;
}
