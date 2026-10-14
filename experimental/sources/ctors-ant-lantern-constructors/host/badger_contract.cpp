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
static unsigned char chunk_model[8];
static float seen_scale; static void* seen_map; static char** seen_args; static short seen_type;
static float params[5]; static int param_flag; static int param_calls;
static int chunk_class; static void* chunk_arg;
struct Event { int a; float fa; int b; float fb; int c; float fc; int d; float fd; int e; };
static Event events[8]; static int event_count;
static void* add_header; static void* add_state; static void* init_state;
static unsigned word_at(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static unsigned half_at(const unsigned char* p) { return unsigned(p[0]) | (unsigned(p[1]) << 8); }
static unsigned bits(const void* p) { return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(p)); }
static unsigned fb(float f) { unsigned b; std::memcpy(&b, &f, 4); return b; }
unsigned char norrath_ctors_ant_lantern_view_42b1d0[176 * 4], norrath_ctors_ant_lantern_view_44c350[4];
unsigned char norrath_ctors_ant_lantern_view_34b6b1[16], norrath_ctors_ant_lantern_view_46dbe0[16] = "match";
unsigned char norrath_ctors_ant_lantern_view_480cc8[4], norrath_ctors_ant_lantern_view_480ccc[4], norrath_ctors_ant_lantern_view_480cd0[4],
    norrath_ctors_ant_lantern_view_480cd4[4], norrath_ctors_ant_lantern_view_480cd8[4], norrath_ctors_ant_lantern_view_480cdc[4],
    norrath_ctors_ant_lantern_view_480ce0[4], norrath_ctors_ant_lantern_view_480ce4[4], norrath_ctors_ant_lantern_view_480ce8[4],
    norrath_ctors_ant_lantern_view_480cec[4];
unsigned char norrath_actor_view_4844a4[4] = {4, 3, 2, 1};
extern "C" unsigned norrath_creature_construct(void*, const void* point, short type, void* map, float scale, char** args) {
    expect(stage == 0 && point != nullptr, "creature first");
    stage = 1; seen_scale = scale; seen_map = map; seen_args = args; seen_type = type;
    std::memset(object + 160, 0, 8);
    const unsigned m = bits(model);
    std::memcpy(object + 36, &m, 4);
    const unsigned chunk = bits(chunk_model);
    std::memcpy(model + 12, &chunk, 4);
    const unsigned h = bits(header);
    std::memcpy(model + 132, &h, 4);
    return 0;
}
extern "C" unsigned norrath_actor_set_parameters(void*, float a, float b, float c, float d, float e, int f) {
    expect(stage == 1 && param_calls == 0, "one parameters call after creature");
    ++param_calls; stage = 2;
    params[0] = a; params[1] = b; params[2] = c; params[3] = d; params[4] = e; param_flag = f;
    return 0;
}
extern "C" unsigned long long norrath_actor_model_active_chunks(void* m, int cls) { expect(stage == 2, "chunks after parameters"); stage = 3; chunk_arg = m; chunk_class = cls; return 0xaabbccdd11223344ull; }
extern "C" void norrath_actor_animation_initialize(void* s) { expect(stage == 3, "init after chunks"); stage = 4; init_state = s; }
extern "C" void norrath_actor_animation_add(void* s, void* h, int f) { expect(stage == 4 && f == 1, "add after init"); stage = 5; add_state = s; add_header = h; }
extern "C" unsigned norrath_actor_add_slow_run_list(void*) { expect(stage == 5, "slow list after add"); stage = 6; return 0; }
extern "C" void setAnimEventFlags__8Creatureififififi(void*, int a, float fa, int b, float fb_, int c, float fc, int d, float fd, int e) {
    expect(stage == 6, "events after slow list");
    events[event_count++] = Event{a, fa, b, fb_, c, fc, d, fd, e};
}
extern "C" unsigned norrath_actor_finish_construction(void*) { expect(stage == 6 && event_count == 7, "finish last"); stage = 7; return 0xf1f1u; }
static void run(int kind, const char* level) {
    stage = 0; event_count = 0; param_calls = 0;
    std::memcpy(norrath_ctors_ant_lantern_view_34b6b1, level, std::strlen(level) + 1);
    std::memset(object, 0x5a, sizeof object);
    unsigned char point[12] = {0};
    char* args[] = {nullptr};
    expect(norrath_construct_Badger(object, point, static_cast<short>(-3), args, kind) == 0xf1f1u && stage == 7, "finish result");
    expect(seen_scale == 10.0f && seen_type == -3 && seen_args == args, "creature arguments");
}
int main() {
    norrath_ctors_ant_lantern::host_note(model);
    norrath_ctors_ant_lantern::host_note(header);
    norrath_ctors_ant_lantern::host_note(chunk_model);
    float g[10];
    unsigned char* views[10] = {norrath_ctors_ant_lantern_view_480cc8, norrath_ctors_ant_lantern_view_480ccc, norrath_ctors_ant_lantern_view_480cd0,
        norrath_ctors_ant_lantern_view_480cd4, norrath_ctors_ant_lantern_view_480cd8, norrath_ctors_ant_lantern_view_480cdc,
        norrath_ctors_ant_lantern_view_480ce0, norrath_ctors_ant_lantern_view_480ce4, norrath_ctors_ant_lantern_view_480ce8, norrath_ctors_ant_lantern_view_480cec};
    for (int i = 0; i < 10; ++i) { g[i] = 0.25f * static_cast<float>(i + 3); std::memcpy(views[i], &g[i], 4); }
    run(1, "other");
    expect(seen_map == norrath_ctors_ant_lantern_view_42b1d0 + 176, "map record 1");
    expect(object[40] == 40 && word_at(object + 164) == 2u && word_at(object + 160) == 0u, "byte 40 and the high flag word");
    expect(word_at(object + 72) == bits(norrath_ctors_ant_lantern_view_44c350) && word_at(object + 28) == fb(25.0f) && word_at(object + 248) == 0x01020304u, "vtable, 25.0 and global");
    expect(object[172] == 3 && word_at(object + 176) == 0 && word_at(object + 168) == fb(1.0f) && word_at(object + 192) == 0 && word_at(object + 236) == 0 && half_at(object + 572) == 0 && object[964] == 1, "stores");
    expect(word_at(object + 92) == fb(1.0f) && word_at(object + 88) == fb(g[0]) && word_at(object + 224) == fb(g[1]) && word_at(object + 228) == fb(-10.0f), "kind 1 stores");
    expect(params[0] == 6.0f && params[1] == g[2] && params[2] == 3.0f && params[3] == g[3] && params[4] == 2.0f && param_flag == 1, "kind 1 parameters");
    expect(chunk_class == 0 && chunk_arg == chunk_model && word_at(object + 244) == 0 && word_at(object + 552) == 0x11223344u && word_at(object + 556) == 0xaabbccddu, "chunks");
    expect(init_state == object + 288 && add_state == object + 288 && add_header == header, "animation with model+132");
    const int idx[7] = {4, 5, 6, 7, 10, 11, 12};
    for (int i = 0; i < 7; ++i) expect(events[i].a == idx[i], "event index");
    for (int i = 0; i < 4; ++i) expect(events[i].fa == g[7] && events[i].b == 1 && events[i].fb == g[8] && events[i].c == 2 && events[i].fc == 0.0f && events[i].d == 0, "paired events");
    expect(events[4].fa == 0.5f && events[4].b == 1 && events[4].fb == g[9] && events[4].c == 2, "event 10");
    expect(events[5].fa == 1.0f && events[5].b == 5 && events[5].c == 0 && events[6].fa == 1.0f && events[6].b == 5 && events[6].c == 0, "events 11 and 12");
    for (int i = 0; i < 7; ++i) expect(events[i].e == 0 && events[i].fd == 0.0f, "trailing zeros");
    // other kinds: byte 40 is rewritten to 45 and -15.0 stored; the level text selects the parameters
    run(0, "match");
    expect(seen_map == norrath_ctors_ant_lantern_view_42b1d0 && object[40] == 45 && word_at(object + 228) == fb(-15.0f) && object[964] == 0, "kind 0 stores");
    expect(word_at(object + 28) == fb(35.0f) && word_at(object + 88) == 0x5a5a5a5au, "kind 0 keeps 35.0 and skips the kind 1 stores");
    expect(params[0] == 10.0f && params[1] == g[4] && params[2] == 3.0f && params[3] == 3.0f && params[4] == 4.5f, "matching level parameters");
    run(2, "different");
    expect(seen_map == norrath_ctors_ant_lantern_view_42b1d0 + 352, "record 2");
    expect(params[0] == 10.0f && params[1] == g[5] && params[2] == 2.5f && params[3] == g[6] && params[4] == 2.0f, "other level parameters");
    // the kind is compared as a signed byte: 257 behaves like 1
    run(257, "other");
    expect(object[40] == 40 && object[964] == 1 && params[0] == 6.0f, "kind compared as a byte");
    if (fails != 0) return 1;
    std::puts("badger constructor passed");
    return 0;
}
