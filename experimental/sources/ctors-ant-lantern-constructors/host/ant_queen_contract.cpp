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
static unsigned rolls[2];
static int roll_calls;
static float seen_scale; static void* seen_map; static char** seen_args; static short seen_type;
static float params[5]; static int param_flag;
static int chunk_class; static void* chunk_arg;
struct Event { int a; float fa; int b; float fb; int c; float fc; int d; float fd; int e; };
static Event events[6]; static int event_count;
static void* add_header; static void* add_state; static void* init_state;
static unsigned word_at(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static unsigned half_at(const unsigned char* p) { return unsigned(p[0]) | (unsigned(p[1]) << 8); }
static unsigned bits(const void* p) { return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(p)); }
static unsigned fb(float f) { unsigned b; std::memcpy(&b, &f, 4); return b; }
unsigned char norrath_ctors_ant_lantern_view_42bf58[4], norrath_ctors_ant_lantern_view_44c440[4];
unsigned char norrath_ctors_ant_lantern_view_4812d8[4], norrath_ctors_ant_lantern_view_4812dc[4], norrath_ctors_ant_lantern_view_4812e0[4],
    norrath_ctors_ant_lantern_view_4812e4[4], norrath_ctors_ant_lantern_view_4812e8[4], norrath_ctors_ant_lantern_view_4812ec[4];
unsigned char norrath_actor_view_4844a4[4] = {4, 3, 2, 1};
extern "C" unsigned norrath_actor_random(void*) { return rolls[roll_calls++]; }
extern "C" unsigned norrath_creature_construct(void*, const void* point, short type, void* map, float scale, char** args) {
    expect(stage == 0 && roll_calls == 1 && point != nullptr, "creature after the first roll only");
    stage = 1; seen_scale = scale; seen_map = map; seen_args = args; seen_type = type;
    std::memset(object + 160, 0, 8);
    const unsigned m = bits(model);
    std::memcpy(object + 36, &m, 4);
    const unsigned chunk = bits(chunk_model);
    std::memcpy(model + 12, &chunk, 4);
    const unsigned h = bits(header);
    std::memcpy(model + 124, &h, 4);
    const unsigned base = 1000u;
    std::memcpy(object + 808, &base, 4);
    return 0;
}
extern "C" unsigned norrath_actor_set_parameters(void*, float a, float b, float c, float d, float e, int f) {
    expect(stage == 1 && roll_calls == 2, "parameters after the second roll");
    stage = 2;
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
extern "C" unsigned norrath_actor_finish_construction(void*) { expect(stage == 6 && event_count == 5, "finish last"); stage = 7; return 0xf1f1u; }
int main() {
    norrath_ctors_ant_lantern::host_note(model);
    norrath_ctors_ant_lantern::host_note(header);
    norrath_ctors_ant_lantern::host_note(chunk_model);
    const float v[6] = {0.5f, 1.5f, 2.5f, 3.5f, 5.5f, 6.5f};
    unsigned char* views[6] = {norrath_ctors_ant_lantern_view_4812d8, norrath_ctors_ant_lantern_view_4812dc, norrath_ctors_ant_lantern_view_4812e0,
        norrath_ctors_ant_lantern_view_4812e4, norrath_ctors_ant_lantern_view_4812e8, norrath_ctors_ant_lantern_view_4812ec};
    for (int i = 0; i < 6; ++i) std::memcpy(views[i], &v[i], 4);
    rolls[0] = 30; rolls[1] = 365;    // 30 % 14 + 4 = 6; 365 % 180 = 5
    std::memset(object, 0x5a, sizeof object);
    unsigned char point[12] = {0};
    char* args[] = {nullptr};
    const unsigned r = norrath_construct_AntQueen(object, point, static_cast<short>(-3), args);
    expect(r == 0xf1f1u && stage == 7, "all calls, finish result");
    expect(seen_scale == 6.0f && seen_map == norrath_ctors_ant_lantern_view_42bf58 && seen_type == -3 && seen_args == args, "creature arguments");
    expect(object[40] == 55 && object[172] == 19 && word_at(object + 176) == fb(140.0f) && word_at(object + 28) == fb(45.0f), "byte and single stores");
    expect(word_at(object + 168) == fb(v[2]) && word_at(object + 88) == fb(v[3]), "global singles");
    expect(word_at(object + 248) == 0x01020304u && word_at(object + 72) == bits(norrath_ctors_ant_lantern_view_44c440), "global word and vtable");
    expect(word_at(object + 160) == 0x20001000u && word_at(object + 164) == 0x80u, "flag words");
    expect(word_at(object + 192) == fb(0.0f) && word_at(object + 972) == 180 && word_at(object + 236) == 0 && half_at(object + 572) == 0 && word_at(object + 244) == 0, "stores");
    expect(word_at(object + 976) == 1000u + 5u + 180u, "+976 from +808, roll and 180");
    expect(params[0] == 48.0f && params[1] == v[4] && params[2] == 4.0f && params[3] == v[5] && params[4] == 10.5f && param_flag == 1, "parameters");
    expect(chunk_class == 0 && chunk_arg == chunk_model && word_at(object + 552) == 0x11223344u && word_at(object + 556) == 0xaabbccddu, "chunks");
    expect(init_state == object + 288 && add_state == object + 288 && add_header == header, "animation with model+124");
    const int idx[5] = {3, 4, 5, 6, 19};
    const float want[5] = {v[0], v[1], v[1], v[1], v[0]};
    for (int i = 0; i < 5; ++i)
        expect(events[i].a == idx[i] && events[i].fa == want[i] && events[i].b == 1 && events[i].fb == 0.0f && events[i].c == 0 && events[i].fc == 0.0f &&
               events[i].d == 0 && events[i].fd == 0.0f && events[i].e == 0, "event");
    if (fails != 0) return 1;
    std::puts("ant queen constructor passed");
    return 0;
}
