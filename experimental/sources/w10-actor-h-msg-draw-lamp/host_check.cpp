// Host contract for Lamp msg_draw; not an EE input.
#define W10_ACTOR_H_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10h_host_base;
void* w10h_host_fn(unsigned) { return nullptr; }
extern "C" void* haloTexture5 = reinterpret_cast<void*>(0x1234);
#include "msg_draw.cpp"

static std::string calls; static unsigned char seenPoint[12]; static float f1, f2; static int ia, ib, ic, id, ie; static short is_; static void* tex;
static void* queued;
extern "C" {
void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(const void* p, float a, float b, int c, int d, int e, int f, void* t, short g, int h) {
    calls += "face;"; std::memcpy(seenPoint, p, 12); f1 = a; f2 = b; ia = c; ib = d; ic = e; id = f; tex = t; is_ = g; ie = h; }
void objectAddToRunList__FP10GameObject(void* o) { calls += "run;"; queued = o; }
}
int main() {
    int bad = 0;
    const unsigned char flags[] = {0, 1, 0xff, 0x80, 0x7f};
    for (unsigned char flag : flags) {
        alignas(16) unsigned char object[160];
        std::memset(object, 0x44, sizeof object); calls.clear(); queued = nullptr;
        for (int i = 0; i < 12; ++i) object[16 + i] = static_cast<unsigned char>(0x10 + i);
        object[136] = flag;
        msg_draw__4Lamp(object);
        const bool run = flag == 0;
        if (calls != (run ? "face;run;" : "face;")) { std::printf("fail: calls %s for flag %u\n", calls.c_str(), flag); bad = 1; }
        if (std::memcmp(seenPoint, object + 16, 12) != 0 || f1 != 13.0f || f2 != 7963.0f) { std::puts("fail: position or floats"); bad = 1; }
        if (ia != 128 || ib != 100 || ic != 90 || id != 115 || is_ != 0 || ie != 2 || tex != reinterpret_cast<void*>(0x1234)) { std::puts("fail: integer arguments"); bad = 1; }
        if (object[137] != 0) { std::puts("fail: byte 137 cleared"); bad = 1; }
        if (object[136] != (run ? 1 : flag)) { std::puts("fail: byte 136"); bad = 1; }
        if (run && queued != object) { std::puts("fail: queued object"); bad = 1; }
        for (int i = 0; i < 160; ++i) if (i != 136 && i != 137 && !(i >= 16 && i < 28) && object[i] != 0x44) { std::puts("fail: stray write"); bad = 1; break; }
    }
    if (!bad) std::puts("lamp msg_draw ok");
    return bad;
}
