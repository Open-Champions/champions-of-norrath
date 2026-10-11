#include "../detail.hpp"
#include <cstdio>
#include <cstdint>
#include <cstring>
static int fails;
static void expect(bool cond, const char* message) {
    if (!cond) { std::printf("FAIL %s\n", message); ++fails; }
}
static int stage;
static int passable_arg;
extern "C" unsigned norrath_construct_AutoProp(void* object, char* name, const void* point,
                                              short type, char** arguments) {
    (void)name; (void)point; (void)type; (void)arguments;
    expect(stage == 0, "AutoProp first");
    stage = 1;
    auto* raw = reinterpret_cast<unsigned char*>(object);
    raw[12] = 0x10; raw[13] = raw[14] = raw[15] = 0;
    raw[72] = 0xee;
    return 0x1u;
}
extern "C" unsigned norrath_actor_add_run_list(void* object) {
    expect(stage == 1, "run list after AutoProp");
    stage = 2;
    auto* raw = reinterpret_cast<unsigned char*>(object);
    unsigned flags = unsigned(raw[12]) | (unsigned(raw[13]) << 8) | (unsigned(raw[14]) << 16) | (unsigned(raw[15]) << 24);
    expect(flags == 0x11u, "flags ored before run list");
    expect(raw[72] != 0xee, "vtable stored before run list");
    return 0xb22u;
}
extern "C" unsigned norrath_goth_trap_set_passable_state(void* object, int enabled) {
    (void)object;
    expect(stage == 2, "passable state after run list");
    stage = 3;
    passable_arg = enabled;
    return 0xc33u;
}
unsigned char norrath_actors_b_view_44f000[4];
static unsigned word_at(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
int main() {
    alignas(16) unsigned char object[128];
    unsigned char point[12] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 11, 10};
    char name[] = "goth";
    char* args[] = {name};
    std::memset(object, 0x5a, sizeof object);
    const unsigned result = norrath_construct_GothTrap(object, name, point, static_cast<short>(-4), args);
    expect(result == 0xc33u, "forwards passable-state result");
    expect(stage == 3 && passable_arg == 1, "call order and argument");
    expect(word_at(object + 72) == static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(norrath_actors_b_view_44f000)),
           "vtable");
    if (fails != 0) return 1;
    std::puts("goth trap constructor passed");
    return 0;
}
