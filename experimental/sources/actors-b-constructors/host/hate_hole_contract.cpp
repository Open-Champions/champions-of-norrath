#include "../detail.hpp"
#include <cstdio>
#include <cstdint>
#include <cstring>
static int fails;
static void expect(bool cond, const char* message) {
    if (!cond) { std::printf("FAIL %s\n", message); ++fails; }
}
static int stage;
static void* seen_world;
static int seen_index;
static unsigned char world_body[8];
extern "C" unsigned norrath_construct_AutoProp(void* object, char* name, const void* point,
                                              short type, char** arguments) {
    (void)name; (void)point; (void)type; (void)arguments;
    expect(stage == 0, "AutoProp first");
    stage = 1;
    auto* raw = reinterpret_cast<unsigned char*>(object);
    raw[12] = 0x10; raw[13] = raw[14] = raw[15] = 0;
    raw[396] = 0xfe; raw[397] = 0xff; raw[398] = 0xff; raw[399] = 0xff;
    return 0x1u;
}
extern "C" int norrath_world_get_door_height(void* world, int index) {
    expect(stage == 1, "height after AutoProp");
    stage = 2;
    seen_world = world;
    seen_index = index;
    auto* raw = reinterpret_cast<unsigned char*>(world) - 0; // world body itself
    (void)raw;
    return -2;
}
extern "C" unsigned norrath_actor_add_run_list(void* object) {
    expect(stage == 2, "run list after height");
    stage = 3;
    auto* raw = reinterpret_cast<unsigned char*>(object);
    unsigned flags = unsigned(raw[12]) | (unsigned(raw[13]) << 8) | (unsigned(raw[14]) << 16) | (unsigned(raw[15]) << 24);
    unsigned bits = unsigned(raw[408]) | (unsigned(raw[409]) << 8) | (unsigned(raw[410]) << 16) | (unsigned(raw[411]) << 24);
    expect(flags == 0x11u, "flags before run list");
    expect(bits == 0xc0000000u, "signed height converted before run list");
    return 0xe55u;
}
unsigned char norrath_actors_b_view_44ef58[4];
unsigned char norrath_actors_b_view_4844a0[4];
static unsigned word_at(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
int main() {
    const unsigned world_bits = static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(world_body));
    norrath_actors_b_view_4844a0[0] = static_cast<unsigned char>(world_bits);
    norrath_actors_b_view_4844a0[1] = static_cast<unsigned char>(world_bits >> 8);
    norrath_actors_b_view_4844a0[2] = static_cast<unsigned char>(world_bits >> 16);
    norrath_actors_b_view_4844a0[3] = static_cast<unsigned char>(world_bits >> 24);
    alignas(16) unsigned char object[432];
    unsigned char point[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    char name[] = "hole";
    char* args[] = {nullptr};
    std::memset(object, 0x5a, sizeof object);
    const unsigned result = norrath_construct_HateHole(object, name, point, 1, args);
    expect(result == 0xe55u, "forwards run-list result");
    expect(stage == 3, "call order");
    const auto world_back = reinterpret_cast<void*>(static_cast<std::uintptr_t>(world_bits));
    expect(seen_world == world_back, "world pointer loaded from the global");
    expect(seen_index == -2, "door index is the signed word at object+396");
    expect(word_at(object + 72) == static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(norrath_actors_b_view_44ef58)),
           "vtable");
    if (fails != 0) return 1;
    std::puts("hate hole constructor passed");
    return 0;
}
