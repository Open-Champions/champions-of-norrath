// Host contract for findTagFloat__10GameObjectPci. Not an EE plan input.
// Build: c++ -std=c++17 -DNORRATH_HOST_ARENA host_check.cpp
#define NORRATH_HOST_ARENA
#include <cstdlib>
#include <string>
#include <vector>
#include "../w9-animation-object-a-common/host_arena.hpp"
#include "../w9-animation-object-a-common/host_check.hpp"
static std::vector<std::string> calls;
extern "C" unsigned norrath_factory_strlen(const char* t) { calls.push_back("strlen"); return (unsigned)std::strlen(t); }
extern "C" int norrath_factory_strncmp(const char* a, const char* b, unsigned n) {
    calls.push_back(std::string("strncmp ") + a);
    return std::strncmp(a, b, n);
}
// Mock parse: the text is a decimal number; the "double" word is its float bits widened.
extern "C" unsigned long long norrath_player_atof(const char* t) {
    calls.push_back(std::string("atof ") + t);
    float f = (float)std::atof(t);
    unsigned b; std::memcpy(&b, &f, 4);
    return 0x1111000000000000ull | b;
}
extern "C" float norrath_player_double_to_float(unsigned long long v) {
    calls.push_back("narrow");
    unsigned b = (unsigned)v; float f; std::memcpy(&f, &b, 4); return f;
}
// Upper-case letters (ctype bit 0) at table[c + 128]: 193 zeros, 26 ones for 'A'..'Z', 165 zeros.
#define Z8 0, 0, 0, 0, 0, 0, 0, 0
#define Z64 Z8, Z8, Z8, Z8, Z8, Z8, Z8, Z8
#define O8 1, 1, 1, 1, 1, 1, 1, 1
extern "C" const unsigned char norrath_factory_ctype_minimum_view[384] = {
    Z64, Z64, Z64, 0, O8, O8, O8, 1, 1, Z64, Z64, Z8, Z8, Z8, Z8, 0, 0, 0, 0, 0};
static_assert(sizeof(norrath_factory_ctype_minimum_view) == 384, "view size");
#include "find_tag_float.cpp"
int main() {
    if (norrath_factory_ctype_minimum_view['A' + 128] != 1 || norrath_factory_ctype_minimum_view['Z' + 128] != 1 ||
        norrath_factory_ctype_minimum_view['a' + 128] != 0 || norrath_factory_ctype_minimum_view['@' + 128] != 0 ||
        norrath_factory_ctype_minimum_view['[' + 128] != 0) { std::printf("table\n"); return 9; }
    unsigned char* object = arena_at(0x100);
    unsigned char* list = arena_at(0x400);
    unsigned char* text = arena_at(0x800);
    int failures = 0;
    auto put_string = [&](unsigned at, const char* s) { std::memcpy(arena_at(at), s, std::strlen(s) + 1); return norrath_host_address(arena_at(at)); };
    auto build = [&](const std::vector<const char*>& entries) {
        unsigned at = 0x1000;
        for (size_t i = 0; i < entries.size(); ++i) {
            sw_(list + 4 * i, put_string(at, entries[i]));
            at += 64;
        }
        sw_(list + 4 * entries.size(), 0);
        std::memset(object, 0x33, 64);
        sw_(object + 44, norrath_host_address(list));
    };
    auto query = [&](const char* q, int index) {
        std::strcpy(reinterpret_cast<char*>(text), q);
        calls.clear();
        return findTagFloat__10GameObjectPci(object, reinterpret_cast<char*>(text), index);
    };
    auto expect = [&](const char* what, float got, float want) {
        if (got != want) { std::printf("%s: got %f want %f\n", what, got, want); ++failures; }
    };
    build({"speed=2.5", "turn=7", "speed=9.25", "speedy=1"});
    expect("first hit", query("speed", 0), 2.5f);
    if (std::string(reinterpret_cast<char*>(text)) != "speed") { std::printf("query text\n"); ++failures; }
    expect("second hit", query("speed", 1), 9.25f);
    expect("third hit missing", query("speed", 2), 0.0f);
    expect("other key", query("turn", 0), 7.0f);
    // Query is lowercased in place, and the stored entries are matched as stored.
    expect("upper-case query", query("TURN", 0), 7.0f);
    if (std::string(reinterpret_cast<char*>(text)) != "turn") { std::printf("lowercase in place\n"); ++failures; }
    // 'speedy=1' matches the prefix 'speed' but is not followed by '=', so it is no hit.
    expect("prefix is no hit", query("speed", 2), 0.0f);
    // The call order for a first-entry hit: strlen, strncmp on the entry, atof on the text after '=', narrow.
    query("speed", 0);
    if (calls != std::vector<std::string>{"strlen", "strncmp speed=2.5", "atof 2.5", "narrow"}) { std::printf("call order\n"); ++failures; }
    // Empty list, no list, and no tag access when the list word is zero.
    build({});
    expect("empty list", query("speed", 0), 0.0f);
    sw_(object + 44, 0);
    std::strcpy(reinterpret_cast<char*>(text), "SPEED");
    calls.clear();
    expect("no list", findTagFloat__10GameObjectPci(object, reinterpret_cast<char*>(text), 0), 0.0f);
    if (!calls.empty() || std::string(reinterpret_cast<char*>(text)) != "SPEED") { std::printf("touched query without list\n"); ++failures; }
    // Negative index never hits (index is decremented first and compared with -1 only after a hit).
    build({"a=1", "a=2"});
    expect("negative index", query("a", -1), 0.0f);
    expect("index 1", query("a", 1), 2.0f);
    // The first hit that is not selected is skipped without a parse.
    query("a", 1);
    if (calls != std::vector<std::string>{"strlen", "strncmp a=1", "strncmp a=2", "atof 2", "narrow"}) { std::printf("skip order\n"); ++failures; }
    if (failures) return 1;
    std::printf("findTagFloat check passed\n");
    return 0;
}
