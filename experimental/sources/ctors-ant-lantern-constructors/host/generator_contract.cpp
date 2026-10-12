#include "../detail.hpp"
#include <cstdio>
#include <cstdint>
#include <cstring>
static int fails;
static void expect(bool cond, const char* message) {
    if (!cond) { std::printf("FAIL %s\n", message); ++fails; }
}
static int stage;
static unsigned char* obj;
static const void* seen_point;
static short seen_type;
static void* seen_map;
static float seen_scale;
static char** seen_args;
static char** want_args;
static unsigned char point_copy[12];
static int tag_int_calls;
static unsigned first_int, second_int;
static char found[] = "gen";
static char* found_ptr = found;
static int compare_result;
static const char* compared_left;
static const char* compared_right;
static unsigned slow_flags;
static unsigned word_at(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static unsigned half_at(const unsigned char* p) { return unsigned(p[0]) | (unsigned(p[1]) << 8); }
extern "C" unsigned norrath_creature_construct(void* object, const void* point, short type, void* map, float scale, char** args) {
    expect(stage == 0, "creature first");
    stage = 1;
    seen_point = point; seen_type = type; seen_map = map; seen_scale = scale; seen_args = args;
    std::memcpy(point_copy, point, 12);
    unsigned char* o = reinterpret_cast<unsigned char*>(object);
    const unsigned flags = 0x20u, low160 = 1u, tags = 0x40u, junk = 0x77u;
    std::memcpy(o + 12, &flags, 4);
    std::memcpy(o + 160, &low160, 4);
    std::memcpy(o + 980, &tags, 4);
    std::memcpy(o + 984, &junk, 4);   // must be cleared
    return 0;
}
extern "C" unsigned norrath_actor_set_parameters(void* object, float a, float b, float c, float d, float e, int f) {
    expect(stage == 1, "parameters after creature");
    stage = 2;
    expect(a == 1.0f && b == 1.0f && c == 1.0f && d == 1.0f && e == 1.0f && f == 1, "parameter values");
    const unsigned char* o = reinterpret_cast<unsigned char*>(object);
    expect(o[40] == 20 && o[984] == 0 && word_at(o + 12) == 0x29u, "stores before parameters");
    expect(word_at(o + 28) == 0x41a00000u && word_at(o + 192) == 0 && word_at(o + 236) == 0 && half_at(o + 572) == 0, "singles and clears before");
    expect(word_at(o + 160) == 0x04000001u, "bit 26 set in word 160");
    expect(word_at(o + 248) == 0x01020304u, "global word copied");
    return 0;
}
extern "C" char* norrath_actor_find_tag_string(void* object, char* tag, int occurrence) {
    expect(stage == 2 && occurrence == 0 && tag == reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_46ded0), "name tag");
    stage = 3;
    (void)object;
    return found_ptr;
}
extern "C" int norrath_game_object_find_tag_int(void* object, char* tag, int occurrence) {
    (void)object;
    expect(occurrence == 0, "tag int occurrence");
    ++tag_int_calls;
    if (tag_int_calls == 1) {
        expect(stage == 3 && tag == reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_46dee0), "first int tag");
        return int(first_int);
    }
    expect(stage == 3 && tag == reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_46def0), "second int tag");
    return int(second_int);
}
extern "C" unsigned norrath_actor_add_slow_run_list(void* object) {
    expect(stage == 3 && tag_int_calls == 2, "slow list after tags");
    stage = 4;
    const unsigned char* o = reinterpret_cast<unsigned char*>(object);
    expect(word_at(o + 964) == 600 && word_at(o + 968) == 0xcafe0001u && word_at(o + 972) == 4 && word_at(o + 976) == 0 && word_at(o + 984) == 0,
           "words before slow list");
    slow_flags = word_at(o + 980);
    return 0;
}
extern "C" int norrath_factory_strcasecmp(const char* left, const char* right) {
    expect(stage == 4, "compare last");
    stage = 5;
    compared_left = left; compared_right = right;
    return compare_result;
}
unsigned char norrath_ctors_ant_lantern_view_44bbd0[4];
unsigned char norrath_ctors_ant_lantern_view_46ded0[4];
unsigned char norrath_ctors_ant_lantern_view_46dee0[4];
unsigned char norrath_ctors_ant_lantern_view_46def0[4];
unsigned char norrath_ctors_ant_lantern_view_485230[4];
unsigned char norrath_actor_view_4844a4[4] = {4, 3, 2, 1};
static unsigned run(unsigned char* object, unsigned a, unsigned b, int cmp) {
    stage = 0; tag_int_calls = 0; first_int = a; second_int = b; compare_result = cmp;
    std::memset(object, 0x5a, 1024);
    object[808] = 1; object[809] = 0; object[810] = 0xfe; object[811] = 0xca;
    unsigned char point[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    char* args[] = {nullptr};
    want_args = args;
    const unsigned r = norrath_construct_Generator(object, point, static_cast<short>(-9), args);
    expect(stage == 5, "all calls made");
    expect(seen_type == -9 && seen_map == nullptr && seen_scale == 8.0f && seen_args == want_args, "creature arguments");
    expect(seen_point != point && std::memcmp(point_copy, point, 12) == 0, "point snapshot");
    return r;
}
int main() {
    norrath_ctors_ant_lantern::host_note(found);
    alignas(16) unsigned char object[1024];
    unsigned char* o = object;
    obj = o;
    // object+968 reads object+808; make that word recognisable in the stub's check
    unsigned r = run(o, 0, 0, 5);
    expect(r == 5u, "nonzero comparison is returned");
    expect(slow_flags == 0x40u && word_at(o + 980) == 0x40u, "no tag bits set");
    expect(word_at(o + 960) == static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(found)), "name stored");
    expect(compared_left == found && compared_right == reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_485230), "compare arguments");
    expect(word_at(o + 72) == static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(norrath_ctors_ant_lantern_view_44bbd0)), "vtable");
    r = run(o, 1, 0, 0);
    expect(slow_flags == 0x41u && word_at(o + 980) == 0x43u && r == 0x43u, "first tag then equal name sets bit 2");
    r = run(o, 0, 1, 7);
    expect(slow_flags == 0x42u && word_at(o + 980) == 0x42u && r == 7u, "second tag only");
    if (fails != 0) return 1;
    std::puts("generator constructor passed");
    return 0;
}
