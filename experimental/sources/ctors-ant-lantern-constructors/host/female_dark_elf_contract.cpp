#include "../detail.hpp"
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <initializer_list>
static int fails;
static void expect(bool cond, const char* message) {
    if (!cond) { std::printf("FAIL %s\n", message); ++fails; }
}
static int stage;
static unsigned char object[1300];
static unsigned char model[240];
static unsigned char header[8];
static unsigned char chunk_model[8];
static unsigned rolls[8]; static int roll_calls;
static int compares; static int match_at;
static float seen_scale; static void* seen_map; static char** seen_args; static short seen_type;
static int chunk_class; static void* chunk_arg;
static int init_type_calls; static int init_type_flag;
struct Event { int a; float fa; int b; float fb; int c; float fc; int d; float fd; int e; };
static Event events[12]; static int event_count;
static void* add_header; static void* add_state; static void* init_state;
static unsigned word_at(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static unsigned half_at(const unsigned char* p) { return unsigned(p[0]) | (unsigned(p[1]) << 8); }
static unsigned bits(const void* p) { return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(p)); }
static unsigned fb(float f) { unsigned b; std::memcpy(&b, &f, 4); return b; }
unsigned char norrath_ctors_ant_lantern_view_438b28[4], norrath_ctors_ant_lantern_view_450530[4], norrath_ctors_ant_lantern_view_4770d0[16], norrath_ctors_ant_lantern_view_4770e8[16];
unsigned char norrath_ctors_ant_lantern_view_482414[4], norrath_ctors_ant_lantern_view_482418[4], norrath_ctors_ant_lantern_view_48241c[4], norrath_ctors_ant_lantern_view_482420[4];
unsigned char norrath_actor_view_4844a4[4] = {4, 3, 2, 1};
extern "C" unsigned norrath_actor_random(void*) { expect(stage >= 1, "rolls after creature"); return rolls[roll_calls++]; }
extern "C" unsigned norrath_creature_construct(void*, const void* point, short type, void* map, float scale, char** args) {
    expect(stage == 0 && point != nullptr, "creature first");
    stage = 1; seen_scale = scale; seen_map = map; seen_args = args; seen_type = type;
    std::memset(object + 160, 0, 8);
    const unsigned m = bits(model);
    std::memcpy(object + 36, &m, 4);
    const unsigned chunk = bits(chunk_model);
    std::memcpy(model + 12, &chunk, 4);
    const unsigned h = bits(header);
    std::memcpy(model + 212, &h, 4);
    return 0;
}
extern "C" int strticmp__FPCcT0(const char* l, const char* r) {
    ++compares;
    const unsigned char* want = compares == 1 ? norrath_ctors_ant_lantern_view_4770d0 : norrath_ctors_ant_lantern_view_4770e8;
    expect(r == reinterpret_cast<const char*>(want) && l != nullptr, "compare order and texts");
    return compares == match_at ? 0 : 1;
}
extern "C" unsigned long long norrath_actor_model_active_chunks(void* m, int cls) { expect(stage == 1, "chunks after the rolls"); stage = 2; chunk_arg = m; chunk_class = cls; return 0xaabbccdd11223344ull; }
extern "C" void initType__13FemaleDarkElfb(void*, int flag) { expect(stage == 2, "initType after chunks"); stage = 3; ++init_type_calls; init_type_flag = flag; expect((word_at(object + 164) & 8u) == 8u, "high bit set before initType"); }
extern "C" void norrath_actor_animation_initialize(void* s) { expect(stage == 3, "init after initType"); stage = 4; init_state = s; }
extern "C" void norrath_actor_animation_add(void* s, void* h, int f) { expect(stage == 4 && f == 1, "add after init"); stage = 5; add_state = s; add_header = h; }
extern "C" unsigned norrath_actor_add_slow_run_list(void*) { expect(stage == 5, "slow list after add"); stage = 6; return 0; }
extern "C" void setAnimEventFlags__8Creatureififififi(void*, int a, float fa, int b, float fb_, int c, float fc, int d, float fd, int e) {
    expect(stage == 6, "events after slow list");
    events[event_count++] = Event{a, fa, b, fb_, c, fc, d, fd, e};
}
extern "C" unsigned norrath_actor_finish_construction(void*) { expect(stage == 6 && event_count == 10, "finish last"); stage = 7; return 0xf1f1u; }
static void run(int match, std::initializer_list<unsigned> list) {
    stage = 0; roll_calls = 0; event_count = 0; compares = 0; match_at = match; init_type_calls = 0;
    int i = 0; for (unsigned v : list) rolls[i++] = v;
    std::memset(object, 0x5a, sizeof object);
    unsigned char point[12] = {0};
    char* args[] = {nullptr};
    char name[] = "nm";
    expect(norrath_construct_FemaleDarkElf(object, point, static_cast<short>(-3), args, name) == 0xf1f1u && stage == 7, "finish result");
    expect(seen_scale == 10.0f && seen_map == norrath_ctors_ant_lantern_view_438b28 && seen_type == -3 && seen_args == args && init_type_calls == 1 && init_type_flag == 0, "creature arguments and initType");
}
int main() {
    norrath_ctors_ant_lantern::host_note(model);
    norrath_ctors_ant_lantern::host_note(header);
    norrath_ctors_ant_lantern::host_note(chunk_model);
    const float g[4] = {0.5f, 0.25f, 3.5f, 4.5f};
    unsigned char* views[4] = {norrath_ctors_ant_lantern_view_482414, norrath_ctors_ant_lantern_view_482418, norrath_ctors_ant_lantern_view_48241c, norrath_ctors_ant_lantern_view_482420};
    for (int i = 0; i < 4; ++i) std::memcpy(views[i], &g[i], 4);
    // no name match; roll bit 0 gives byte 960 = 0, so the long branch runs
    // rolls: first (low bit), then 29 -> 1 << (29 % 14 + 4) = 1 << 5, then 3, then 5, then 0x3fff (below 0.25), then 7
    run(0, {2, 29, 3, 5, 0x3fff, 7});
    expect(compares == 2 && object[960] == 0, "no match keeps the low bit");
    expect(word_at(object + 244) == (1u << (29 % 14 + 4)) && word_at(object + 964) == (3u & 2u) + 29u && word_at(object + 1104) == (5u % 3u) * 2u + 33u, "long branch words");
    expect(word_at(object + 208) == 7u % 3u + 1u && roll_calls == 6, "208 from the last roll");
    expect(chunk_class == static_cast<int>(1u << 5) && chunk_arg == chunk_model && word_at(object + 552) == 0x11223344u && word_at(object + 556) == 0xaabbccddu, "chunks");
    expect(word_at(object + 72) == bits(norrath_ctors_ant_lantern_view_450530) && object[40] == 25 && word_at(object + 168) == fb(g[0]) && word_at(object + 28) == fb(15.0f) &&
           word_at(object + 248) == 0x01020304u && object[172] == 9, "stores");
    expect(word_at(object + 164) == 8u && word_at(object + 160) == 0u && word_at(object + 176) == 0 && word_at(object + 192) == 0 && word_at(object + 236) == 0 && half_at(object + 572) == 0, "flag word and clears");
    for (unsigned offset = 1068; offset <= 1092; offset += 8) expect(half_at(object + offset) == 0xffff, "0xffff halves");
    expect(object[1064] == 0xff && object[1065] == 0xff && object[1047] == 0x5a && object[1066] == 0x5a, "marker bytes");
    for (unsigned i = 1048; i < 1064; ++i) expect(object[i] == 0, "zero run");
    expect(init_state == object + 288 && add_state == object + 288 && add_header == header, "animation with model+212");
    const int idx[10] = {18, 19, 20, 21, 4, 6, 8, 10, 13, 14};
    for (int i = 0; i < 10; ++i) expect(events[i].a == idx[i], "event index");
    for (int i = 0; i < 4; ++i) expect(events[i].fa == g[2] && events[i].b == 1, "first four events");
    for (int i = 4; i < 8; ++i) expect(events[i].fa == 1.0f && events[i].b == 16, "next four events");
    for (int i = 8; i < 10; ++i) expect(events[i].fa == g[3] && events[i].b == 1, "last two events");
    for (int i = 0; i < 10; ++i) expect(events[i].fb == 0.0f && events[i].c == 0 && events[i].fc == 0.0f && events[i].d == 0 && events[i].fd == 0.0f && events[i].e == 0, "zeros");
    // high fourth roll: no 208 and only five rolls
    run(0, {2, 29, 3, 5, 0x4000, 7});
    expect(word_at(object + 208) == 0 && roll_calls == 5, "high roll skips 208");
    // low bit set: short branch
    run(0, {1, 6});
    expect(object[960] == 1 && word_at(object + 244) == (1u << (6 & 3)) && word_at(object + 964) == 0 && word_at(object + 1104) == 0 && roll_calls == 2, "bit 0 set -> short branch");
    // name matches
    run(1, {1, 6});
    expect(compares == 1 && object[960] == 0, "first name clears byte 960");
    expect(word_at(object + 964) != 0, "first name takes the long branch");
    run(2, {0, 6});
    expect(compares == 2 && object[960] == 1 && word_at(object + 244) == (1u << (6 & 3)), "second name sets byte 960");
    if (fails != 0) return 1;
    std::puts("female dark elf constructor passed");
    return 0;
}
