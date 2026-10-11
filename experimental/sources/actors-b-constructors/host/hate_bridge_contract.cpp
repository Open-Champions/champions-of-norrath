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
    raw[12] = 0x04; raw[13] = raw[14] = raw[15] = 0;
    raw[40] = 0x11;
    return 0x1u;
}
extern "C" unsigned norrath_hate_bridge_set_passable_state(void* object, int enabled) {
    expect(stage == 1, "passable state before run list");
    stage = 2;
    passable_arg = enabled;
    auto* raw = reinterpret_cast<unsigned char*>(object);
    unsigned flags = unsigned(raw[12]) | (unsigned(raw[13]) << 8) | (unsigned(raw[14]) << 16) | (unsigned(raw[15]) << 24);
    expect(flags == 0x05u, "flags ored before passable state");
    return 0x111u;
}
extern "C" unsigned norrath_actor_add_run_list(void* object) {
    (void)object;
    expect(stage == 2, "run list after passable state");
    stage = 3;
    return 0x222u;
}
unsigned char norrath_actors_b_view_44eeb0[4];
static unsigned word_at(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
int main() {
    alignas(16) unsigned char object[128];
    unsigned char point[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    char name[] = "bridge";
    char* args[] = {nullptr};
    std::memset(object, 0x5a, sizeof object);
    const unsigned result = norrath_construct_HateBridge(object, name, point, 8, args);
    expect(result == 0x222u, "forwards run-list result, not passable state");
    expect(stage == 3 && passable_arg == 1, "call order");
    expect(object[40] == 0xb0, "byte 40 is the low 8 bits of -80");
    expect(word_at(object + 72) == static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(norrath_actors_b_view_44eeb0)),
           "vtable");
    if (fails != 0) return 1;
    std::puts("hate bridge constructor passed");
    return 0;
}
