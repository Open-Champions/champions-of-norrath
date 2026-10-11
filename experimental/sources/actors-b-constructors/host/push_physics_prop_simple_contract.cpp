#include "../detail.hpp"
#include <cstdio>
#include <cstdint>
#include <cstring>
static int fails;
static void expect(bool cond, const char* message) {
    if (!cond) { std::printf("FAIL %s\n", message); ++fails; }
}
static int stage;
static void* seen_map;
static char** seen_args;
extern "C" unsigned norrath_game_object_construct(void* object, const void* point, void* resource_map, char** arguments) {
    expect(stage == 0, "base first");
    stage = 1;
    seen_map = resource_map;
    seen_args = arguments;
    auto* raw = reinterpret_cast<unsigned char*>(object);
    std::memcpy(raw + 16, point, 12);
    raw[12] = 0x01; raw[13] = raw[14] = raw[15] = 0;
    return 0x1u;
}
extern "C" unsigned norrath_actor_add_run_list(void* object) {
    expect(stage == 1, "run list after base");
    stage = 2;
    auto* raw = reinterpret_cast<unsigned char*>(object);
    unsigned flags = unsigned(raw[12]) | (unsigned(raw[13]) << 8) | (unsigned(raw[14]) << 16) | (unsigned(raw[15]) << 24);
    expect(flags == 0x11u, "flag 0x10 ored before run list");
    expect(raw[232] == 0x5a, "point not copied before run list");
    return 0xb22u;
}
unsigned char norrath_actors_b_view_448a70[4];
static unsigned word_at(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
int main() {
    alignas(16) unsigned char object[256];
    unsigned char point[12] = {0x44, 0x33, 0x22, 0x11, 5, 6, 7, 8, 9, 10, 11, 12};
    std::memset(object, 0x5a, sizeof object);
    const unsigned result = norrath_construct_PushPhysicsPropSimple(object, point);
    expect(seen_map == nullptr && seen_args == nullptr, "null map and arguments");
    expect(result == 0x11223344u, "low V0 is the word at object+16");
    expect(std::memcmp(object + 232, object + 16, 12) == 0, "position copied after run list");
    expect(word_at(object + 72) == static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(norrath_actors_b_view_448a70)),
           "vtable");
    expect(stage == 2, "run list called");
    if (fails != 0) return 1;
    std::puts("push physics prop simple constructor passed");
    return 0;
}
