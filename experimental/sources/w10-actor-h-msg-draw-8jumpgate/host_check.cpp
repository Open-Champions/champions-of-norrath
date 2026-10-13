// Host contract for JumpGate msg_draw; not an EE input. Words are offsets from a host base.
#define W10_ACTOR_H_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10h_host_base;
void* w10h_host_fn(unsigned) { return nullptr; }
extern "C" unsigned char jumpGatePos[12] = {};
extern "C" void* haloTexture2 = reinterpret_cast<void*>(0x4321);
#include "msg_draw.cpp"

static unsigned char arena[0x2000] __attribute__((aligned(16)));
static std::string calls; static unsigned char seenPoint1[12]; static short seenYaw; static void* seenMatrix;
static void *sModel, *sMatrix, *sAnim, *sRet = reinterpret_cast<void*>(0x7777); static int sMode; static char sFlag;
static void *dModel, *dTexture, *dAnim, *dRecord, *dMatrix; static int dMode, dLast; static unsigned dFlags; static unsigned char dPoint[12];
static const void* fPoint; static float f1, f2; static int fi[4]; static void* fTex; static short fShort; static int fLast;
extern "C" {
void matMakeTransYaw__FP8Matrix34G6Point3s(void* m, const void* p, short yaw) { calls += "mat;"; seenMatrix = m; std::memcpy(seenPoint1, p, 12); seenYaw = yaw; }
void* modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(void* m, int mode, void* mx, void* a, char f) {
    calls += "shadow;"; sModel = m; sMode = mode; sMatrix = mx; sAnim = a; sFlag = f; return sRet; }
int modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(void* m, void* t, int mode, const void* p, void* mx, void* a, unsigned f, void* r, int l) {
    calls += "draw;"; dModel = m; dTexture = t; dMode = mode; std::memcpy(dPoint, p, 12); dMatrix = mx; dAnim = a; dFlags = f; dRecord = r; dLast = l; return 0; }
void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(const void* p, float a, float b, int c, int d, int e, int f, void* t, short g, int h) {
    calls += "face;"; fPoint = p; f1 = a; f2 = b; fi[0] = c; fi[1] = d; fi[2] = e; fi[3] = f; fTex = t; fShort = g; fLast = h; }
}
int main() {
    int bad = 0;
    w10h_host_base = arena;
    const unsigned char b77s[] = {0, 1, 0xff, 0x80}, b78s[] = {0, 1, 0xff, 0x80};
    const short yaws[] = {0, 5, -1, 0x4000, -0x8000};
    for (unsigned char b77 : b77s) for (unsigned char b78 : b78s) for (short yaw : yaws) {
        unsigned char* object = arena + 0x1000;
        std::memset(arena, 0x33, sizeof arena); calls.clear();
        for (int i = 0; i < 12; ++i) object[16 + i] = static_cast<unsigned char>(0x40 + i);
        std::memcpy(object + 56, &yaw, 2);
        object[77] = b77; object[78] = b78; object[208] = 0;
        unsigned model = 0x240, texture = 0x580; std::memcpy(object + 348, &model, 4); std::memcpy(object + 352, &texture, 4);
        msg_draw__8JumpGate(object);
        const bool anim = b77 != 0, sprite = b78 != 0;
        if (calls != (sprite ? "mat;shadow;draw;face;" : "mat;shadow;draw;")) { std::printf("fail: calls %s\n", calls.c_str()); bad = 1; }
        if (object[208] != 1) { std::puts("fail: byte 208 set"); bad = 1; }
        if (seenYaw != yaw || std::memcmp(seenPoint1, object + 16, 12) != 0 || std::memcmp(dPoint, object + 16, 12) != 0) { std::puts("fail: yaw or point"); bad = 1; }
        void* wantAnim = anim ? object + 216 : nullptr;
        if (sModel != arena + model || sMode != 0 || sFlag != 0 || sAnim != wantAnim || sMatrix != seenMatrix) { std::puts("fail: shadow arguments"); bad = 1; }
        if (dModel != arena + model || dTexture != arena + texture || dMode != 0 || dAnim != wantAnim || dFlags != 0xffffffffu || dRecord != sRet || dLast != 0 || dMatrix != seenMatrix) { std::puts("fail: draw arguments"); bad = 1; }
        if (sprite && (fPoint != jumpGatePos || f1 != 20.0f || f2 != 19000.0f || fi[0] != 100 || fi[1] != 100 || fi[2] != 188 || fi[3] != 64 || fTex != reinterpret_cast<void*>(0x4321) || fShort != 0 || fLast != 2)) { std::puts("fail: sprite arguments"); bad = 1; }
    }
    if (!bad) std::puts("jumpgate msg_draw ok");
    return bad;
}
