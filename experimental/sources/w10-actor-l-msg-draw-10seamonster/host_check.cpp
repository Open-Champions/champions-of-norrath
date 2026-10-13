// Host contract for msg_draw__10SeaMonster; not an EE input. Words holding addresses are offsets
// from a host base. The Creature draw routine logs its arguments; the object must not change.
#define W10_ACTOR_L_HOST 1
#include <cstring>
#include <initializer_list>
unsigned char* w10_actor_l_host_base;
unsigned w10_actor_l_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
static void *s_self, *s_model, *s_tex; static int s_extra = -1, ncalls;
extern "C" void draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, int extra) {
    s_self = self; s_model = model; s_tex = texture; s_extra = extra; ++ncalls;
}
#include "msg_draw.cpp"
alignas(16) static unsigned char arena[0x1000], ref[0x1000];
int main() {
    w10_actor_l_host_base = arena;
    int bad = 0;
    for (unsigned g : {0x200u, 0x340u}) {
        std::memset(arena, 0x11, sizeof arena);
        unsigned v = g; std::memcpy(arena + 0x100 + 36, &v, 4);
        unsigned m = 0x600 + g, t = 0x700 + g;
        std::memcpy(arena + g + 12, &m, 4); std::memcpy(arena + g + 20, &t, 4);
        std::memcpy(ref, arena, sizeof arena); ncalls = 0;
        msg_draw__10SeaMonster(arena + 0x100);
        if (ncalls != 1 || s_self != arena + 0x100 || s_model != arena + m || s_tex != arena + t || s_extra != 0) bad = 1;
        if (std::memcmp(arena, ref, sizeof arena) != 0) bad = 1;
    }
    return bad;
}
