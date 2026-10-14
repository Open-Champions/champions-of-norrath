#include "../detail.hpp"
#include <cstdio>
#include <cstdint>
#include <cstring>
static int fails;
static void expect(bool cond, const char* message) {
    if (!cond) { std::printf("FAIL %s\n", message); ++fails; }
}
static int stage;
static int seen_flag;
static float seen_radius;
static const void* seen_point;
static unsigned char seen_bytes[12];
static void* seen_output;
static unsigned char world_body[4];
extern "C" unsigned norrath_construct_Vehicle(void* object, const void* point, short type, char** arguments, int flag) {
    (void)point; (void)type; (void)arguments;
    expect(stage == 0, "vehicle first");
    stage = 1;
    seen_flag = flag;
    auto* raw = reinterpret_cast<unsigned char*>(object);
    const unsigned x = 0x11111111u, y = 0x22222222u, z = 0x33333333u;
    std::memcpy(raw + 16, &x, 4);
    std::memcpy(raw + 20, &y, 4);
    std::memcpy(raw + 24, &z, 4);
    raw[12] = 0x11; raw[13] = 0; raw[14] = 0x10; raw[15] = 0;
    return 0u;
}
extern "C" int norrath_world_check_for_water(void* world, const void* point, float radius, float* output) {
    expect(stage == 1, "water check after vehicle");
    stage = 2;
    seen_point = point;
    seen_radius = radius;
    seen_output = output;
    std::memcpy(seen_bytes, point, 12);
    const unsigned replaced = 0x44444444u;
    std::memcpy(output, &replaced, 4);
    const auto world_bits = static_cast<std::uintptr_t>(static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(world_body)));
    expect(world == reinterpret_cast<void*>(world_bits), "world pointer");
    return 3;
}
extern "C" unsigned norrath_vehicle_reset(void* object, int value) {
    expect(stage == 2 && value == 0, "reset last");
    stage = 3;
    auto* raw = reinterpret_cast<unsigned char*>(object);
    unsigned flags = unsigned(raw[12]) | (unsigned(raw[13]) << 8) | (unsigned(raw[14]) << 16) | (unsigned(raw[15]) << 24);
    expect(flags == 0x11u, "bit 0x00100000 cleared before reset");
    expect(std::memcmp(raw + 1632, raw + 16, 8) == 0, "xy copied before reset");
    unsigned z = unsigned(raw[1640]) | (unsigned(raw[1641]) << 8) | (unsigned(raw[1642]) << 16) | (unsigned(raw[1643]) << 24);
    expect(z == 0x44444444u, "copied z includes the water-check write");
    return 0x808u;
}
unsigned char norrath_actors_b_view_453230[4];
unsigned char norrath_actors_b_view_4844a0[4];
int main() {
    const auto bits = static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(world_body));
    norrath_actors_b_view_4844a0[0] = static_cast<unsigned char>(bits);
    norrath_actors_b_view_4844a0[1] = static_cast<unsigned char>(bits >> 8);
    norrath_actors_b_view_4844a0[2] = static_cast<unsigned char>(bits >> 16);
    norrath_actors_b_view_4844a0[3] = static_cast<unsigned char>(bits >> 24);
    alignas(16) unsigned char object[1656];
    unsigned char point[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    char* args[] = {nullptr};
    std::memset(object, 0x5a, sizeof object);
    const unsigned result = norrath_construct_SkulBoat(object, point, 6, args);
    expect(result == 0x808u && stage == 3 && seen_flag == 1, "vehicle flag 1 and reset result");
    expect(seen_radius == 32.0f && seen_output == object + 24, "radius and output pointer");
    expect(seen_point != object + 16 && std::memcmp(seen_bytes, "\x11\x11\x11\x11\x22\x22\x22\x22\x33\x33\x33\x33", 12) == 0,
           "water point is a snapshot of the pre-call position");
    if (fails != 0) return 1;
    std::puts("skul boat constructor passed");
    return 0;
}
