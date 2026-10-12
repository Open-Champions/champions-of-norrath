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
static unsigned char model[200];
static unsigned char header[8];
static unsigned char chunk_model[8];
static unsigned rolls[3];
static int roll_calls;
static float seen_scale; static void* seen_map; static char** seen_args; static short seen_type;
static float params[5]; static int param_flag;
static int chunk_class; static void* chunk_arg;
struct Event { int a; float fa; int b; float fb; int c; float fc; int d; float fd; int e; };
static Event events[8]; static int event_count;
static void* add_header; static void* add_state; static void* init_state; static void* random_state;
static unsigned word_at(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static unsigned half_at(const unsigned char* p) { return unsigned(p[0]) | (unsigned(p[1]) << 8); }
static unsigned bits(const void* p) { return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(p)); }
static unsigned fb(float f) { unsigned b; std::memcpy(&b, &f, 4); return b; }
unsigned char norrath_ctors_ant_lantern_view_42b630[4], norrath_ctors_ant_lantern_view_42b548[4], norrath_ctors_ant_lantern_view_44bae0[4];
unsigned char norrath_ctors_ant_lantern_view_480e58[4], norrath_ctors_ant_lantern_view_480e5c[4], norrath_ctors_ant_lantern_view_480e60[4],
    norrath_ctors_ant_lantern_view_480e64[4], norrath_ctors_ant_lantern_view_480e68[4], norrath_ctors_ant_lantern_view_480e6c[4],
    norrath_ctors_ant_lantern_view_480e70[4];
unsigned char norrath_actor_view_4844a4[4] = {4, 3, 2, 1};
extern "C" unsigned norrath_actor_random(void*) { expect(stage == 0, "rolls before creature"); return rolls[roll_calls++]; }
extern "C" unsigned norrath_creature_construct(void*, const void* point, short type, void* map, float scale, char** args) {
    expect(stage == 0 && roll_calls == 3 && point != nullptr, "creature after three rolls");
    stage = 1; seen_scale = scale; seen_map = map; seen_args = args; seen_type = type;
    std::memset(object + 160, 0, 8);
    const unsigned m = bits(model);
    std::memcpy(object + 36, &m, 4);
    const unsigned chunk = bits(chunk_model);
    std::memcpy(model + 12, &chunk, 4);
    const unsigned h = bits(header);
    std::memcpy(model + 172, &h, 4);
    const unsigned chunks = 9u;
    std::memcpy(object + 244, &chunks, 4);
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
extern "C" void norrath_actor_animation_randomize_time(void* s) { expect(stage == 4, "randomize after add"); stage = 5; random_state = s; }
extern "C" unsigned norrath_actor_add_slow_run_list(void*) { expect(stage == 5, "slow list after randomize"); stage = 6; return 0; }
extern "C" void setAnimEventFlags__8Creatureififififi(void*, int a, float fa, int b, float fb_, int c, float fc, int d, float fd, int e) {
    expect(stage == 6, "events after slow list");
    events[event_count++] = Event{a, fa, b, fb_, c, fc, d, fd, e};
}
extern "C" unsigned long long norrath_actor_model_active_chunks(void* m, int cls) { expect(stage == 6 && event_count == 8, "chunks after events"); stage = 7; chunk_arg = m; chunk_class = cls; return 0xaabbccdd11223344ull; }
extern "C" unsigned norrath_actor_finish_construction(void*) { expect(stage == 7, "finish last"); stage = 8; return 0xf1f1u; }
static void run(int variant) {
    stage = 0; roll_calls = 0; event_count = 0;
    std::memset(object, 0x5a, sizeof object);
    unsigned char point[12] = {0};
    char* args[] = {nullptr};
    expect(norrath_construct_Ghoul(object, point, static_cast<short>(-3), variant, args) == 0xf1f1u && stage == 8, "finish result");
    expect(seen_type == -3 && seen_args == args, "type and arguments");
}
int main() {
    norrath_ctors_ant_lantern::host_note(model);
    norrath_ctors_ant_lantern::host_note(header);
    norrath_ctors_ant_lantern::host_note(chunk_model);
    const float g[7] = {0.5f, 1.5f, 2.5f, 3.5f, 5.5f, 6.5f, 7.5f};
    unsigned char* views[7] = {norrath_ctors_ant_lantern_view_480e58, norrath_ctors_ant_lantern_view_480e5c, norrath_ctors_ant_lantern_view_480e60,
        norrath_ctors_ant_lantern_view_480e64, norrath_ctors_ant_lantern_view_480e68, norrath_ctors_ant_lantern_view_480e6c, norrath_ctors_ant_lantern_view_480e70};
    for (int i = 0; i < 7; ++i) std::memcpy(views[i], &g[i], 4);
    rolls[0] = 0x1f; rolls[1] = 0x2e; rolls[2] = 0x3d;   // 7 + 6 + 5 + 18 = 36
    run(1);
    expect(seen_scale == 36.0f && seen_map == norrath_ctors_ant_lantern_view_42b630, "scale and nonzero-variant map");
    run(0);
    expect(seen_map == norrath_ctors_ant_lantern_view_42b548, "zero-variant map");
    expect(word_at(object + 160) == 0u && word_at(object + 164) == 2u, "flag words");
    expect(word_at(object + 72) == bits(norrath_ctors_ant_lantern_view_44bae0) && word_at(object + 248) == 0x01020304u, "vtable and global");
    expect(word_at(object + 176) == fb(170.0f) && object[172] == 16 && word_at(object + 168) == fb(g[0]) && word_at(object + 28) == fb(18.0f) &&
           word_at(object + 104) == fb(g[1]) && word_at(object + 100) == fb(1.0f) && word_at(object + 96) == fb(1.0f), "stores");
    expect(word_at(object + 236) == 0 && half_at(object + 572) == 0, "clears");
    expect(params[0] == g[2] && params[1] == 1.0f && params[2] == 1.0f && params[3] == 1.0f && params[4] == 1.0f && param_flag == 1, "parameters");
    expect(init_state == object + 288 && add_state == object + 288 && random_state == object + 288 && add_header == header, "animation with model+172");
    expect(chunk_class == 9 && chunk_arg == chunk_model && word_at(object + 552) == 0x11223344u && word_at(object + 556) == 0xaabbccddu, "chunks");
    const int idx[8] = {3, 4, 5, 5, 17, 18, 19, 20};
    for (int i = 0; i < 8; ++i) expect(events[i].a == idx[i], "event index");
    for (int i = 0; i < 3; ++i) expect(events[i].fa == g[3] && events[i].b == 2 && events[i].fb == 0.0f && events[i].c == 0 && events[i].d == 0, "plain events");
    expect(events[3].fa == g[3] && events[3].b == 2 && events[3].fb == g[4] && events[3].c == 2 && events[3].fc == 0.0f && events[3].d == 0, "fourth event");
    for (int i = 4; i < 8; ++i)
        expect(events[i].fa == g[5] && events[i].b == 1 && events[i].fb == g[4] && events[i].c == 2 && events[i].fc == g[6] && events[i].d == 4 && events[i].fd == 0.0f, "late events");
    for (int i = 0; i < 8; ++i) expect(events[i].e == 0, "last integer");
    if (fails != 0) return 1;
    std::puts("ghoul constructor passed");
    return 0;
}
