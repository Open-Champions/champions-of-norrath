#include "../detail.hpp"
#include <cstdint>
#include <cstring>
extern "C" int puts(const char*);
static int fails;
static void expect(bool cond, const char* message) {
    if (!cond) { puts(message); ++fails; }
}
static int scans;
static int stage;
extern "C" unsigned norrath_game_object_construct(void* object, const void* point, void* resource_map, char** arguments) {
    (void)point; (void)resource_map;
    auto* raw = reinterpret_cast<unsigned char*>(object);
    const auto bits = static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(arguments));
    raw[44] = static_cast<unsigned char>(bits); raw[45] = static_cast<unsigned char>(bits >> 8);
    raw[46] = static_cast<unsigned char>(bits >> 16); raw[47] = static_cast<unsigned char>(bits >> 24);
    raw[12] = 0x20; raw[13] = raw[14] = raw[15] = 0;
    return 0u;
}
extern "C" int sscanf(char* text, const char* format, int* first) {
    (void)text; (void)format;
    ++scans;
    *first = 7;
    return 1;
}
extern "C" unsigned norrath_actor_add_medium_run_list(void* object) {
    expect(stage == 0, "medium list once");
    stage = 1;
    auto* raw = reinterpret_cast<unsigned char*>(object);
    unsigned flags = unsigned(raw[12]) | (unsigned(raw[13]) << 8) | (unsigned(raw[14]) << 16) | (unsigned(raw[15]) << 24);
    expect(flags == 0x29u && raw[76] == 0 && raw[77] == 0 && raw[78] == 0 && raw[79] == 0 && raw[80] == 0,
           "clears and flags before medium list");
    expect(raw[81] == 0x5a, "byte 81 preserved");
    return 0x5151u;
}
unsigned char norrath_actors_b_view_44a420[4];
unsigned char norrath_actors_b_view_4850f8[4];
static unsigned half_at(const unsigned char* p) { return unsigned(p[0]) | (unsigned(p[1]) << 8); }
int main() {
    alignas(16) unsigned char object[96];
    unsigned char point[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    char text[] = "7";
    char* args[] = {text};
    norrath_actors_b::host_note(args);
    norrath_actors_b::host_note(text);
    std::memset(object, 0x5a, sizeof object);
    const unsigned result = norrath_construct_Shooter(object, point, 0x12345, args);
    expect(result == 0x5151u && stage == 1 && scans == 1, "forwards medium-list result");
    expect(half_at(object + 82) == 0x2345 && half_at(object + 84) == 100, "type half and constant 100");
    if (fails != 0) return 1;
    puts("shooter constructor passed");
    return 0;
}
