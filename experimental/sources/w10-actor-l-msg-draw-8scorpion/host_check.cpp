// Host contract for msg_draw__8Scorpion; not an EE input. Words holding addresses are offsets
// from a host base. The draw routine logs its arguments; the object must not change.
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
alignas(16) static unsigned char arena[0x2000], ref[0x2000];
int main() {
    w10_actor_l_host_base = arena;
    int bad = 0;
    for (int idx : {0, 1, 5, 127, -1, -3, -128}) {
        std::memset(arena, 0x11, sizeof arena);
        const unsigned g = 0x1000;
        unsigned v = g; std::memcpy(arena + 0x100 + 36, &v, 4);
        arena[0x100 + 960] = static_cast<unsigned char>(idx);
        for (int k = -128; k <= 127; ++k) { unsigned t = 0x1d00 + (k + 128); std::memcpy(arena + g + k * 8 + 20, &t, 4); }
        const unsigned m = 0x1c00; std::memcpy(arena + g + 12, &m, 4);
        std::memcpy(ref, arena, sizeof arena); ncalls = 0;
        msg_draw__8Scorpion(arena + 0x100);
        if (ncalls != 1 || s_self != arena + 0x100 || s_model != arena + m || s_tex != arena + (idx == -1 ? m : 0x1d00 + (idx + 128)) || s_extra != 0) bad = 1;
        if (std::memcmp(arena, ref, sizeof arena) != 0) bad = 1;
    }
    return bad;
}
