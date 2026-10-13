// Host contract for msg_draw__7Vampire; not an EE input. 32-bit words are offsets from a host base.
#define W10_ACTOR_O_HOST 1
#include <cstring>
unsigned char* w10_actor_o_host_base;
#include "msg_draw.cpp"
static unsigned char arena[0x2000] __attribute__((aligned(16)));
static int calls; static void *s_self, *s_model, *s_texture, *s_extra;
extern "C" void* draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, void* texture2) {
    ++calls; s_self = self; s_model = model; s_texture = texture; s_extra = texture2; return nullptr;
}
int main() {
    unsigned char* const obj = arena + 0x400;
    std::memset(arena, 0, sizeof arena); w10_actor_o_host_base = arena;
    const unsigned group = 0x1000, model = 0x1234, texture = 0x1456, ptr = group;
    std::memcpy(obj + 36, &ptr, 4); std::memcpy(arena + group + 12, &model, 4); std::memcpy(arena + group + 20, &texture, 4);
    msg_draw__7Vampire(obj);
    if (calls != 1 || s_self != obj || s_model != arena + model || s_texture != arena + texture || s_extra != nullptr) return 1;
    return 0;
}
