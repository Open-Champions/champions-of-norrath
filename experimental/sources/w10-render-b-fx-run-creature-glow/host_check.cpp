// Host contract for fxRunCreatureGlow. Not an EE plan input. Build with -ffp-contract=off.
#include <cmath>
#include <cstdio>
#include <cstring>
struct Point3 {
    float x, y, z;
};
extern "C" {
int frameNm = 0;
void* haloTexture2 = reinterpret_cast<void*>(0x1234);
static int closeResult = 1, points = 2, faces, lastColor[4];
static float lastSize;
static Point3 lastPoint;
int gameCloseToCamera__FP6Point3(Point3*) { return closeResult; }
int modelGetNmShadowPoints__FP12_modelHeader(void*) { return points; }
float icos__Fi(int) { return 1.0f; }
float isin__Fi(int) { return 0.0f; }
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
    void*, void*, Point3* p, short, int index, Point3* out, float*, void*, float) {
    *out = {static_cast<float>(index), p->x, p->y};
}
void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(Point3* p, float, float size, int a, int b, int c, int d, void*,
                                               short, int) {
    ++faces;
    lastPoint = *p;
    lastSize = size;
    lastColor[0] = a; lastColor[1] = b; lastColor[2] = c; lastColor[3] = d;
}
void fxRunCreatureGlow__FP10GameObjectP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefffPsT9ifi(
    unsigned char*, void*, void*, Point3*, short, void*, float, float, float, short*, short*, int, float, int);
}
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    alignas(8) unsigned char obj[160];
    Point3 pos = {10, 20, 30};
    short ca[4] = {60, 120, -60, 600}, cb[4] = {6, 0, 0, 0};
    float* o108 = reinterpret_cast<float*>(obj + 108);
    float* o124 = reinterpret_cast<float*>(obj + 124);
    auto reset = [&]() {
        std::memset(obj, 0, sizeof obj);
        int flags = 0x20;
        std::memcpy(obj + 12, &flags, 4);
        for (int i = 0; i < 4; ++i) { o108[i] = 5.0f; o124[i] = 10.0f * (i + 1); }
        faces = 0;
    };
    auto run = [&](int mode, float scale) {
        fxRunCreatureGlow__FP10GameObjectP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefffPsT9ifi(
            obj, reinterpret_cast<void*>(1), reinterpret_cast<void*>(2), &pos, 0, nullptr, 0.0f, 0.0f, 0.0f, ca, cb,
            mode, scale, 3);
    };
    reset();
    run(0, 1.0f);  // step is value / 60
    expect(o108[0] == 6.0f && o108[1] == 7.0f && o108[3] == 15.0f, "mode 0 adds X / 60");
    expect(o108[2] == -60.0f, "mode 0 clamps to the target when it is below the sum");
    expect(faces == 2 && lastPoint.x == 1.0f, "two sprites");
    reset();
    run(2, 1.0f);
    expect(o108[0] == 4.0f && o108[1] == 3.0f && o108[2] == 6.0f && o108[3] == 0.0f, "mode 2 subtracts and floors at 0");
    reset();
    run(1, 1.0f);
    expect(o108[0] == 5.0f, "other modes leave the arrays");
    expect(lastColor[0] == 10 && lastColor[1] == 20 && lastColor[2] == 30 && lastColor[3] == 40, "colour ints");
    expect(std::fabs(lastSize - 7990.0f) < 1e-3f, "size = 10000 - 2013 + value");
    reset();
    obj[12] = 0;
    run(0, 1.0f);
    expect(faces == 0 && o108[0] == 5.0f, "flag bit clear");
    closeResult = 0;
    reset();
    run(0, 1.0f);
    expect(faces == 0, "not close");
    if (failures) return 1;
    std::printf("fxRunCreatureGlow contract passed\n");
    return 0;
}
