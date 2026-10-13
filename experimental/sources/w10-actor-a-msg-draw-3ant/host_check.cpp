// Host contract for msg_draw__3Ant; not an EE input. 32-bit words in the objects are offsets from
// a host base (W10_ACTOR_A_HOST), so the real field offsets (+36, +12, +20, +28, +960) are used.
#define W10_ACTOR_A_HOST 1
#include <cstdio>
#include <cstring>
unsigned char* w10_actor_a_host_base;
#include "msg_draw.cpp"

static void* seenSelf; static void* seenModel; static void* seenTexture; static void* seenSecond; static int calls;
int draw__8CreatureP12_modelHeaderP8_textureT1(void* s, void* m, void* t, void* x) {
    ++calls; seenSelf = s; seenModel = m; seenTexture = t; seenSecond = x; return 42;
}
int main() {
    static unsigned char arena[4096] __attribute__((aligned(16)));
    static unsigned char object[1024] __attribute__((aligned(16)));
    w10_actor_a_host_base = arena;
    unsigned block = 256; std::memcpy(object + 36, &block, 4);
    unsigned model = 1000, texA = 2000, texB = 3000;
    std::memcpy(arena + block + 12, &model, 4); std::memcpy(arena + block + 20, &texA, 4); std::memcpy(arena + block + 28, &texB, 4);
    int bad = 0;
    struct Case { unsigned char flag; unsigned tex; } cases[] = {{0, 2000}, {1, 3000}, {0x80, 3000}, {0xff, 3000}};
    for (const Case& c : cases) {
        object[960] = c.flag; calls = 0;
        int r = msg_draw__3Ant(object);
        if (r != 42 || calls != 1 || seenSelf != object || seenModel != arena + 1000 || seenTexture != arena + c.tex || seenSecond != nullptr) {
            std::printf("fail: flag %d\n", c.flag); bad = 1;
        }
    }
    return bad;
}
