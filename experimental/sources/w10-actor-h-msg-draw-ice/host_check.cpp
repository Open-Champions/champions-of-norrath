// Host contract for Ice msg_draw; not an EE input. Words are offsets from a host base.
#define W10_ACTOR_H_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10h_host_base;
void* w10h_host_fn(unsigned) { return nullptr; }
#include "msg_draw.cpp"

static unsigned char arena[0x2000] __attribute__((aligned(16)));
static std::string calls; static unsigned char seenPoint1[12], seenPoint2[12]; static short seenYaw; static void* seenMatrix;
static void *dModel, *dTexture, *dAnim, *dRecord; static int dMode, dLast; static unsigned dFlags; static unsigned char dPoint[12]; static void* dMatrix;
extern "C" {
void matMakeTransYaw__FP8Matrix34G6Point3s(void* m, const void* p, short yaw) { calls += "mat;"; seenMatrix = m; std::memcpy(seenPoint1, p, 12); seenYaw = yaw; }
int modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(void* m, void* t, int mode, const void* p, void* mx, void* a, unsigned f, void* r, int l) {
    calls += "draw;"; dModel = m; dTexture = t; dMode = mode; std::memcpy(dPoint, p, 12); dMatrix = mx; dAnim = a; dFlags = f; dRecord = r; dLast = l; return 0; }
}
int main() {
    int bad = 0;
    w10h_host_base = arena;
    const short halves[] = {0, 1, 0x3fff, 0x4000, -1, 0x7fff, -0x8000};
    for (short h : halves) {
        unsigned char* object = arena + 0x1000;
        std::memset(arena, 0x33, sizeof arena); calls.clear();
        for (int i = 0; i < 12; ++i) object[16 + i] = static_cast<unsigned char>(0x40 + i);
        std::memcpy(object + 56, &h, 2);
        unsigned model = 0x240, texture = 0x580; std::memcpy(object + 96, &model, 4); std::memcpy(object + 100, &texture, 4);
        msg_draw__3Ice(object);
        if (calls != "mat;draw;") { std::puts("fail: call order"); bad = 1; }
        if (std::memcmp(seenPoint1, object + 16, 12) != 0 || std::memcmp(dPoint, object + 16, 12) != 0) { std::puts("fail: point copies"); bad = 1; }
        if (seenYaw != static_cast<short>(static_cast<unsigned short>(h) + 16384)) { std::printf("fail: yaw %d for %d\n", seenYaw, h); bad = 1; }
        if (dModel != arena + model || dTexture != arena + texture) { std::puts("fail: model/texture words"); bad = 1; }
        if (dMode != 1 || dAnim != nullptr || dFlags != 0xffffffffu || dRecord != nullptr || dLast != 0) { std::puts("fail: constant arguments"); bad = 1; }
        if (dMatrix != seenMatrix) { std::puts("fail: same matrix"); bad = 1; }
    }
    if (!bad) std::puts("ice msg_draw ok");
    return bad;
}
