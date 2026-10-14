#include <cstdint>
#include <cstring>

extern "C" void _$_8ItemProp(void* object, int flags);
extern "C" unsigned char norrath_player_view_44d018[8] = {};

static int base_calls, free_calls, base_flags = -1;
static unsigned vt_at_base;
static void* base_object;
static void* freed;
static bool free_first;

extern "C" void norrath_vt_dtor_10GameObject(void* object, int flags) {
    ++base_calls;
    base_object = object;
    base_flags = flags;
    std::memcpy(&vt_at_base, static_cast<unsigned char*>(object) + 72, 4);
}
extern "C" void norrath_block_free(void* object) {
    if (base_calls == 0) free_first = true;
    ++free_calls;
    freed = object;
}

int main() {
    unsigned char object[400];
    std::memset(object, 0xcc, sizeof object);
    _$_8ItemProp(object, 1);
    if (base_calls != 1 || free_calls != 1 || free_first) return 1;
    if (base_object != object || base_flags != 0 || freed != object) return 2;
    if (vt_at_base != static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(norrath_player_view_44d018))) return 3;
    base_calls = free_calls = 0;
    _$_8ItemProp(object, 2);
    if (base_calls != 1 || free_calls != 0 || base_flags != 0) return 4;
    base_calls = free_calls = 0;
    _$_8ItemProp(object, 0xffff);
    if (base_calls != 1 || free_calls != 1) return 5;
    return 0;
}
