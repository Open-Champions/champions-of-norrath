#include "../detail.hpp"
#include <cstdint>
#include <cstring>
extern "C" int puts(const char*);
static int fails;
static void expect(bool cond, const char* message) {
    if (!cond) { puts(message); ++fails; }
}
static int scans;
static const char* seen_format;
extern "C" unsigned norrath_game_object_construct(void* object, const void* point, void* resource_map, char** arguments) {
    (void)point; (void)resource_map;
    auto* raw = reinterpret_cast<unsigned char*>(object);
    const auto bits = static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(arguments));
    raw[44] = static_cast<unsigned char>(bits); raw[45] = static_cast<unsigned char>(bits >> 8);
    raw[46] = static_cast<unsigned char>(bits >> 16); raw[47] = static_cast<unsigned char>(bits >> 24);
    raw[12] = 0x10; raw[13] = raw[14] = raw[15] = 0;
    return 0u;
}
extern "C" int sscanf(char* text, const char* format, int* first, int* second) {
    (void)text;
    ++scans;
    seen_format = format;
    *first = 0x10001;
    *second = 0x20002;
    return 2;
}
unsigned char norrath_actors_b_view_44a4b0[4];
unsigned char norrath_actors_b_view_4850f0[8];
static unsigned word_at(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static unsigned half_at(const unsigned char* p) { return unsigned(p[0]) | (unsigned(p[1]) << 8); }
int main() {
    alignas(16) unsigned char object[96];
    unsigned char point[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    char text[] = "9,8";
    char* args[] = {text};
    norrath_actors_b::host_note(args);
    norrath_actors_b::host_note(text);
    std::memset(object, 0x5a, sizeof object);
    unsigned result = norrath_construct_MissileTrap(object, point, 0x1abcd, args);
    expect(scans == 1 && seen_format == reinterpret_cast<char*>(norrath_actors_b_view_4850f0), "sscanf format address");
    expect(result == 0x19u && word_at(object + 12) == 0x19u, "flags ored with 9 and returned");
    expect(half_at(object + 56) == 0xabcd && half_at(object + 76) == 0x0001 && half_at(object + 78) == 0x0002, "halves");
    expect(word_at(object + 72) == static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(norrath_actors_b_view_44a4b0)),
           "vtable");
    char* empty[] = {nullptr};
    norrath_actors_b::host_note(empty);
    scans = 0;
    std::memset(object, 0x5a, sizeof object);
    result = norrath_construct_MissileTrap(object, point, 0, empty);
    expect(scans == 0 && result == 0x19u && half_at(object + 76) == 0 && half_at(object + 78) == 0, "null string skips sscanf");
    if (fails != 0) return 1;
    puts("missile trap constructor passed");
    return 0;
}
