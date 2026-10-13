// Host contract for msg_draw__10CloudGiant; not an EE input. 32-bit words holding pointers are
// offsets from a host base (W10_ACTOR_D_HOST), so the real field offsets are used.
#define W10_ACTOR_D_HOST 1
#include <cstdio>
#include <cstring>
#include <initializer_list>
unsigned char* w10_actor_d_host_base;
extern "C" { signed char cutMode; }
#include "msg_draw.cpp"
static int drawCalls, attachCalls;
static void *dSelf, *dModel, *dTexture, *dSecond, *aModel, *aTexture, *aModel2, *aRecord;
static int aA, aB, aC; static unsigned long aMask;
static unsigned char recordMarker[4];
void* draw__8CreatureP12_modelHeaderP8_textureT1(void* s, void* m, void* t, void* x) { ++drawCalls; dSelf = s; dModel = m; dTexture = t; dSecond = x; return recordMarker; }
void modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(void* m, void* t, int a, void* m2, void* r, unsigned long mask, int b, int c) {
    ++attachCalls; aModel = m; aTexture = t; aA = a; aModel2 = m2; aRecord = r; aMask = mask; aB = b; aC = c;
}
int main() {
    alignas(16) static unsigned char arena[4096], object[1100];
    w10_actor_d_host_base = arena;
    int bad = 0;
    unsigned block = 256, w12 = 1000, w20 = 2000, w196 = 1100, w204 = 1200;
    for (int attachment : {0, 1, 9}) for (int cut : {0, 1, -1}) {
        std::memset(object, 0, sizeof object);
        std::memcpy(object + 36, &block, 4); std::memcpy(object + 984, &attachment, 4);
        std::memcpy(arena + block + 12, &w12, 4); std::memcpy(arena + block + 20, &w20, 4);
        std::memcpy(arena + block + 196, &w196, 4); std::memcpy(arena + block + 204, &w204, 4);
        cutMode = static_cast<signed char>(cut); drawCalls = attachCalls = 0; dSecond = object;
        msg_draw__10CloudGiant(object);
        bool drawn = drawCalls == 1 && dSelf == object && dModel == arena + w12 && dTexture == arena + w20 && dSecond == nullptr;
        bool wantAttach = attachment != 0 && cut == 0;
        bool attached = wantAttach ? (attachCalls == 1 && aModel == arena + w196 && aTexture == arena + w204 && aA == 0 && aModel2 == arena + w12 && aRecord == recordMarker && aMask == 0xfffffffful && aB == -1 && aC == 0) : attachCalls == 0;
        if (!drawn || !attached) { std::printf("fail: attachment=%d cut=%d\n", attachment, cut); bad = 1; }
    }
    return bad;
}
