// Host check only. Not an EE build input.
#define NORRATH_RTTI_C_HOST
#include <cstdio>
#include <cstring>
#include <string>

extern "C" {
int objectKillingEveryone;
const unsigned char norrath_ctors_ant_lantern_view_44ebb0[8] = {};
const unsigned char norrath_rtti_entities_a_view_44ed78[8] = {};
}
static std::string calls;
static unsigned char* current;
static void *anim_state, *game_object, *freed_object;
static int anim_flags, game_flags;
static unsigned vptr_at_first_child, vptr_at_anim, vptr_at_game;
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

static void *child_arg[2];
static int child_flags[2];
static void child_delete0(void* self, int flags) {
    calls += "child0;"; child_arg[0] = self; child_flags[0] = flags;
    if (vptr_at_first_child == 0) std::memcpy(&vptr_at_first_child, current + 72, 4);
}
static void child_delete1(void* self, int flags) {
    calls += "child1;"; child_arg[1] = self; child_flags[1] = flags;
    if (vptr_at_first_child == 0) std::memcpy(&vptr_at_first_child, current + 72, 4);
}
#include "dtor.cpp"

static void put(unsigned char* base, unsigned off, unsigned value) { std::memcpy(base + off, &value, 4); }
int main() {
    int failures = 0;
    alignas(8) unsigned char object[512];
    alignas(8) unsigned char children[2][128];
    alignas(8) unsigned char tables[2][32];
    const int adjustments[2] = {16, -32};
    void (*const functions[2])(void*, int) = {child_delete0, child_delete1};
    for (int kill : {0, 1, 7}) {
        for (int has0 = 0; has0 < 2; ++has0) {
            for (int has1 = 0; has1 < 2; ++has1) {
                for (int flags : {0, 1, 2, 3, -1}) {
                    std::memset(object, 0x5c, sizeof object);
                    for (int i = 0; i < 2; ++i) {
                        std::memset(children[i], 0x44, sizeof children[i]);
                        std::memset(tables[i], 0x99, sizeof tables[i]);
                        const unsigned short adj = static_cast<unsigned short>(adjustments[i]);
                        std::memcpy(tables[i] + 8, &adj, 2);
                        const unsigned fbits = static_cast<unsigned>(reinterpret_cast<unsigned long long>(functions[i]));
                        put(tables[i], 12, fbits);
                        rtti_c::host_note(reinterpret_cast<void*>(functions[i]));
                        rtti_c::host_note(tables[i]);
                        rtti_c::host_note(children[i]);
                        put(children[i], 72, static_cast<unsigned>(reinterpret_cast<unsigned long long>(tables[i])));
                    }
                    put(object, 320, has0 ? static_cast<unsigned>(reinterpret_cast<unsigned long long>(children[0])) : 0u);
                    put(object, 324, has1 ? static_cast<unsigned>(reinterpret_cast<unsigned long long>(children[1])) : 0u);
                    objectKillingEveryone = kill;
                    current = object; calls.clear(); vptr_at_first_child = 0;
                    child_arg[0] = child_arg[1] = anim_state = game_object = freed_object = nullptr;
                    child_flags[0] = child_flags[1] = -99;
                    dtor_13AutoContainer(object, flags);
                    std::string want;
                    if (kill == 0) {
                        if (has0) want += "child0;";
                        if (has1) want += "child1;";
                    }
                    want += "anim;game;";
                    if (flags & 1) want += "free;";
                    if (calls != want) { std::fprintf(stderr, "fail: calls %s want %s\n", calls.c_str(), want.c_str()); ++failures; }
                    if (kill == 0 && has0 && (child_arg[0] != children[0] + 16 || child_flags[0] != 3)) { std::fprintf(stderr, "fail: child 0 call\n"); ++failures; }
                    if (kill == 0 && has1 && (child_arg[1] != children[1] - 32 || child_flags[1] != 3)) { std::fprintf(stderr, "fail: child 1 call\n"); ++failures; }
                    if (kill == 0 && (has0 || has1) && vptr_at_first_child != rtti_c::address_bits(norrath_ctors_ant_lantern_view_44ebb0)) {
                        std::fprintf(stderr, "fail: own vtable not stored before the children\n"); ++failures;
                    }
                    const unsigned base_vt = rtti_c::address_bits(norrath_rtti_entities_a_view_44ed78);
                    if (vptr_at_anim != base_vt || vptr_at_game != base_vt) { std::fprintf(stderr, "fail: base vtable\n"); ++failures; }
                    if (anim_state != object + 152 || anim_flags != 2 || game_object != object || game_flags != 0) { std::fprintf(stderr, "fail: base arguments\n"); ++failures; }
                    if ((flags & 1) && freed_object != object) { std::fprintf(stderr, "fail: freed object\n"); ++failures; }
                }
            }
        }
    }
    return failures ? 1 : 0;
}
