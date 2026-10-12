#include "../detail.hpp"
#include <cstdio>
#include <cstdint>
#include <cstring>
static int fails;
static void expect(bool cond, const char* message) {
    if (!cond) { std::printf("FAIL %s\n", message); ++fails; }
}
static int stage;
static unsigned char object[1300];
static unsigned char model[140];
static unsigned char header[8];
static float seen_scale; static void* seen_map; static char** seen_args; static short seen_type;
static unsigned char light_copy[48]; static unsigned char point_after[12];
static int light_calls;
static float params[5]; static int param_flag;
static bool tag_present;
struct Event { int a; float fa; int b; float fb; int c; float fc; int d; float fd; int e; };
static Event events[16]; static int event_count;
static void* add_header; static void* add_state; static void* init_state;
static unsigned word_at(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static unsigned half_at(const unsigned char* p) { return unsigned(p[0]) | (unsigned(p[1]) << 8); }
static unsigned bits(const void* p) { return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(p)); }
static unsigned fb(float f) { unsigned b; std::memcpy(&b, &f, 4); return b; }
#define VIEW(name, size) unsigned char norrath_ctors_ant_lantern_view_##name[size]
VIEW(42b870, 4); VIEW(44e550, 4); VIEW(46edb0, 16); VIEW(34b6b1, 16); VIEW(485270, 16) = "same";
VIEW(480f4c, 4); VIEW(480f50, 4); VIEW(480f54, 4); VIEW(480f58, 4); VIEW(480f5c, 4); VIEW(480f60, 4); VIEW(480f64, 4); VIEW(480f68, 4); VIEW(480f6c, 4);
VIEW(480f70, 4); VIEW(480f74, 4); VIEW(480f78, 4); VIEW(480f7c, 4); VIEW(480f80, 4); VIEW(480f84, 4);
unsigned char norrath_actor_view_4844a4[4] = {4, 3, 2, 1};
extern "C" unsigned norrath_creature_construct(void*, const void* point, short type, void* map, float scale, char** args) {
    expect(stage == 0 && point != nullptr, "creature first");
    stage = 1; seen_scale = scale; seen_map = map; seen_args = args; seen_type = type;
    std::memset(object + 160, 0, 8);
    const unsigned m = bits(model);
    std::memcpy(object + 36, &m, 4);
    const unsigned h = bits(header);
    std::memcpy(model + 124, &h, 4);
    const_cast<unsigned char*>(static_cast<const unsigned char*>(point))[0] = 0xee;    // Creature mutates the snapshot
    return 0;
}
extern "C" void norrath_light_add(void* light) {
    expect(stage == 1 && light == object + 1016, "light after creature");
    stage = 2; ++light_calls;
    std::memcpy(light_copy, light, sizeof light_copy);
    std::memcpy(point_after, object + 1064, 12);
}
extern "C" int norrath_game_object_find_tag_int(void*, char* tag, int) { expect(stage == 2 && tag == reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_46edb0), "tag after the light"); return tag_present; }
extern "C" unsigned norrath_actor_set_parameters(void*, float a, float b, float c, float d, float e, int f) {
    expect(stage == 2, "parameters after the tag");
    stage = 3;
    params[0] = a; params[1] = b; params[2] = c; params[3] = d; params[4] = e; param_flag = f;
    return 0;
}
extern "C" void norrath_actor_animation_initialize(void* s) { expect(stage == 3, "init after parameters"); stage = 4; init_state = s; }
extern "C" void norrath_actor_animation_add(void* s, void* h, int f) { expect(stage == 4 && f == 1, "add after init"); stage = 5; add_state = s; add_header = h; }
extern "C" void setAnimEventFlags__8Creatureififififi(void*, int a, float fa, int b, float fb_, int c, float fc, int d, float fd, int e) {
    expect(stage == 5, "events after the header");
    events[event_count++] = Event{a, fa, b, fb_, c, fc, d, fd, e};
}
extern "C" unsigned norrath_actor_add_slow_run_list(void*) { expect(stage == 5 && event_count == 12, "slow list after events"); stage = 6; return 0; }
extern "C" unsigned norrath_actor_finish_construction(void*) { expect(stage == 6, "finish last"); stage = 7; return 0xf1f1u; }
static void run(bool tag, const char* level) {
    stage = 0; event_count = 0; light_calls = 0; tag_present = tag;
    std::memcpy(norrath_ctors_ant_lantern_view_34b6b1, level, std::strlen(level) + 1);
    std::memset(object, 0x5a, sizeof object);
    unsigned char point[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    char* args[] = {nullptr};
    expect(norrath_construct_Innoruuk(object, point, static_cast<short>(-3), args) == 0xf1f1u && stage == 7, "finish result");
    expect(seen_scale == 10.0f && seen_map == norrath_ctors_ant_lantern_view_42b870 && seen_type == -3 && seen_args == args && light_calls == 1, "creature arguments");
}
int main() {
    norrath_ctors_ant_lantern::host_note(model);
    norrath_ctors_ant_lantern::host_note(header);
    float g[22];
    unsigned char* views[22] = {norrath_ctors_ant_lantern_view_480f4c, norrath_ctors_ant_lantern_view_480f50, norrath_ctors_ant_lantern_view_480f54,
        norrath_ctors_ant_lantern_view_480f58, norrath_ctors_ant_lantern_view_480f5c, norrath_ctors_ant_lantern_view_480f60, norrath_ctors_ant_lantern_view_480f64,
        norrath_ctors_ant_lantern_view_480f68, norrath_ctors_ant_lantern_view_480f6c, norrath_ctors_ant_lantern_view_480f70, norrath_ctors_ant_lantern_view_480f74,
        norrath_ctors_ant_lantern_view_480f78, norrath_ctors_ant_lantern_view_480f7c, norrath_ctors_ant_lantern_view_480f80, norrath_ctors_ant_lantern_view_480f84,
        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
    for (int i = 0; i < 15; ++i) { g[i] = 0.125f * static_cast<float>(i + 5); std::memcpy(views[i], &g[i], 4); }
    run(false, "other");
    volatile float zero = 0.0f;
    const float quotient = 1.0f / zero;
    const unsigned char* l = light_copy;
    expect(word_at(object + 72) == bits(norrath_ctors_ant_lantern_view_44e550) && word_at(object + 988) == 0, "vtable and +988");
    expect(l[9] == 0 && l[8] == 0 && half_at(l + 18) == 115 && half_at(l + 10) == 115 && half_at(l + 20) == 15 && half_at(l + 12) == 15 &&
           half_at(l + 24) == 0xffe0 && half_at(l + 16) == 0xffe0 && half_at(l + 14) == 0 && half_at(l + 22) == 0, "light halves at lightAdd");
    expect(word_at(l + 28) == 0 && word_at(l + 36) == 0 && word_at(l + 32) == fb(quotient) && word_at(l + 40) == fb(quotient) && word_at(l + 44) == fb(g[0]), "light singles");
    expect(point_after[0] == 0xee && std::memcmp(point_after + 1, "\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c", 11) == 0, "mutated snapshot copied to +1064");
    for (unsigned offset = 1180; offset <= 1204; offset += 8) expect(half_at(object + offset) == 0xffff, "0xffff halves");
    expect(object[1176] == 0xff && object[1177] == 0xff && object[1159] == 0x5a && object[1178] == 0x5a, "marker bytes");
    for (unsigned i = 1160; i < 1176; ++i) expect(object[i] == 0, "zero run");
    expect(object[40] == 50 && object[41] == 50 && word_at(object + 164) == 0x10u && word_at(object + 160) == 0x30040400u && word_at(object + 1216) == 0xffffffffu, "bytes, flag words and -1");
    expect(word_at(object + 88) == fb(g[1]) && word_at(object + 100) == fb(g[1]) && word_at(object + 96) == fb(g[1]) && word_at(object + 104) == fb(g[2]) &&
           word_at(object + 92) == fb(g[2]) && word_at(object + 168) == fb(g[3]), "global singles");
    expect(word_at(object + 28) == fb(30.0f) && word_at(object + 224) == fb(2.0f) && word_at(object + 248) == 0x01020304u, "constants");
    expect(word_at(object + 960) == 1u && word_at(object + 984) == 0u && word_at(object + 1076) == 0u && word_at(object + 1004) == 0u && word_at(object + 980) == 0u, "word state");
    expect(params[0] == 60.0f && params[1] == g[5] && params[2] == 4.0f && params[3] == g[6] && params[4] == 10.5f && param_flag == 1, "absent-tag parameters");
    expect(init_state == object + 288 && add_state == object + 288 && add_header == header, "animation with model+124");
    const int idx[12] = {10, 11, 9, 3, 6, 7, 8, 16, 5, 17, 12, 4};
    const float fa[12] = {g[7], g[11], g[12], 0.5f, g[10], g[10], g[10], 1.0f, g[13], g[14], g[8], g[7]};
    const float fbv[12] = {g[8], g[9], g[9], 0, 0, 0, 0, 0, 0, 0, 0, 0};
    const int cc[12] = {1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i < 12; ++i)
        expect(events[i].a == idx[i] && events[i].fa == fa[i] && events[i].b == 1 && events[i].fb == fbv[i] && events[i].c == cc[i] && events[i].fc == 0.0f &&
               events[i].d == 0 && events[i].fd == 0.0f && events[i].e == 0, "event");
    run(true, "other");
    expect(params[0] == 100.0f && params[1] == 100.0f && params[2] == 4.0f && params[3] == g[4] && params[4] == 3.0f && word_at(object + 984) == 1u, "present-tag parameters");
    expect(word_at(object + 960) == 1u, "level text different");
    run(false, "same");
    expect(word_at(object + 960) == 0u, "level text equal clears +960");
    if (fails != 0) return 1;
    std::puts("innoruuk constructor passed");
    return 0;
}
