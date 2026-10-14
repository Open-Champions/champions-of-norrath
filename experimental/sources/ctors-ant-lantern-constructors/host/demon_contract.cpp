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
static unsigned char model[96];
static unsigned char header[8];
static unsigned rolls[2];
static int roll_calls;
static int variant_calls;
static float params[5]; static int param_flag;
struct Event { int a; float fa; int b; float fb; int c; float fc; int d; float fd; int e; };
static Event events[8];
static int event_count;
static void* add_header; static void* init_state; static void* add_state;
static unsigned word_at(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static unsigned half_at(const unsigned char* p) { return unsigned(p[0]) | (unsigned(p[1]) << 8); }
static unsigned bits(const void* p) { return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(p)); }
static unsigned fb(float f) { unsigned b; std::memcpy(&b, &f, 4); return b; }
unsigned char norrath_ctors_ant_lantern_view_42af70[4];
unsigned char norrath_ctors_ant_lantern_view_44dd68[4];
unsigned char norrath_ctors_ant_lantern_view_480ba8[4];
unsigned char norrath_ctors_ant_lantern_view_480bac[4];
unsigned char norrath_ctors_ant_lantern_view_480bb0[4];
unsigned char norrath_ctors_ant_lantern_view_480bb4[4];
unsigned char norrath_actor_view_4844a4[4] = {4, 3, 2, 1};
extern "C" unsigned norrath_creature_construct(void*, const void* point, short type, void* map, float scale, char** args) {
    expect(stage == 0 && point != nullptr && type == -6 && map == norrath_ctors_ant_lantern_view_42af70 && scale == 10.0f && args == nullptr, "creature arguments");
    stage = 1;
    std::memset(object + 160, 0, 8);
    const unsigned m = bits(model);
    std::memcpy(object + 36, &m, 4);
    const unsigned h = bits(header);
    std::memcpy(model + 68, &h, 4);
    return 0;
}
extern "C" unsigned norrath_actor_random(void*) { expect(stage == 1, "random after creature"); return rolls[roll_calls++]; }
extern "C" void initAttackVariant__5Demon(void*) {
    expect(stage == 1, "variant after the random rolls");
    stage = 2; ++variant_calls;
}
extern "C" unsigned norrath_actor_set_parameters(void*, float a, float b, float c, float d, float e, int f) {
    expect(stage == 2, "parameters after the variant call");
    stage = 3;
    params[0] = a; params[1] = b; params[2] = c; params[3] = d; params[4] = e; param_flag = f;
    expect(word_at(object + 960) == 17 && word_at(object + 224) == fb(1.0f), "960 and 224 before parameters");
    return 0;
}
extern "C" void norrath_actor_animation_initialize(void* s) { expect(stage == 3, "init after parameters"); stage = 4; init_state = s; }
extern "C" void norrath_actor_animation_add(void* s, void* h, int f) { expect(stage == 4 && f == 1, "add after init"); stage = 5; add_state = s; add_header = h; }
extern "C" unsigned norrath_actor_add_slow_run_list(void*) { expect(stage == 5, "slow list after add"); stage = 6; return 0; }
extern "C" void setAnimEventFlags__8Creatureififififi(void* o, int a, float fa, int b, float fb_, int c, float fc, int d, float fd, int e) {
    expect(stage == 6 && o == object, "events after slow list");
    events[event_count++] = Event{a, fa, b, fb_, c, fc, d, fd, e};
}
extern "C" unsigned norrath_actor_finish_construction(void*) { expect(stage == 6 && event_count == 5, "finish last"); stage = 7; return 0xf1f1u; }
static void run(unsigned r0, unsigned r1) {
    stage = 0; roll_calls = 0; event_count = 0; rolls[0] = r0; rolls[1] = r1;
    std::memset(object, 0x5a, 1160);
    unsigned char point[12] = {0};
    expect(norrath_construct_Demon(object, point, static_cast<short>(-6), nullptr) == 0xf1f1u && stage == 7, "all calls, finish result");
}
int main() {
    norrath_ctors_ant_lantern::host_note(model);
    norrath_ctors_ant_lantern::host_note(header);
    const float g0 = 0.125f, g1 = 0.375f, g2 = 0.875f, g3 = 5.5f;
    std::memcpy(norrath_ctors_ant_lantern_view_480ba8, &g0, 4);
    std::memcpy(norrath_ctors_ant_lantern_view_480bac, &g1, 4);
    std::memcpy(norrath_ctors_ant_lantern_view_480bb0, &g2, 4);
    std::memcpy(norrath_ctors_ant_lantern_view_480bb4, &g3, 4);
    run(0x7fff, 0);    // 32767/65536 = 0.49998 < 0.5: second roll used
    expect(roll_calls == 2 && word_at(object + 208) == 1, "second roll stored as (r & 3) + 1");
    run(0x7fff, 6);
    expect(word_at(object + 208) == 3, "(6 & 3) + 1");
    run(0x8000, 0);    // exactly 0.5 is not below 0.5
    expect(roll_calls == 1 && word_at(object + 208) == 0x5a5a5a5au, "no second roll at 0.5");
    for (unsigned offset = 1068; offset <= 1092; offset += 8) expect(half_at(object + offset) == 0xffff, "halves");
    expect(half_at(object + 1076) == 0xffff && half_at(object + 1084) == 0xffff, "middle halves");
    expect(object[1064] == 0xff && object[1065] == 0xff, "bytes 1064 and 1065");
    for (unsigned i = 1048; i < 1064; ++i) expect(object[i] == 0, "zeroed run");
    expect(object[1047] == 0x5a && object[1066] == 0x5a && object[1067] == 0x5a, "run edges untouched");
    expect(word_at(object + 160) == 0x00040000u && word_at(object + 164) == 2u, "flag words");
    expect(word_at(object + 28) == fb(20.0f) && word_at(object + 92) == fb(-0.25f) && word_at(object + 88) == fb(1.0f) && word_at(object + 168) == fb(g0), "singles");
    expect(word_at(object + 248) == 0x01020304u && word_at(object + 72) == bits(norrath_ctors_ant_lantern_view_44dd68), "global and vtable");
    expect(word_at(object + 176) == 0 && word_at(object + 192) == 0 && word_at(object + 236) == 0 && word_at(object + 256) == 0 && word_at(object + 964) == 0 && half_at(object + 572) == 0, "clears");
    expect(params[0] == g1 && params[1] == g2 && params[2] == 1.0f && params[3] == 1.0f && params[4] == 1.0f && param_flag == 1, "parameters");
    expect(init_state == object + 288 && add_state == object + 288 && add_header == header, "animation");
    const int idx[5] = {3, 4, 5, 14, 15};
    for (int i = 0; i < 5; ++i) expect(events[i].a == idx[i] && events[i].b == 1, "event index");
    expect(events[0].fa == g3 && events[0].fb == 0.5f && events[0].c == 2 && events[0].fc == 0.0f, "event 3");
    expect(events[1].fa == g3 && events[1].fb == 0.5f && events[1].c == 2, "event 4");
    expect(events[2].fa == g3 && events[2].fb == 0.0f && events[2].c == 0, "event 5");
    expect(events[3].fa == 1.0f && events[3].fb == 0.0f && events[3].c == 0, "event 14");
    expect(events[4].fa == g3 && events[4].fb == 0.0f && events[4].c == 0, "event 15");
    for (int i = 0; i < 5; ++i) expect(events[i].d == 0 && events[i].e == 0 && events[i].fd == 0.0f, "trailing zeros");
    if (fails != 0) return 1;
    std::puts("demon constructor passed");
    return 0;
}
