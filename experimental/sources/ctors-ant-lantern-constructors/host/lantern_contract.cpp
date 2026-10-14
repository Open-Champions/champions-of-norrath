#include "../detail.hpp"
#include <cstdio>
#include <cstdint>
#include <cstring>
static int fails;
static void expect(bool cond, const char* message) {
    if (!cond) { std::printf("FAIL %s\n", message); ++fails; }
}
static int stage;
static void* seen_object;
static const void* seen_point;
static void* seen_map;
static char** seen_args;
static void* seen_light;
static unsigned char light_copy[48];
static unsigned word_at(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static unsigned half_at(const unsigned char* p) { return unsigned(p[0]) | (unsigned(p[1]) << 8); }
extern "C" unsigned norrath_game_object_construct(void* object, const void* point, void* map, char** args) {
    expect(stage == 0, "game object first");
    stage = 1;
    seen_object = object;
    seen_point = point;
    seen_map = map;
    seen_args = args;
    auto* raw = const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(point));
    raw[0] = 0xab;
    return 0x600du;
}
extern "C" void norrath_light_add(void* light) {
    expect(stage == 1, "light after game object");
    stage = 2;
    seen_light = light;
    std::memcpy(light_copy, light, sizeof light_copy);
}
extern "C" unsigned norrath_actor_add_run_list(void* object) {
    expect(stage == 2 && object == seen_object, "run list last");
    stage = 3;
    return 0x7117u;
}
unsigned char norrath_ctors_ant_lantern_view_44a8a0[4];
unsigned char norrath_ctors_ant_lantern_view_48198c[4] = {0x11, 0x22, 0x33, 0x44};
int main() {
    alignas(16) unsigned char object[160];
    unsigned char point[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    std::memset(object, 0x5a, sizeof object);
    const unsigned r = norrath_construct_Lantern(object, point);
    expect(r == 0x7117u && stage == 3, "run list result");
    expect(seen_point != point && seen_map == nullptr && seen_args == nullptr, "snapshot and null arguments");
    expect(seen_light == object + 76, "light inside object");
    const unsigned char* l = light_copy;
    expect(l[8] == 0 && l[9] == 0 && half_at(l + 16) == 0 && half_at(l + 24) == 0, "zeroed light fields at lightAdd");
    expect(word_at(l + 28) == 0x43700000u && word_at(l + 36) == 0x43700000u && word_at(l + 44) == 0x3f800000u, "light singles");
    expect(word_at(l + 32) == 0x44332211u && word_at(l + 40) == 0x44332211u, "tint global copied twice");
    expect(half_at(l + 10) == 200 && half_at(l + 12) == 150 && half_at(l + 14) == 50, "colour halves a");
    expect(half_at(l + 18) == 200 && half_at(l + 20) == 150 && half_at(l + 22) == 50, "colour halves b");
    expect(object[124] == 0xab && std::memcmp(object + 125, point + 1, 11) == 0, "mutated snapshot copied to 124");
    expect(word_at(object + 72) == static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(norrath_ctors_ant_lantern_view_44a8a0)), "vtable");
    expect(word_at(object + 12) == 0x5a5b5a5bu, "flag bit 0 set only");
    expect(object[85] == 1 && object[40] == 0xf0, "bytes 85 and 40");
    expect(half_at(object + 94) == 51 && half_at(object + 96) == 39 && half_at(object + 98) == 18, "halves 94 96 98");
    expect(object[148] == 0 && object[149] == 0 && half_at(object + 100) == 0, "clears");
    expect(std::memcmp(object + 136, object + 16, 12) == 0, "position copied to 136");
    expect(object[150] == 0x5a && object[152] == 0x5a && object[102] == 0x5a, "no stray stores");
    if (fails != 0) return 1;
    std::puts("lantern constructor passed");
    return 0;
}
