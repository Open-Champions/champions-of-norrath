#include "../detail.hpp"
#include <cstdio>
#include <cstdint>
#include <cstring>
static int fails;
static void expect(bool cond, const char* message) {
    if (!cond) { std::printf("FAIL %s\n", message); ++fails; }
}
static int stage;
static unsigned char object[1000];
static unsigned char model[160];
static unsigned char header[8];
static unsigned rolls[1];
static int roll_calls;
static float seen_scale; static void* seen_map; static char** seen_args; static short seen_type;
static float params[5]; static int param_flag;
struct Event { int a; float fa; int b; float fb; int c; float fc; int d; float fd; int e; };
static Event events[12]; static int event_count;
static void* add_header; static void* add_state; static void* init_state;
static unsigned word_at(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static unsigned half_at(const unsigned char* p) { return unsigned(p[0]) | (unsigned(p[1]) << 8); }
static unsigned bits(const void* p) { return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(p)); }
static unsigned fb(float f) { unsigned b; std::memcpy(&b, &f, 4); return b; }
unsigned char norrath_ctors_ant_lantern_view_42d200[4], norrath_ctors_ant_lantern_view_44f6a8[4];
unsigned char norrath_ctors_ant_lantern_view_481ab4[4], norrath_ctors_ant_lantern_view_481ab8[4], norrath_ctors_ant_lantern_view_481abc[4],
    norrath_ctors_ant_lantern_view_481ac0[4], norrath_ctors_ant_lantern_view_481ac4[4], norrath_ctors_ant_lantern_view_481ac8[4];
unsigned char norrath_actor_view_4844a4[4] = {4, 3, 2, 1};
extern "C" unsigned norrath_creature_construct(void*, const void* point, short type, void* map, float scale, char** args) {
    expect(stage == 0 && point != nullptr, "creature first");
    stage = 1; seen_scale = scale; seen_map = map; seen_args = args; seen_type = type;
    std::memset(object + 160, 0, 8);
    const unsigned m = bits(model);
    std::memcpy(object + 36, &m, 4);
    const unsigned h = bits(header);
    std::memcpy(model + 140, &h, 4);
    const unsigned base = 2000u;
    std::memcpy(object + 808, &base, 4);
    return 0;
}
extern "C" unsigned norrath_actor_set_parameters(void*, float a, float b, float c, float d, float e, int f) {
    expect(stage == 1, "parameters after creature");
    stage = 2;
    params[0] = a; params[1] = b; params[2] = c; params[3] = d; params[4] = e; param_flag = f;
    return 0;
}
extern "C" void norrath_actor_animation_initialize(void* s) { expect(stage == 2, "init after parameters"); stage = 3; init_state = s; }
extern "C" void norrath_actor_animation_add(void* s, void* h, int f) { expect(stage == 3 && f == 1, "add after init"); stage = 4; add_state = s; add_header = h; }
extern "C" unsigned norrath_actor_add_slow_run_list(void*) { expect(stage == 4, "slow list after add"); stage = 5; return 0; }
extern "C" void setAnimEventFlags__8Creatureififififi(void*, int a, float fa, int b, float fb_, int c, float fc, int d, float fd, int e) {
    expect(stage == 5, "events after slow list");
    events[event_count++] = Event{a, fa, b, fb_, c, fc, d, fd, e};
}
extern "C" unsigned norrath_actor_random(void*) { expect(stage == 5 && event_count == 11, "the roll comes after all events"); ++roll_calls; return rolls[0]; }
extern "C" unsigned norrath_actor_finish_construction(void*) {
    expect(stage == 5 && roll_calls == 1, "finish last");
    expect(word_at(object + 964) == 2000u + 7u + 300u, "+964 stored before finish");
    stage = 7; return 0xf1f1u;
}
int main() {
    norrath_ctors_ant_lantern::host_note(model);
    norrath_ctors_ant_lantern::host_note(header);
    const float g[6] = {0.5f, 1.5f, 2.5f, 3.5f, 5.5f, 6.5f};
    unsigned char* views[6] = {norrath_ctors_ant_lantern_view_481ab4, norrath_ctors_ant_lantern_view_481ab8, norrath_ctors_ant_lantern_view_481abc,
        norrath_ctors_ant_lantern_view_481ac0, norrath_ctors_ant_lantern_view_481ac4, norrath_ctors_ant_lantern_view_481ac8};
    for (int i = 0; i < 6; ++i) std::memcpy(views[i], &g[i], 4);
    rolls[0] = 607;   // 607 % 300 = 7
    std::memset(object, 0x5a, sizeof object);
    unsigned char point[12] = {0};
    char* args[] = {nullptr};
    const unsigned r = norrath_construct_Arenabeast(object, point, static_cast<short>(-3), args);
    expect(r == 0xf1f1u && stage == 7, "finish result");
    expect(seen_scale == 10.0f && seen_map == norrath_ctors_ant_lantern_view_42d200 && seen_type == -3 && seen_args == args, "creature arguments");
    expect(object[40] == 85 && object[41] == 80 && word_at(object + 160) == 0x20040000u && word_at(object + 164) == 2u, "bytes and flag words");
    expect(word_at(object + 28) == fb(50.0f) && word_at(object + 104) == fb(g[1]) && word_at(object + 96) == fb(g[2]) && word_at(object + 224) == fb(1.0f) && word_at(object + 168) == fb(g[0]), "singles");
    expect(word_at(object + 248) == 0x01020304u && word_at(object + 72) == bits(norrath_ctors_ant_lantern_view_44f6a8), "global word and vtable");
    expect(word_at(object + 176) == 0 && word_at(object + 192) == 0 && word_at(object + 236) == 0 && half_at(object + 572) == 0 && word_at(object + 256) == 0 && word_at(object + 228) == 0, "clears");
    expect(params[0] == 60.0f && params[1] == g[3] && params[2] == 2.0f && params[3] == g[4] && params[4] == 10.5f && param_flag == 1, "parameters");
    expect(init_state == object + 288 && add_state == object + 288 && add_header == header, "animation with model+140");
    expect(event_count == 11, "eleven events");
    const int idx[11] = {5, 6, 7, 8, 10, 11, 12, 9, 3, 4, 13};
    const float fa[11] = {g[0], g[0], g[0], g[0], 0.5f, g[5], 0.5f, g[0], g[0], g[0], 2.0f};
    const int b[11] = {1, 1, 1, 1, 1, 1, 1, 17, 16, 16, 16};
    for (int i = 0; i < 11; ++i)
        expect(events[i].a == idx[i] && events[i].fa == fa[i] && events[i].b == b[i] && events[i].fb == 0.0f && events[i].c == 0 && events[i].fc == 0.0f &&
               events[i].d == 0 && events[i].fd == 0.0f && events[i].e == 0, "event");
    if (fails != 0) return 1;
    std::puts("arenabeast constructor passed");
    return 0;
}
