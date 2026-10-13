// Host contract for _$_7FireFly. Includes the source; not an EE input.
#define NORRATH_VSLOTS_B_HOST 1
#include "dtor.cpp"
#include <initializer_list>
extern "C" const unsigned char norrath_ctors_ant_lantern_view_44bdb0[4] = {};
extern "C" const unsigned char norrath_ctors_ant_lantern_view_44bea0[4] = {};
static int order = 0, light_at = -1, base_at = -1, free_at = -1, frees = 0;
static void *light_arg, *base_arg, *free_arg;
static int light_flags = -1, base_flags = -1;
static unsigned vt_at_light = 0, vt_at_base = 0;
extern "C" void _$_5Light(void* light, int flags) {
    light_at = order++; light_arg = light; light_flags = flags;
    vt_at_light = vslots_b::read_word(vslots_b::address(light) - 1064u + 72u);
}
extern "C" void _$_8Creature(void* object, int flags) {
    base_at = order++; base_arg = object; base_flags = flags;
    vt_at_base = vslots_b::read_word(vslots_b::address(object) + 72u);
}
extern "C" void blockFree__FPv(void* object) { free_at = order++; free_arg = object; ++frees; }
int main() {
    using namespace vslots_b;
    for (int flags : {0, 1, 2, 3, 5, -1, -2}) {
        alignas(16) unsigned char object[1200];
        for (unsigned i = 0; i < sizeof object; ++i) object[i] = 0x41;
        order = 0; frees = 0; light_at = base_at = free_at = -1;
        _$_7FireFly(object, flags);
        if (light_at != 0 || base_at != 1 || light_arg != object + 1064 || light_flags != 2 || base_arg != object || base_flags != 0) return 1;
        if (vt_at_light != address_bits(norrath_ctors_ant_lantern_view_44bdb0)) return 2;
        if (vt_at_base != address_bits(norrath_ctors_ant_lantern_view_44bea0)) return 3;
        if (read_word(address(object) + 72u) != address_bits(norrath_ctors_ant_lantern_view_44bea0)) return 4;
        if ((flags & 1) != 0) { if (frees != 1 || free_at != 2 || free_arg != object) return 5; }
        else if (frees != 0) return 6;
        for (unsigned i = 0; i < sizeof object; ++i)
            if ((i < 72 || i >= 76) && object[i] != 0x41) return 7;
    }
    return 0;
}
