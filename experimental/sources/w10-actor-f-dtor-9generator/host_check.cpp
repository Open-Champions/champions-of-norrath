// Host contract for _$_9Generator. Includes the source; not an EE input.
#define NORRATH_VSLOTS_B_HOST 1
#include "dtor.cpp"
#include <initializer_list>
extern "C" const unsigned char norrath_ctors_ant_lantern_view_44bbd0[4] = {};
static int order = 0, list_at = -1, base_at = -1, free_at = -1, frees = 0, base_flags = -1;
static void *list_arg, *base_arg, *free_arg;
static unsigned vt_at_list = 0, vt_at_base = 0;
extern "C" void listDestroy__FP8ListHead(void* head) {
    list_at = order++; list_arg = head;
    vt_at_list = vslots_b::read_word(vslots_b::address(head) - 984u + 72u);
}
extern "C" void _$_8Creature(void* object, int flags) {
    base_at = order++; base_arg = object; base_flags = flags;
    vt_at_base = vslots_b::read_word(vslots_b::address(object) + 72u);
}
extern "C" void blockFree__FPv(void* object) { free_at = order++; free_arg = object; ++frees; }
int main() {
    using namespace vslots_b;
    for (int flags : {0, 1, 2, 3, 5, -1, -2}) {
        alignas(16) unsigned char object[1100];
        for (unsigned i = 0; i < sizeof object; ++i) object[i] = 0x41;
        order = 0; frees = 0; list_at = base_at = free_at = -1;
        _$_9Generator(object, flags);
        if (list_at != 0 || base_at != 1 || list_arg != object + 984 || base_arg != object || base_flags != 0) return 1;
        if (vt_at_list != address_bits(norrath_ctors_ant_lantern_view_44bbd0)) return 2;
        if (vt_at_base != address_bits(norrath_ctors_ant_lantern_view_44bbd0)) return 3;
        if ((flags & 1) != 0) { if (frees != 1 || free_at != 2 || free_arg != object) return 4; }
        else if (frees != 0) return 5;
        for (unsigned i = 0; i < sizeof object; ++i)
            if ((i < 72 || i >= 76) && object[i] != 0x41) return 6;
    }
    return 0;
}
