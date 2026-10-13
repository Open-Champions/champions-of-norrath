// Host contract for msg_draw__5Ghoul; not an EE input. Words are offsets from a host base.
#define W10_ACTOR_G_HOST 1
#include <cstdio>
#include <cstring>
unsigned char* w10g_host_base;
void* w10g_host_fn(unsigned) { return nullptr; }
#include "msg_draw.cpp"

static unsigned char arena[0x2000] __attribute__((aligned(16)));
static void *seenSelf, *seenModel, *seenTexture, *seenSecond; static int calls;
extern "C" int draw__8CreatureP12_modelHeaderP8_textureT1(void* s, void* m, void* t, void* x) {
    ++calls; seenSelf = s; seenModel = m; seenTexture = t; seenSecond = x; return 77;
}
int main() {
    w10g_host_base = arena;
    unsigned char* object = arena + 0x1000;
    unsigned block = 0x100; std::memcpy(object + 36, &block, 4);
    unsigned model = 0x400, texture = 0x500, other = 0x600;
    std::memcpy(arena + block + 12, &model, 4); std::memcpy(arena + block + 20, &texture, 4);
    std::memcpy(arena + block + 28, &other, 4);
    int r = msg_draw__5Ghoul(object);
    int bad = 0;
    if (r != 77 || calls != 1) { std::puts("fail: result or call count"); bad = 1; }
    if (seenSelf != object || seenModel != arena + model || seenTexture != arena + texture || seenSecond != nullptr) {
        std::puts("fail: draw arguments (model from +12, texture from +20, null fourth)"); bad = 1;
    }
    if (!bad) std::puts("ghoul msg_draw ok");
    return bad;
}
