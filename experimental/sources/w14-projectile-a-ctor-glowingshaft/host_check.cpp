// Host contract for __12GlowingShaftG6Point3iiii; not an EE input. The two callees log their arguments. The authored function was
// also compared with the original in a private R5900 run (40 scenarios, every original word covered).
#define W14_PROJECTILE_A_HOST 1
#include "../w14-projectile-a-common/access.hpp"
#include <cstring>
unsigned char* w14_projectile_a_host_base;
unsigned w14_projectile_a_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
void w14_projectile_a_host_hurt(unsigned, void*, float, void*, int) {}
static unsigned char arena[512];
static int order[4], n_order;
static void* got_object;
static Point3 got_position;
static void* got_map; static char** got_names;
static unsigned word12_at_run_list;
extern "C" {
unsigned char vt_GlowingShaft[16] __asm__("_vt$12GlowingShaft");
void game_object_construct(void* self, Point3 position, void* map, char** names) __asm__("__10GameObjectG6Point3PQ210GameObject11resourceMapPPc");
void game_object_construct(void* self, Point3 position, void* map, char** names) {
    order[n_order++] = 1; got_object = self; got_position = position; got_map = map; got_names = names;
    std::memset(static_cast<unsigned char*>(self) + 12, 0x11, 4);
}
void objectAddToRunList__FP10GameObject(void* object) {
    order[n_order++] = 2;
    word12_at_run_list = *reinterpret_cast<unsigned*>(static_cast<unsigned char*>(object) + 12);
    if (static_cast<unsigned char*>(object)[40] == 48) order[n_order++] = 99;
}
}
#include "ctor.cpp"

int main() {
    using namespace w14pa;
    w14_projectile_a_host_base = arena;
    unsigned char* o = arena + 64;
    std::memset(arena, 0xee, sizeof arena);
    Point3 p = {1.0f, -2.5f, 300.0f};
    __12GlowingShaftG6Point3iiii(o, p, 90, 70000, -2, 250);
    if (n_order != 2 || order[0] != 1 || order[1] != 2) return 1;
    if (got_object != o || got_position.x != 1.0f || got_position.y != -2.5f || got_position.z != 300.0f) return 2;
    if (got_map != nullptr || got_names != nullptr) return 3;
    if (word(o, 12) != (0x11111111u | 0x4009u) || word12_at_run_list != word(o, 12)) return 4;
    if (word(o, 72) != to_word(vt_GlowingShaft)) return 5;
    if (word(o, 76) != 90 || word(o, 80) != 90) return 6;
    if (half(o, 84) != static_cast<short>(70000) || half(o, 86) != -2 || half(o, 88) != 250) return 7;
    if (o[40] != 48) return 8;
    // bytes the routine must not touch
    if (o[39] != 0xee || o[41] != 0xee || o[90] != 0xee || o[-1] != 0xee) return 9;
    return 0;
}
