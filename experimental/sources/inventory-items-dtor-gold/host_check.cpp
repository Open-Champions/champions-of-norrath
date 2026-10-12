#include <cstdint>
#include <cstring>

extern "C" void _$_4Gold(void* object, int flags);
extern "C" unsigned char norrath_player_view_44cd48[8] = {};

static int order[8], steps;
static unsigned vt_at_animation, vt_at_base;
static void* animation_object;
static int animation_flags = -1, base_flags = -1;
static void* base_object;
static void* freed;

static unsigned word(const unsigned char* p) { unsigned v; std::memcpy(&v, p, 4); return v; }
extern "C" void _$_14AnimationState(void* state, int flags) {
    order[steps++] = 1;
    animation_object = state;
    animation_flags = flags;
    vt_at_animation = word(static_cast<unsigned char*>(state) - 16);
}
extern "C" void norrath_vt_dtor_10GameObject(void* object, int flags) {
    order[steps++] = 2;
    base_object = object;
    base_flags = flags;
    vt_at_base = word(static_cast<unsigned char*>(object) + 72);
}
extern "C" void norrath_block_free(void* object) { order[steps++] = 3; freed = object; }

int main() {
    unsigned char object[192];
    std::memset(object, 0xcc, sizeof object);
    _$_4Gold(object, 1);
    if (steps != 3 || order[0] != 1 || order[1] != 2 || order[2] != 3) return 1;
    if (animation_object != object + 88 || animation_flags != 2) return 2;
    if (base_object != object || base_flags != 0 || freed != object) return 3;
    const unsigned want = static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(norrath_player_view_44cd48));
    if (vt_at_animation != want || vt_at_base != want) return 4;
    steps = 0; freed = nullptr;
    _$_4Gold(object, 2);   // bit 0 clear: no free
    if (steps != 2 || freed != nullptr) return 5;
    steps = 0;
    _$_4Gold(object, 3);
    if (steps != 3 || freed != object) return 6;
    steps = 0; freed = nullptr;
    _$_4Gold(object, -2);
    if (steps != 2 || freed != nullptr) return 7;
    return 0;
}
