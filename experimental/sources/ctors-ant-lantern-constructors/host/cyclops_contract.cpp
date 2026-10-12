#include "../detail.hpp"
#include <cstdio>
#include <cstdint>
#include <cstring>
static int fails;
static void expect(bool cond, const char* message) {
    if (!cond) { std::printf("FAIL %s\n", message); ++fails; }
}
static int stage;
static unsigned char object[700];
static unsigned char model[128];
static unsigned char header[8];
static unsigned char chunk_model[8];
static unsigned rolls[2];
static int roll_calls;
static float seen_scale; static void* seen_map; static char** seen_args; static short seen_type;
static float params[5]; static int param_flag;
static void* chunk_arg; static int chunk_class;
struct Event { int a; float fa; int b; float fb; int c; float fc; int d; float fd; int e; };
static Event events[10];
static int event_count;
static void* add_header; static void* add_state; static void* init_state;
static unsigned word_at(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static unsigned half_at(const unsigned char* p) { return unsigned(p[0]) | (unsigned(p[1]) << 8); }
static unsigned bits(const void* p) { return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(p)); }
static unsigned fb(float f) { unsigned b; std::memcpy(&b, &f, 4); return b; }
unsigned char norrath_ctors_ant_lantern_view_42bb08[216 * 4];
unsigned char norrath_ctors_ant_lantern_view_44bf90[4];
unsigned char norrath_ctors_ant_lantern_view_481160[4], norrath_ctors_ant_lantern_view_481164[4], norrath_ctors_ant_lantern_view_481168[4],
    norrath_ctors_ant_lantern_view_48116c[4], norrath_ctors_ant_lantern_view_481170[4], norrath_ctors_ant_lantern_view_481174[4],
    norrath_ctors_ant_lantern_view_481178[4], norrath_ctors_ant_lantern_view_48117c[4];
unsigned char norrath_actor_view_4844a4[4] = {4, 3, 2, 1};
extern "C" unsigned norrath_actor_random(void*) {
    if (roll_calls == 0) expect(stage == 0, "first roll before creature");
    return rolls[roll_calls++];
}
extern "C" unsigned norrath_creature_construct(void*, const void* point, short type, void* map, float scale, char** args) {
    expect(stage == 0 && roll_calls == 1 && point != nullptr, "creature after the first roll");
    stage = 1; seen_scale = scale; seen_map = map; seen_args = args; seen_type = type;
    std::memset(object + 160, 0, 8);
    const unsigned m = bits(model);
    std::memcpy(object + 36, &m, 4);
    const unsigned chunk = bits(chunk_model);
    std::memcpy(model + 12, &chunk, 4);
    const unsigned h = bits(header);
    std::memcpy(model + 108, &h, 4);
    return 0;
}
extern "C" unsigned norrath_actor_set_parameters(void*, float a, float b, float c, float d, float e, int f) {
    expect(stage == 1 && roll_calls == 1, "parameters before the second roll");
    stage = 2;
    params[0] = a; params[1] = b; params[2] = c; params[3] = d; params[4] = e; param_flag = f;
    expect(object[40] == 55 && word_at(object + 28) == fb(35.0f) && word_at(object + 960) == 0, "stores before parameters");
    return 0;
}
extern "C" unsigned long long norrath_actor_model_active_chunks(void* m, int cls) {
    expect(stage == 2 && roll_calls == 2, "chunks after the second roll");
    stage = 3; chunk_arg = m; chunk_class = cls;
    return 0xaabbccdd11223344ull;
}
extern "C" void norrath_actor_animation_initialize(void* s) { expect(stage == 3, "init after chunks"); stage = 4; init_state = s; }
extern "C" void norrath_actor_animation_add(void* s, void* h, int f) { expect(stage == 4 && f == 1, "add after init"); stage = 5; add_state = s; add_header = h; }
extern "C" unsigned norrath_actor_add_slow_run_list(void*) { expect(stage == 5, "slow list after add"); stage = 6; return 0; }
extern "C" void setAnimEventFlags__8Creatureififififi(void* o, int a, float fa, int b, float fb_, int c, float fc, int d, float fd, int e) {
    expect(stage == 6 && o == object, "events after slow list");
    events[event_count++] = Event{a, fa, b, fb_, c, fc, d, fd, e};
}
extern "C" unsigned norrath_actor_finish_construction(void*) { expect(stage == 6 && event_count == 8, "finish last"); stage = 7; return 0xf1f1u; }
static void run(unsigned r0, unsigned r1, int kind) {
    stage = 0; roll_calls = 0; event_count = 0; rolls[0] = r0; rolls[1] = r1;
    std::memset(object, 0x5a, sizeof object);
    unsigned char point[12] = {0};
    char* args[] = {nullptr};
    seen_args = nullptr;
    expect(norrath_construct_Cyclops(object, point, static_cast<short>(-8), args, kind) == 0xf1f1u && stage == 7, "all calls, finish result");
    expect(seen_type == -8 && seen_args == args, "type and arguments");
}
int main() {
    norrath_ctors_ant_lantern::host_note(model);
    norrath_ctors_ant_lantern::host_note(header);
    norrath_ctors_ant_lantern::host_note(chunk_model);
    const float g[8] = {0.5f, 1.25f, 2.25f, 3.25f, 4.25f, 5.25f, 6.25f, 7.25f};
    unsigned char* views[8] = {norrath_ctors_ant_lantern_view_481160, norrath_ctors_ant_lantern_view_481164, norrath_ctors_ant_lantern_view_481168,
        norrath_ctors_ant_lantern_view_48116c, norrath_ctors_ant_lantern_view_481170, norrath_ctors_ant_lantern_view_481174,
        norrath_ctors_ant_lantern_view_481178, norrath_ctors_ant_lantern_view_48117c};
    for (int i = 0; i < 8; ++i) std::memcpy(views[i], &g[i], 4);
    run(30, 0x1f, 3);   // 30 % 14 = 2 -> scale 6.0
    expect(seen_scale == 6.0f && seen_map == norrath_ctors_ant_lantern_view_42bb08 + 216 * 3, "scale and record 3");
    expect(word_at(object + 244) == 0x1fu, "all low bits and 0x10 accumulate");
    expect(chunk_arg == chunk_model && chunk_class == 0x1f, "chunk call uses +244");
    expect(word_at(object + 552) == 0x11223344u && word_at(object + 556) == 0xaabbccddu, "64-bit result stored at 552");
    expect(word_at(object + 160) == 0x20040000u && word_at(object + 164) == 2u, "flag words");
    expect(word_at(object + 168) == fb(g[0]) && word_at(object + 248) == 0x01020304u && word_at(object + 72) == bits(norrath_ctors_ant_lantern_view_44bf90), "globals");
    expect(params[0] == g[1] && params[1] == g[2] && params[2] == g[3] && params[3] == 1.0f && params[4] == 1.0f && param_flag == 1, "parameters");
    expect(init_state == object + 288 && add_state == object + 288 && add_header == header, "animation with model+108");
    run(13, 0x0f, 0);   // 13 % 14 + 4 = 17; bit 0x10 clear: word is replaced by 0x20
    expect(seen_scale == 17.0f && seen_map == norrath_ctors_ant_lantern_view_42bb08, "scale 17, record 0");
    expect(word_at(object + 244) == 0x20u && chunk_class == 0x20, "bit 0x10 clear replaces the word with 0x20");
    run(0, 0x15, 1);    // bits 1, 4, 0x10
    expect(word_at(object + 244) == 0x15u, "selected bits only");
    const int idx[8] = {3, 4, 5, 6, 8, 9, 16, 18};
    for (int i = 0; i < 8; ++i) expect(events[i].a == idx[i], "event index");
    expect(events[0].fa == g[4] && events[0].b == 1 && events[0].c == 0, "event 3");
    expect(events[1].fa == 0.5f && events[1].fb == 1.5f && events[1].fc == 2.5f && events[1].c == 2 && events[1].d == 4, "event 4");
    expect(events[2].fa == g[4] && events[3].fa == g[4], "events 5 and 6");
    expect(events[4].fa == g[5] && events[5].fa == 0.5f, "events 8 and 9");
    expect(events[6].fa == g[6] && events[6].b == 4 && events[6].fb == g[7] && events[6].c == 8 && events[6].d == 0, "event 16");
    expect(events[7].fa == 3.5f && events[7].b == 1, "event 18");
    for (int i = 0; i < 8; ++i) expect(events[i].e == 0, "last integer zero");
    if (fails != 0) return 1;
    std::puts("cyclops constructor passed");
    return 0;
}
