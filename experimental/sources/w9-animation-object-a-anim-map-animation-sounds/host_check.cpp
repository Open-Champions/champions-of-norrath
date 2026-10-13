// Host contract for animMapAnimationSounds__FP15AnimationHeader. Not an EE plan input.
// Build: c++ -std=c++17 -ffp-contract=off -DNORRATH_HOST_ARENA host_check.cpp
#define NORRATH_HOST_ARENA
#include <string>
#include <vector>
#include "../w9-animation-object-a-common/host_arena.hpp"
#include "../w9-animation-object-a-common/host_check.hpp"
static std::vector<std::string> mapped;
static std::vector<int> map_flags;
static float sequence_length;
static unsigned char* length_header;
extern "C" int soundMapSound__FPci(char* name, int flag) { mapped.push_back(name); map_flags.push_back(flag); return 1000 + (int)mapped.size(); }
extern "C" float soundComputeSequenceLength__FP15AnimationHeader(unsigned char* h) { length_header = h; return sequence_length; }
#include "anim_map_animation_sounds.cpp"
namespace {
unsigned char* const header = arena_at(0x100);
unsigned char* const names = arena_at(0x400);
unsigned char* const events = arena_at(0x800);
unsigned char* const text = arena_at(0x2000);
int failures;
void check(bool ok, const char* what) { if (!ok) { std::printf("fail: %s\n", what); ++failures; } }
// Lay out a header with the given names, events (index, word0) and kind byte.
void build(const std::vector<std::string>& n, const std::vector<std::pair<int, unsigned>>& e, unsigned kind) {
    std::memset(header, 0x44, 128);
    std::memset(events, 0x66, 28 * (e.size() + 1));
    sw_(header + 52, norrath_host_address(names));
    sw_(header + 56, norrath_host_address(events));
    header[61] = kind;
    for (size_t i = 0; i < n.size(); ++i) {
        std::memcpy(text + 32 * i, n[i].c_str(), n[i].size() + 1);
        sw_(names + 8 * i, norrath_host_address(text + 32 * i));
        sw_(names + 8 * i + 4, 0x7a7a7a7a);
    }
    sw_(names + 8 * n.size(), 0);
    for (size_t i = 0; i < e.size(); ++i) { sw_(events + 28 * i, e[i].second); sh_(events + 28 * i + 4, (unsigned)e[i].first); }
    sw_(events + 28 * e.size(), 0x7fffffffu);
    mapped.clear(); map_flags.clear(); length_header = nullptr;
}
int half(int event) { int v = events[28 * event + 4] | (events[28 * event + 5] << 8); return (short)v; }
}
int main() {
    sequence_length = 2.0f;
    // No name table: nothing happens, not even the length call.
    build({}, {{1, 5}}, 0);
    sw_(header + 52, 0);
    unsigned char before[128]; std::memcpy(before, header, 128);
    animMapAnimationSounds__FP15AnimationHeader(header);
    check(same("no table", header, before, 128) == 0 && length_header == nullptr && mapped.empty() && half(0) == 1, "zero +52 returns");

    // Names are mapped in order with flag 0 and the results stored in the second words.
    build({"footstep", "*bodyfall*", "*blank*", "other"}, {}, 2);
    animMapAnimationSounds__FP15AnimationHeader(header);
    check(mapped == std::vector<std::string>{"footstep", "*bodyfall*", "*blank*", "other"} && map_flags == std::vector<int>(4, 0), "names mapped in order");
    for (int i = 0; i < 4; ++i) check(hw(names + 8 * i + 4) == 1001u + i, "sound word stored");
    check(length_header == header && hw(header + 64) == 120u, "length times sixty stored at +64");

    // Marker rewriting (kind 2 leaves the event indexes alone afterwards).
    build({"footstep", "*bodyfall*", "*blank*"}, {{0, 1}, {1, 2}, {2, 3}, {-1, 4}, {-7, 5}, {-1, 6}}, 2);
    animMapAnimationSounds__FP15AnimationHeader(header);
    check(half(0) == 0 && half(1) == -3 && half(2) == -4 && half(3) == -1 && half(4) == -7, "bodyfall and blank markers");
    check(half(5) == -1, "negative index skipped");
    for (int i = 0; i < 6; ++i) check(hw(events + 28 * i) == (unsigned)(i + 1), "event word 0 kept");

    // Kind 0 clears bytes 10..14 of each event; neighbouring bytes stay.
    build({"footstep"}, {{0, 1}, {0, 2}}, 0);
    animMapAnimationSounds__FP15AnimationHeader(header);
    for (int e = 0; e < 2; ++e) {
        for (int b = 0; b < 28; ++b) {
            const unsigned char got = events[28 * e + b];
            const bool cleared = b >= 10 && b <= 14;
            const bool known = b < 6;   // word 0 and the halfword at +4
            if (cleared && got != 0) { std::printf("byte %d not cleared\n", b); ++failures; }
            if (!cleared && !known && got != 0x66) { std::printf("event %d byte %d disturbed\n", e, b); ++failures; }
        }
    }
    // Kind 1 does not clear but fixes indexes; kind 2 and above do neither. Every non-negative
    // index is looked up by name first, so out-of-range tests use negative values or a valid
    // string placed beyond the terminator record.
    build({"a", "b"}, {{0, 1}, {1, 2}, {-3, 3}, {-2, 4}, {-9, 5}, {-0x8000, 6}, {3, 7}}, 1);
    std::memcpy(text + 200, "zz", 3);
    sw_(names + 8 * 3, norrath_host_address(text + 200));   // names[3] lies past the terminator at names[2]
    animMapAnimationSounds__FP15AnimationHeader(header);
    check(half(0) == 0 && half(1) == 1, "valid indexes kept");
    check(half(2) == 0 && half(3) == -2 && half(4) == 0 && half(5) == 0 && half(6) == 0, "out of range replaced by 0");
    check(events[10] == 0x66, "kind 1 does not clear");
    build({"a", "b"}, {{-9, 1}, {-5, 2}}, 3);
    animMapAnimationSounds__FP15AnimationHeader(header);
    check(half(0) == -9 && half(1) == -5, "kind 3 keeps indexes");
    // Empty name table: count 0, so an out-of-range index becomes -1.
    build({}, {{-4, 1}, {-9, 2}, {-3, 3}}, 1);
    animMapAnimationSounds__FP15AnimationHeader(header);
    check(half(0) == -1 && half(1) == -1 && half(2) == -1, "empty table gives -1");
    // Boundaries: -2 is kept and -3 replaced; count-1 is kept and a larger index replaced.
    build({"a", "b", "c"}, {{-2, 1}, {-3, 2}, {2, 3}, {4, 4}}, 1);
    std::memcpy(text + 200, "zz", 3);
    sw_(names + 8 * 4, norrath_host_address(text + 200));
    animMapAnimationSounds__FP15AnimationHeader(header);
    check(half(0) == -2 && half(1) == 0 && half(2) == 2 && half(3) == 0, "boundaries");
    // Empty event table only maps names and stores the length.
    build({"a"}, {}, 0);
    sequence_length = 0.5f;
    animMapAnimationSounds__FP15AnimationHeader(header);
    check(hw(header + 64) == 30u && mapped.size() == 1, "empty events");
    // The length is truncated toward zero.
    build({"a"}, {}, 0);
    sequence_length = 1.999f;
    animMapAnimationSounds__FP15AnimationHeader(header);
    check(hw(header + 64) == 119u, "length truncated");
    if (failures) return 1;
    std::printf("animMapAnimationSounds check passed\n");
    return 0;
}
