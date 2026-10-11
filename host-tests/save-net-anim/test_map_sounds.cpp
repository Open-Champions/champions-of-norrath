#include "../../experimental/sources/save-net-anim-map-sounds/map_sounds.hpp"

#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

unsigned char g_arena[65536];
unsigned char* norrath_host_arena = g_arena;

namespace {
int g_failures = 0;
int g_map_calls = 0;
int g_length_calls = 0;
float g_length = 0.0f;
unsigned g_header = 0;
std::vector<std::string> g_mapped_names;
unsigned g_cursor = 256;

void expect(bool condition, const char* message) {
    if (!condition) {
        std::cerr << message << '\n';
        ++g_failures;
    }
}

unsigned read_word(unsigned at) {
    const unsigned char* p = g_arena + at;
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
void write_word(unsigned at, unsigned value) {
    unsigned char* p = g_arena + at;
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
int read_half(unsigned at) {
    const unsigned half = unsigned(g_arena[at]) | (unsigned(g_arena[at + 1]) << 8);
    return static_cast<int>(static_cast<short>(half));
}
void write_half(unsigned at, int value) {
    const unsigned half = static_cast<unsigned>(value) & 0xffffu;
    g_arena[at] = static_cast<unsigned char>(half);
    g_arena[at + 1] = static_cast<unsigned char>(half >> 8);
}
unsigned alloc(unsigned size) {
    const unsigned at = g_cursor;
    g_cursor += (size + 7u) & ~7u;
    return at;
}
unsigned intern(const char* text) {
    const unsigned at = alloc(static_cast<unsigned>(std::strlen(text) + 1u));
    std::memcpy(g_arena + at, text, std::strlen(text) + 1u);
    return at;
}
void reset() {
    std::memset(g_arena, 0, sizeof g_arena);
    g_cursor = 256;
    g_map_calls = 0;
    g_length_calls = 0;
    g_mapped_names.clear();
}
}  // namespace

extern "C" int norrath_string_compare(const char* left, const char* right) {
    return std::strcmp(left, right);
}
extern "C" int soundMapSound__FPci(const char* name, int mode) {
    expect(mode == 0, "sound map mode is zero");
    g_mapped_names.emplace_back(name);
    ++g_map_calls;
    if (g_map_calls == 1 && std::strcmp(name, "switch-list") == 0)
        write_word(g_header + 52u, read_word(g_header + 68u));
    return 40 + g_map_calls;
}
extern "C" float soundComputeSequenceLength__FP15AnimationHeader(void*) {
    ++g_length_calls;
    return g_length;
}

namespace {
void test_null_sound_list_skips_everything() {
    reset();
    g_header = alloc(80);
    g_length = 3.0f;
    norrath_archive_map_animation_sounds(g_arena + g_header);
    expect(g_map_calls == 0, "null sound list does not map sounds");
    expect(g_length_calls == 0, "null sound list does not ask for a length");
}

void test_maps_until_null_and_stores_on_reloaded_list() {
    reset();
    const unsigned first = intern("switch-list");
    const unsigned second = intern("after");
    const unsigned list_a = alloc(16);
    const unsigned list_b = alloc(16);
    write_word(list_a, first);
    write_word(list_b, second);
    g_header = alloc(80);
    const unsigned sentinel = alloc(4);
    write_word(sentinel, 0x7fffffffu);
    write_word(g_header + 52u, list_a);
    write_word(g_header + 68u, list_b);
    write_word(g_header + 56u, sentinel);
    g_arena[g_header + 61u] = 2;
    g_length = 2.5f;
    norrath_archive_map_animation_sounds(g_arena + g_header);
    expect(g_map_calls == 1, "one live sound name is mapped");
    expect(g_mapped_names.size() == 1 && g_mapped_names[0] == "switch-list",
           "the name is taken from the list before the call");
    expect(read_word(list_a + 4u) == 0u, "the old list does not receive the id");
    expect(read_word(list_b + 4u) == 41u, "the id is stored on the reloaded list");
    expect(read_word(g_header + 64u) == 150u, "2.5 seconds becomes 150 frames");
    expect(g_length_calls == 1, "length is requested once");
}

void test_names_markers_clear_and_repair() {
    reset();
    const unsigned names[] = {intern("*bodyfall*"), intern("*blank*"), intern("step")};
    const unsigned sounds = alloc(32);
    for (int i = 0; i < 3; ++i) write_word(sounds + static_cast<unsigned>(i) * 8u, names[i]);
    const unsigned events = alloc(28 * 3);
    write_half(events + 4u, 0);
    write_half(events + 28u + 4u, 1);
    write_half(events + 56u + 4u, 2);
    g_arena[events + 10u] = 9;
    g_arena[events + 28u + 14u] = 8;
    write_word(events + 56u, 0x7fffffffu);
    g_header = alloc(80);
    write_word(g_header + 52u, sounds);
    write_word(g_header + 56u, events);
    g_arena[g_header + 61u] = 0;
    g_length = 1.0f;
    norrath_archive_map_animation_sounds(g_arena + g_header);
    expect(g_map_calls == 3, "three sound names are mapped");
    expect(read_word(sounds + 4u) == 41u, "first mapped id");
    expect(read_word(sounds + 16u + 4u) == 43u, "third mapped id");
    expect(read_half(events + 4u) == 0, "bodyfall marker is repaired to zero");
    expect(read_half(events + 28u + 4u) == 0, "blank marker is repaired to zero");
    expect(g_arena[events + 10u] == 0 && g_arena[events + 28u + 14u] == 0,
           "mode zero clears event bytes 10 through 14");
    expect(read_word(g_header + 64u) == 60u, "one second becomes 60 frames");
}

void test_mode_two_keeps_markers() {
    reset();
    const unsigned sounds = alloc(24);
    write_word(sounds, intern("*bodyfall*"));
    write_word(sounds + 8u, intern("*blank*"));
    const unsigned events = alloc(28 * 3);
    write_half(events + 4u, 0);
    write_half(events + 28u + 4u, 1);
    g_arena[events + 10u] = 4;
    write_word(events + 56u, 0x7fffffffu);
    g_header = alloc(80);
    write_word(g_header + 52u, sounds);
    write_word(g_header + 56u, events);
    g_arena[g_header + 61u] = 2;
    g_length = -0.5f;
    norrath_archive_map_animation_sounds(g_arena + g_header);
    expect(read_half(events + 4u) == -3, "bodyfall stays -3 when repair is off");
    expect(read_half(events + 28u + 4u) == -4, "blank stays -4 when repair is off");
    expect(g_arena[events + 10u] == 4, "mode two does not clear event bytes");
    expect(read_word(g_header + 64u) == static_cast<unsigned>(static_cast<int>(-30.0f)),
           "negative length truncates toward zero");
}

void test_past_end_index_becomes_minus_one_without_sounds() {
    reset();
    const unsigned sounds = alloc(8);
    const unsigned events = alloc(56);
    write_half(events + 4u, 5);
    write_word(events + 28u, 0x7fffffffu);
    g_header = alloc(80);
    write_word(g_header + 52u, sounds);
    write_word(g_header + 56u, events);
    g_arena[g_header + 61u] = 1;
    g_length = 0.0f;
    norrath_archive_map_animation_sounds(g_arena + g_header);
    expect(g_map_calls == 0, "a null first name is not mapped");
    expect(read_half(events + 4u) == -1, "an unreachable index becomes -1 when no sounds exist");
}

void test_truncation_uses_the_product() {
    reset();
    const unsigned sounds = alloc(16);
    write_word(sounds, intern("only"));
    const unsigned sentinel = alloc(4);
    write_word(sentinel, 0x7fffffffu);
    g_header = alloc(80);
    write_word(g_header + 52u, sounds);
    write_word(g_header + 56u, sentinel);
    g_arena[g_header + 61u] = 9;
    g_length = 0.03f;
    norrath_archive_map_animation_sounds(g_arena + g_header);
    const float product = 0.03f * 60.0f;
    const auto expected = static_cast<unsigned>(static_cast<int>(product));
    const auto rounded = static_cast<unsigned>(static_cast<int>(product + 0.5f));
    expect(read_word(g_header + 64u) == expected, "frame count is the truncated product");
    expect(expected != rounded, "this product distinguishes truncation from rounding");
}
}  // namespace

int main() {
    test_null_sound_list_skips_everything();
    test_maps_until_null_and_stores_on_reloaded_list();
    test_names_markers_clear_and_repair();
    test_mode_two_keeps_markers();
    test_past_end_index_becomes_minus_one_without_sounds();
    test_truncation_uses_the_product();
    if (g_failures != 0) {
        std::cerr << g_failures << " failure(s)\n";
        return 1;
    }
    return 0;
}
