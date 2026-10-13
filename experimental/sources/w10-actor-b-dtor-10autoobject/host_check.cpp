// Host check only. Not an EE build input.
#define NORRATH_RTTI_C_HOST
#include <cstdio>
#include <cstring>
#include <string>

extern "C" const unsigned char norrath_rtti_entities_a_view_44ed78[8] = {};
static std::string calls;
static unsigned char* current;
static void *anim_state, *game_object, *freed_object;
static int anim_flags, game_flags;
static unsigned vptr_at_anim, vptr_at_game;
extern "C" void animation_state_destructor(void* state, int flags) asm("_$_14AnimationState");
extern "C" void animation_state_destructor(void* state, int flags) {
    calls += "anim;"; anim_state = state; anim_flags = flags;
    std::memcpy(&vptr_at_anim, current + 72, 4);
}
extern "C" void game_object_destructor(void* object, int flags) asm("_$_10GameObject");
extern "C" void game_object_destructor(void* object, int flags) {
    calls += "game;"; game_object = object; game_flags = flags;
    std::memcpy(&vptr_at_game, current + 72, 4);
}
extern "C" void norrath_block_free(void* object) { calls += "free;"; freed_object = object; }
#include "dtor.cpp"

int main() {
    int failures = 0;
    alignas(8) unsigned char object[512];
    const int flags_list[] = {0, 1, 2, 3, -2, -1, 0x10001, 0x100, static_cast<int>(0x80000000u)};
    for (int flags : flags_list) {
        std::memset(object, 0x5c, sizeof object);
        current = object; calls.clear(); anim_state = game_object = freed_object = nullptr;
        dtor_10AutoObject(object, flags);
        const bool frees = (flags & 1) != 0;
        if (calls != (frees ? "anim;game;free;" : "anim;game;")) { std::fprintf(stderr, "fail: calls %s\n", calls.c_str()); ++failures; }
        if (anim_state != object + 152 || anim_flags != 2 || game_object != object || game_flags != 0) { std::fprintf(stderr, "fail: arguments\n"); ++failures; }
        const unsigned want = rtti_c::address_bits(norrath_rtti_entities_a_view_44ed78);
        if (vptr_at_anim != want || vptr_at_game != want) { std::fprintf(stderr, "fail: vtable not stored first\n"); ++failures; }
        if (frees && freed_object != object) { std::fprintf(stderr, "fail: freed object\n"); ++failures; }
        if (object[71] != 0x5c || object[76] != 0x5c) { std::fprintf(stderr, "fail: neighbours\n"); ++failures; }
    }
    return failures ? 1 : 0;
}
