#include "../detail.hpp"
#include <cstdarg>
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <strings.h>
static int fails;
static void expect(bool cond, const char* message) {
    if (!cond) { std::printf("FAIL %s\n", message); ++fails; }
}
#define VIEW(name, size) unsigned char norrath_ctors_ant_lantern_view_##name[size]
VIEW(44ec40, 4); VIEW(470df8, 16); VIEW(485530, 16); VIEW(470e08, 16); VIEW(470e18, 16); VIEW(470e28, 16); VIEW(470e38, 16);
VIEW(470e48, 16); VIEW(485538, 16); VIEW(470e58, 16) = "%s#%d"; VIEW(485540, 16) = "%s#"; VIEW(470e68, 16); VIEW(470e78, 16);
VIEW(470e88, 16); VIEW(470e98, 16); VIEW(485480, 16) = ".v"; VIEW(485488, 16) = ".t"; VIEW(470ea8, 16); VIEW(470eb8, 16) = "special";
VIEW(485548, 16) = "keyed"; VIEW(481704, 4); VIEW(481708, 4);
unsigned char norrath_actors_b_view_4844a0[4];
static unsigned char* const tag_views[] = {
    norrath_ctors_ant_lantern_view_470df8, norrath_ctors_ant_lantern_view_485530, norrath_ctors_ant_lantern_view_470e08,
    norrath_ctors_ant_lantern_view_470e18, norrath_ctors_ant_lantern_view_470e28, norrath_ctors_ant_lantern_view_470e38,
    norrath_ctors_ant_lantern_view_470e48, norrath_ctors_ant_lantern_view_485538, norrath_ctors_ant_lantern_view_470e78,
    norrath_ctors_ant_lantern_view_470e88, norrath_ctors_ant_lantern_view_470e98, norrath_ctors_ant_lantern_view_470ea8};
static int tag_values[12];
static const char* flags_text;
static unsigned char object[512];
static unsigned char model[8];
static unsigned char world_body[4];
static unsigned char headers[8][64];
static int header_next;
static char lump_texts[8][48];
static void* lump_arg;
static int lump_calls;
static int stage_order;
static int door_calls, door_collision_calls, door_height_calls, door_result, door_height_result;
static unsigned char door_point[12];
static int animations, set_to_end_calls, randomize_calls, animation_flag;
static void* animation_header; static void* animation_state;
static int passable_calls, passable_value, state_calls, state_value;
static int chunk_class;
static unsigned word_at(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static unsigned bits(const void* p) { return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(p)); }
static unsigned fb(float f) { unsigned b; std::memcpy(&b, &f, 4); return b; }
extern "C" unsigned norrath_construct_AutoObject(void* o, char* name, const void*, short, char**) {
    std::memset(object + 12, 0, 4);
    std::strcpy(reinterpret_cast<char*>(object + 76), name);
    const unsigned m = bits(model);
    std::memcpy(object + 144, &m, 4);
    std::memset(object + 284, 0, 4);
    (void)o;
    return 0;
}
extern "C" int norrath_game_object_find_tag_int(void*, char* tag, int occurrence) {
    expect(occurrence == 0, "occurrence");
    for (int i = 0; i < 12; ++i) if (tag == reinterpret_cast<char*>(tag_views[i])) return tag_values[i];
    expect(tag == reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_470e88) || false, "known int tag");
    return 0;
}
extern "C" char* norrath_actor_find_tag_string(void*, char* tag, int) {
    expect(tag == reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_470e68), "flags tag");
    return const_cast<char*>(flags_text);
}
extern "C" int norrath_factory_sprintf(char* dst, const char* fmt, ...) {
    va_list ap; va_start(ap, fmt);
    const int n = std::vsnprintf(dst, 32, fmt, ap);
    va_end(ap);
    return n;
}
extern "C" void* norrath_lump_find_resource(char* lump, char* res) {
    lump_arg = lump;
    std::strcpy(lump_texts[lump_calls], res);
    ++lump_calls;
    return headers[header_next++ % 8];
}
extern "C" int norrath_factory_strcasecmp(const char* l, const char* r) { return ::strcasecmp(l, r); }
extern "C" int norrath_world_find_closest_door(void* world, const void* point) {
    ++door_calls;
    expect(world == world_body, "world pointer");
    std::memcpy(door_point, point, 12);
    return door_result;
}
extern "C" int norrath_world_set_door_collision(void* world, int door, int v) {
    ++door_collision_calls;
    expect(world == world_body && door == door_result && v == 1, "door collision arguments");
    return 0;
}
extern "C" int norrath_world_get_door_height(void* world, int door) {
    ++door_height_calls;
    expect(world == world_body && door == door_result, "door height arguments");
    return door_height_result;
}
extern "C" void norrath_actor_animation_add(void* s, void* h, int f) { ++animations; animation_state = s; animation_header = h; animation_flag = f; }
extern "C" void norrath_animation_set_to_end(void* s) { ++set_to_end_calls; expect(s == object + 152, "set-to-end state"); }
extern "C" void norrath_actor_animation_randomize_time(void* s) { ++randomize_calls; expect(s == object + 152, "randomize state"); }
extern "C" unsigned long long norrath_actor_model_active_chunks(void* m, int cls) {
    expect(m == model, "chunk model"); chunk_class = cls; return 0xaabbccdd11223344ull;
}
extern "C" void setPassableState__8AutoPropb(void*, int v) { ++passable_calls; passable_value = v; }
extern "C" void setState__8AutoPropi(void*, int v) { ++state_calls; state_value = v; }
extern "C" unsigned norrath_actor_add_slow_run_list(void*) { return 0; }
static void run(const char* name, const char* flags) {
    std::memset(object, 0x5a, sizeof object);
    for (auto& h : headers) std::memset(h, 0x77, sizeof h);
    flags_text = flags;
    header_next = 0; lump_calls = 0; door_calls = door_collision_calls = door_height_calls = 0;
    animations = set_to_end_calls = randomize_calls = passable_calls = state_calls = 0;
    unsigned char point[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    char* args[] = {nullptr};
    char nm[8];
    std::strcpy(nm, name);
    expect(norrath_construct_AutoProp(object, nm, point, static_cast<short>(1), args) == 0u, "returns zero");
}
int main() {
    norrath_ctors_ant_lantern::host_note(model);
    for (auto& h : headers) norrath_ctors_ant_lantern::host_note(h);
    norrath_ctors_ant_lantern::host_note(world_body);
    const unsigned wb = bits(world_body);
    std::memcpy(norrath_actors_b_view_4844a0, &wb, 4);
    const float g0 = 0.5f, g1 = 0.75f;
    std::memcpy(norrath_ctors_ant_lantern_view_481704, &g0, 4);
    std::memcpy(norrath_ctors_ant_lantern_view_481708, &g1, 4);
    // 1: no tags
    std::memset(tag_values, 0, sizeof tag_values);
    run("pr", "");
    expect(word_at(object + 396) == 0xffffffffu && word_at(object + 72) == bits(norrath_ctors_ant_lantern_view_44ec40), "-1 and vtable");
    expect(word_at(object + 376) == 0 && word_at(object + 384) == 0 && word_at(object + 388) == 0 && word_at(object + 292) == 0, "clears");
    expect(word_at(object + 284) == 0 && word_at(object + 288) == 0, "no bits, zero count");
    expect(lump_calls == 1 && std::strcmp(lump_texts[0], "pr#") == 0 && lump_arg == object + 109, "single lookup text");
    expect(word_at(object + 296) == bits(headers[0]) && animations == 1 && animation_flag == 1 && randomize_calls == 1 && set_to_end_calls == 0 && animation_header == headers[0], "plain animation");
    expect(std::strcmp(reinterpret_cast<char*>(object + 216), "pr.v") == 0 && std::strcmp(reinterpret_cast<char*>(object + 248), "pr.t") == 0, "copies with suffixes");
    expect(chunk_class == 0 && word_at(object + 200) == 0x11223344u && word_at(object + 204) == 0xaabbccddu, "chunks");
    expect(passable_calls == 1 && passable_value == 0 && state_calls == 0, "passable from bit 0 of +300");
    // 2: every bit tag
    std::memset(tag_values, 0, sizeof tag_values);
    for (int i = 0; i < 7; ++i) tag_values[i] = 1;
    tag_values[7] = 0;
    run("pr", "");
    expect(word_at(object + 284) == (0x4u | 0x8u | 0x10u | 0x20u | 0x40u | 0x80u | 0x100u), "bits");
    expect(word_at(object + 12) == 0x10000u, "tag three also sets 0x10000 in +12");
    // 3: counted lookups: count 2 -> three lookups
    std::memset(tag_values, 0, sizeof tag_values);
    tag_values[7] = 2;
    run("pr", "");
    expect(word_at(object + 288) == 2 && lump_calls == 3, "count 2 -> three lookups");
    expect(std::strcmp(lump_texts[0], "pr#0") == 0 && std::strcmp(lump_texts[2], "pr#2") == 0, "indexed names");
    expect(word_at(object + 296) == bits(headers[0]) && word_at(object + 304) == bits(headers[1]) && word_at(object + 312) == bits(headers[2]), "results at 8-byte stride");
    // 3b: negative count looks nothing up and still stores the count
    tag_values[7] = -1;
    run("pr", "");
    expect(lump_calls == 0 && word_at(object + 288) == 0xffffffffu && word_at(object + 296) == 0x5a5a5a5au, "negative count");
    // 4: flag string
    std::memset(tag_values, 0, sizeof tag_values);
    run("pr", "0110000000111");
    expect(word_at(object + 300) == 0x5a5a5a5au && word_at(object + 308) == 0x5a5a5a5bu && word_at(object + 316) == 0x5a5a5a5bu && word_at(object + 324) == 0x5a5a5a5au, "flag chars");
    expect(word_at(object + 300 + 72) == 0x5a5a5a5au, "only ten characters are scanned");
    run("pr", "1");
    expect(passable_value == 1, "passable follows bit 0 of +300");
    // 5: door path
    std::memset(tag_values, 0, sizeof tag_values);
    tag_values[8] = 1;
    door_result = 5; door_height_result = 7;
    run("pr", "");
    expect(word_at(object + 284) == 1u && door_calls == 1 && door_collision_calls == 1 && door_height_calls == 1, "door calls");
    expect(word_at(object + 396) == 5 && word_at(object + 400) == fb(7.0f), "door and height");
    expect(word_at(object + 288) == 2 && word_at(object + 304) == bits(headers[0]) && word_at(object + 308) == 1 && word_at(object + 296) == 0, "zero count becomes 2, header moved");
    expect(animations == 0 && set_to_end_calls == 0, "no animation when +296 was cleared");
    // 5b: door path with an existing count: header stays, flag 6 and set-to-end
    tag_values[7] = 1;
    run("pr", "");
    expect(word_at(object + 288) == 1 && animations == 1 && animation_flag == 6 && set_to_end_calls == 1 && animation_header == headers[0], "count kept, header animated with flag 6");
    expect(word_at(object + 308) == 0x5a5a5a5au, "no move");
    // 6: the second marker tag
    std::memset(tag_values, 0, sizeof tag_values);
    tag_values[10] = 1;
    run("pr", "");
    expect(word_at(object + 284) == 2u && word_at(object + 288) == 2 && word_at(object + 304) == bits(headers[0]) && word_at(object + 296) == 0, "marker moves header when count is zero");
    tag_values[7] = 1;
    run("pr", "");
    expect(animations == 1 && animation_flag == 6 && set_to_end_calls == 1, "marker with count animates with flag 6");
    // 7: state tag
    std::memset(tag_values, 0, sizeof tag_values);
    tag_values[11] = 3;
    run("pr", "");
    expect(state_calls == 1 && state_value == 3, "state tag forwarded");
    // 8: names
    std::memset(tag_values, 0, sizeof tag_values);
    run("special", "");
    const unsigned char* h = headers[0];
    expect(h[24] == 1 && word_at(h + 20) == fb(0.25f) && h[32] == 2 && word_at(h + 28) == fb(1.0f) && h[40] == 1 && word_at(h + 36) == fb(g0) && h[48] == 2 && word_at(h + 44) == fb(g1), "header edits");
    expect(object[40] == 0x5a, "byte 40 untouched for the first name");
    run("keyed", "");
    expect(object[40] == 0xa8, "byte 40 for the second name");
    run("pr", "");
    expect(object[40] == 0x5a, "other names leave byte 40");
    if (fails != 0) return 1;
    std::puts("auto prop constructor passed");
    return 0;
}
