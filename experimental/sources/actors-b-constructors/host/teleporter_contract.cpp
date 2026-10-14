#include "../detail.hpp"
#include <cstdio>
#include <cstdint>
#include <cstring>
static int fails;
static void expect(bool cond, const char* message) {
    if (!cond) { std::printf("FAIL %s\n", message); ++fails; }
}
static int stage;
static float seen_scale;
static short seen_type;
static void* seen_map;
static const void* seen_point;
static unsigned char seen_bytes[12];
extern "C" unsigned norrath_creature_construct(void* object, const void* point, short type, void* resource_map,
                                              float scale, char** arguments) {
    (void)arguments;
    expect(stage == 0, "creature first");
    stage = 1;
    seen_scale = scale;
    seen_type = type;
    seen_map = resource_map;
    seen_point = point;
    std::memcpy(seen_bytes, point, 12);
    auto* raw = reinterpret_cast<unsigned char*>(object);
    raw[12] = 0x10; raw[13] = raw[14] = raw[15] = 0;
    raw[572] = 0x99; raw[573] = 0x99;
    return 0x1u;
}
extern "C" unsigned norrath_actor_add_slow_run_list(void* object) {
    expect(stage == 1, "slow list after creature");
    stage = 2;
    auto* raw = reinterpret_cast<unsigned char*>(object);
    unsigned flags = unsigned(raw[12]) | (unsigned(raw[13]) << 8) | (unsigned(raw[14]) << 16) | (unsigned(raw[15]) << 24);
    unsigned half = unsigned(raw[572]) | (unsigned(raw[573]) << 8);
    expect(flags == 0x1fu && half == 0u, "flags and half updated before slow list");
    return 0xabcdu;
}
unsigned char norrath_actors_b_view_448e30[4];
unsigned char norrath_actors_b_view_4816d4[4];
static unsigned word_at(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
int main() {
    const float scale = 2.5f;
    std::memcpy(norrath_actors_b_view_4816d4, &scale, 4);
    alignas(16) unsigned char object[576];
    unsigned char point[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    char* args[] = {nullptr};
    std::memset(object, 0x5a, sizeof object);
    const unsigned result = norrath_construct_Teleporter(object, point, static_cast<short>(-3), args);
    expect(result == 0xabcdu && stage == 2, "forwards slow-list result");
    expect(seen_map == nullptr && seen_type == -3 && seen_scale == 2.5f, "creature arguments");
    expect(seen_point != point && std::memcmp(seen_bytes, point, 12) == 0, "point snapshot");
    expect(word_at(object + 72) == static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(norrath_actors_b_view_448e30)),
           "vtable");
    if (fails != 0) return 1;
    std::puts("teleporter constructor passed");
    return 0;
}
