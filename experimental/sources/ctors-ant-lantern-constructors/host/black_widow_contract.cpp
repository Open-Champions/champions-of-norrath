#include "../detail.hpp"
#include <cstdio>
#include <cstdint>
#include <cstring>
static int fails;
static void expect(bool cond, const char* message) {
    if (!cond) { std::printf("FAIL %s\n", message); ++fails; }
}
static int stage;
static unsigned char object[1100];
static unsigned char model[160];
static unsigned char header_a[8], header_b[8];
static unsigned char chunk_model[8];
static unsigned rolls[2]; static int roll_calls;
static float seen_scale; static void* seen_map; static char** seen_args; static short seen_type;
static float params[5]; static int param_flag;
static int chunk_class; static void* chunk_arg;
static void* web_object; static int web_a, web_b, web_calls;
struct Event { int a; float fa; int b; float fb; int c; float fc; int d; float fd; int e; };
static Event events[8]; static int event_count;
static void* add_header; static int add_flag; static void* add_state; static void* init_state; static int randomize_calls;
static bool names_match;
static const char* seen_name; static const void* seen_text;
static unsigned word_at(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static unsigned half_at(const unsigned char* p) { return unsigned(p[0]) | (unsigned(p[1]) << 8); }
static unsigned bits(const void* p) { return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(p)); }
static unsigned fb(float f) { unsigned b; std::memcpy(&b, &f, 4); return b; }
unsigned char norrath_ctors_ant_lantern_view_42c188[4], norrath_ctors_ant_lantern_view_44c260[4], norrath_ctors_ant_lantern_view_46ffe0[8];
unsigned char norrath_ctors_ant_lantern_view_4813bc[4], norrath_ctors_ant_lantern_view_4813c0[4], norrath_ctors_ant_lantern_view_4813c4[4],
    norrath_ctors_ant_lantern_view_4813c8[4], norrath_ctors_ant_lantern_view_4813cc[4], norrath_ctors_ant_lantern_view_4813d0[4],
    norrath_ctors_ant_lantern_view_4813d4[4];
unsigned char norrath_actor_view_4844a4[4] = {4, 3, 2, 1};
extern "C" unsigned norrath_actor_random(void*) { expect(stage == 0, "rolls before creature"); return rolls[roll_calls++]; }
extern "C" unsigned norrath_creature_construct(void*, const void* point, short type, void* map, float scale, char** args) {
    expect(stage == 0 && roll_calls == 2 && point != nullptr, "creature after two rolls");
    stage = 1; seen_scale = scale; seen_map = map; seen_args = args; seen_type = type;
    std::memset(object + 12, 0, 4);
    std::memset(object + 160, 0, 8);
    const unsigned m = bits(model);
    std::memcpy(object + 36, &m, 4);
    const unsigned chunk = bits(chunk_model);
    std::memcpy(model + 12, &chunk, 4);
    const unsigned ha = bits(header_a), hb = bits(header_b);
    std::memcpy(model + 148, &ha, 4);
    std::memcpy(model + 116, &hb, 4);
    return 0;
}
extern "C" void __3Webii(void* o, int a, int b) { ++web_calls; web_object = o; web_a = a; web_b = b; }
extern "C" unsigned norrath_actor_set_parameters(void*, float a, float b, float c, float d, float e, int f) {
    expect(stage == 1, "parameters after creature");
    stage = 2;
    params[0] = a; params[1] = b; params[2] = c; params[3] = d; params[4] = e; param_flag = f;
    return 0;
}
extern "C" void norrath_actor_animation_initialize(void* s) { expect(stage == 2, "init after parameters"); stage = 3; init_state = s; }
extern "C" int strticmp__FPCcT0(const char* l, const char* r) { seen_name = l; seen_text = r; expect(stage == 3, "compare after init"); return names_match ? 0 : 1; }
extern "C" void norrath_actor_animation_add(void* s, void* h, int f) { expect(stage == 3, "add after compare"); stage = 4; add_state = s; add_header = h; add_flag = f; }
extern "C" void norrath_actor_animation_randomize_time(void*) { ++randomize_calls; }
extern "C" unsigned long long norrath_actor_model_active_chunks(void* m, int cls) { expect(stage == 4, "chunks after the animation"); stage = 5; chunk_arg = m; chunk_class = cls; return 0xaabbccdd11223344ull; }
extern "C" unsigned norrath_actor_add_slow_run_list(void*) { expect(stage == 5, "slow list after chunks"); stage = 6; return 0; }
extern "C" void setAnimEventFlags__8Creatureififififi(void*, int a, float fa, int b, float fb_, int c, float fc, int d, float fd, int e) {
    expect(stage == 6, "events after slow list");
    events[event_count++] = Event{a, fa, b, fb_, c, fc, d, fd, e};
}
extern "C" unsigned norrath_actor_finish_construction(void*) { expect(stage == 6 && event_count == 8, "finish last"); stage = 7; return 0xf1f1u; }
static void run(bool match) {
    stage = 0; roll_calls = 0; event_count = 0; randomize_calls = 0; web_calls = 0; names_match = match;
    rolls[0] = 0x1d; rolls[1] = 0x0e;     // 5 + 6 + 32 = 43
    std::memset(object, 0x5a, sizeof object);
    unsigned char point[12] = {0};
    char* args[] = {nullptr};
    char name[] = "queen";
    expect(norrath_construct_BlackWidow(object, point, static_cast<short>(-3), args, name) == 0xf1f1u && stage == 7, "finish result");
    expect(seen_scale == 43.0f && seen_map == norrath_ctors_ant_lantern_view_42c188 && seen_type == -3 && seen_args == args, "creature arguments");
    expect(seen_name == name && seen_text == norrath_ctors_ant_lantern_view_46ffe0, "name compare arguments");
}
int main() {
    norrath_ctors_ant_lantern::host_note(model);
    norrath_ctors_ant_lantern::host_note(header_a);
    norrath_ctors_ant_lantern::host_note(header_b);
    norrath_ctors_ant_lantern::host_note(chunk_model);
    float g[7];
    unsigned char* views[7] = {norrath_ctors_ant_lantern_view_4813bc, norrath_ctors_ant_lantern_view_4813c0, norrath_ctors_ant_lantern_view_4813c4,
        norrath_ctors_ant_lantern_view_4813c8, norrath_ctors_ant_lantern_view_4813cc, norrath_ctors_ant_lantern_view_4813d0, norrath_ctors_ant_lantern_view_4813d4};
    for (int i = 0; i < 7; ++i) { g[i] = 0.5f * static_cast<float>(i + 3); std::memcpy(views[i], &g[i], 4); }
    run(false);
    expect(word_at(object + 72) == bits(norrath_ctors_ant_lantern_view_44c260) && web_calls == 1 && web_object == object + 976 && web_a == 35 && web_b == 180, "vtable and Web");
    expect(half_at(object + 56) == 0xfffd && word_at(object + 160) == 0x1000u && word_at(object + 164) == 0x80u, "type half and flag words");
    expect(word_at(object + 176) == fb(140.0f) && word_at(object + 168) == fb(g[0]) && word_at(object + 248) == 0x01020304u && object[172] == 18 &&
           word_at(object + 28) == fb(15.0f) && word_at(object + 224) == fb(0.5f) && word_at(object + 96) == fb(g[1]), "stores");
    expect(word_at(object + 236) == 0 && half_at(object + 572) == 0 && word_at(object + 972) == 0 && word_at(object + 960) == 0, "clears");
    expect(params[0] == 1.0f && params[1] == g[2] && params[2] == 1.0f && params[3] == 1.0f && params[4] == 1.0f && param_flag == 1, "parameters");
    expect(add_header == header_b && add_flag == 1 && randomize_calls == 1 && word_at(object + 12) == 0u, "non-matching name uses model+116");
    expect(chunk_class == 0 && chunk_arg == chunk_model && word_at(object + 552) == 0x11223344u && word_at(object + 556) == 0xaabbccddu, "chunks");
    const int idx[8] = {16, 4, 6, 8, 9, 10, 11, 18};
    const int b[8] = {1, 2, 2, 4, 4, 4, 4, 1};
    const float fa[8] = {g[4], 0.5f, g[5], g[3], g[3], g[3], g[3], g[6]};
    for (int i = 0; i < 8; ++i)
        expect(events[i].a == idx[i] && events[i].fa == fa[i] && events[i].b == b[i] && events[i].fb == 0.0f && events[i].c == 0 && events[i].fc == 0.0f &&
               events[i].d == 0 && events[i].fd == 0.0f && events[i].e == 0, "event");
    run(true);
    expect(add_header == header_a && add_flag == 4 && randomize_calls == 0, "matching name uses model+148 with flag 4");
    expect(word_at(object + 12) == 1u && word_at(object + 160) == (0x1000u | 0x01200800u) && half_at(object + 572) == 16, "matching name stores");
    if (fails != 0) return 1;
    std::puts("black widow constructor passed");
    return 0;
}
