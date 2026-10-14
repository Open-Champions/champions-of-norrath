// Host contract for msg_draw__6Cthulu; not an EE input. 32-bit words in the objects are offsets
// from a host base (W10_ACTOR_D_HOST), so the real field offsets (+36, +12, +20) are used.
#define W10_ACTOR_D_HOST 1
#include <cstdio>
#include <cstring>
unsigned char* w10_actor_d_host_base;
#include "msg_draw.cpp"
static void *seenSelf, *seenModel, *seenTexture, *seenSecond; static int calls;
int draw__8CreatureP12_modelHeaderP8_textureT1(void* s, void* m, void* t, void* x) {
    ++calls; seenSelf = s; seenModel = m; seenTexture = t; seenSecond = x; return 42;
}
int main() {
    alignas(16) static unsigned char arena[4096], object[1100];
    w10_actor_d_host_base = arena;
    unsigned block = 256, model = 1000, texture = 2000, decoy = 3000;
    std::memcpy(object + 36, &block, 4);
    std::memcpy(arena + block + 12, &model, 4); std::memcpy(arena + block + 20, &texture, 4);
    std::memcpy(arena + block + 16, &decoy, 4); std::memcpy(arena + block + 28, &decoy, 4);
    seenSecond = object;
    msg_draw__6Cthulu(object);
    if (calls != 1 || seenSelf != object || seenModel != arena + model || seenTexture != arena + texture || seenSecond != nullptr) {
        std::puts("fail"); return 1;
    }
    return 0;
}
