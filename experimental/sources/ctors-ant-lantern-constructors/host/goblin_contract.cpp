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
static unsigned char model[128];
static unsigned char header[8];
static unsigned char chunk_model[8];
static unsigned rolls[8]; static int roll_calls;
static int compares; static int match_at;
static float seen_scale; static void* seen_map; static char** seen_args; static short seen_type;
static int chunk_class; static void* chunk_arg;
static int weapons_calls, init_type_calls, init_type_flag;
struct Event { int a; float fa; int b; float fb; int c; float fc; int d; float fd; int e; };
static Event events[8]; static int event_count;
static void* add_header; static void* add_state; static void* init_state;
static int lump_calls; static char* lump_name_arg[4]; static char* lump_res_arg[4];
static unsigned char resource_a[8], resource_b[8];
static char lump_text[] = "lumpname", name_a[] = "entry_a", name_b[] = "entry_b";
static unsigned word_at(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static unsigned half_at(const unsigned char* p) { return unsigned(p[0]) | (unsigned(p[1]) << 8); }
static unsigned bits(const void* p) { return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(p)); }
static unsigned fb(float f) { unsigned b; std::memcpy(&b, &f, 4); return b; }
static void put(unsigned char* p, unsigned v) { std::memcpy(p, &v, 4); }
#define VIEW(name, size) unsigned char norrath_ctors_ant_lantern_view_##name[size]
VIEW(42cef0, 4); VIEW(44b900, 4); VIEW(42cfc8, 40); VIEW(483f88, 4);
VIEW(471ed0, 16); VIEW(471ee0, 16); VIEW(471ef0, 16); VIEW(471f00, 16); VIEW(471f10, 16); VIEW(471f20, 16);
VIEW(481890, 4); VIEW(481894, 4); VIEW(481898, 4); VIEW(48189c, 4); VIEW(4818a0, 4); VIEW(4818a4, 4); VIEW(4818a8, 4);
unsigned char norrath_actor_view_4844a4[4] = {4, 3, 2, 1};
static unsigned char* const name_views[6] = {norrath_ctors_ant_lantern_view_471ed0, norrath_ctors_ant_lantern_view_471ee0, norrath_ctors_ant_lantern_view_471ef0,
    norrath_ctors_ant_lantern_view_471f00, norrath_ctors_ant_lantern_view_471f10, norrath_ctors_ant_lantern_view_471f20};
extern "C" unsigned norrath_actor_random(void*) { return rolls[roll_calls++]; }
extern "C" unsigned norrath_creature_construct(void*, const void* point, short type, void* map, float scale, char** args) {
    expect(stage == 0 && roll_calls == 1 && point != nullptr, "creature after the scale roll");
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
extern "C" void* norrath_lump_find_resource(char* lump, char* res) {
    expect(stage == 1, "lookups after creature");
    lump_name_arg[lump_calls] = lump; lump_res_arg[lump_calls] = res;
    return lump_calls++ == 0 ? static_cast<void*>(resource_a) : static_cast<void*>(resource_b);
}
extern "C" int strticmp__FPCcT0(const char* l, const char* r) {
    ++compares;
    expect(l != nullptr && r == reinterpret_cast<const char*>(name_views[compares - 1]), "compare order and texts");
    return compares == match_at ? 0 : 1;
}
extern "C" void setWeapons__6Goblin(void*) { expect(stage == 1 && roll_calls >= 2, "weapons after the variant logic"); stage = 2; ++weapons_calls; }
extern "C" void initType__6Goblinb(void*, int flag) { expect(stage == 2, "initType after weapons"); stage = 3; ++init_type_calls; init_type_flag = flag; }
extern "C" unsigned long long norrath_actor_model_active_chunks(void* m, int cls) { expect(stage == 3, "chunks after initType"); stage = 4; chunk_arg = m; chunk_class = cls; return 0xaabbccdd11223344ull; }
extern "C" void norrath_actor_animation_initialize(void* s) { expect(stage == 4, "init after chunks"); stage = 5; init_state = s; }
extern "C" void norrath_actor_animation_add(void* s, void* h, int f) { expect(stage == 5 && f == 1, "add after init"); stage = 6; add_state = s; add_header = h; }
extern "C" unsigned norrath_actor_add_slow_run_list(void*) { expect(stage == 6, "slow list after add"); stage = 7; return 0; }
extern "C" void setAnimEventFlags__8Creatureififififi(void*, int a, float fa, int b, float fb_, int c, float fc, int d, float fd, int e) {
    expect(stage == 7, "events after slow list");
    events[event_count++] = Event{a, fa, b, fb_, c, fc, d, fd, e};
}
extern "C" unsigned norrath_actor_finish_construction(void*) {
    expect(stage == 7 && event_count == 6, "finish last");
    for (unsigned i = 1000; i < 1064; ++i) expect(object[i] == 0, "64 bytes cleared at +1000 before finish");
    expect(object[999] == 0x5a && object[1064] == 0x5a, "clear range edges");
    stage = 8; return 0xf1f1u;
}
static void setup_table(unsigned stamp) {
    unsigned char* t = norrath_ctors_ant_lantern_view_42cfc8;
    std::memset(t, 0, 40);
    put(t, bits(lump_text)); put(t + 4, stamp);
    put(t + 8, bits(name_a)); put(t + 16, bits(name_b));
}
static void run(int match, std::initializer_list<unsigned> list) {
    stage = 0; roll_calls = 0; event_count = 0; compares = 0; match_at = match; weapons_calls = init_type_calls = 0;
    int i = 0; for (unsigned v : list) rolls[i++] = v;
    std::memset(object, 0x5a, sizeof object);
    unsigned char point[12] = {0};
    char* args[] = {nullptr};
    char name[] = "nm";
    expect(norrath_construct_Goblin(object, point, static_cast<short>(-3), args, name) == 0xf1f1u && stage == 8, "finish result");
    expect(seen_map == norrath_ctors_ant_lantern_view_42cef0 && seen_type == -3 && seen_args == args && weapons_calls == 1 && init_type_calls == 1 && init_type_flag == 0, "arguments and calls");
}
int main() {
    norrath_ctors_ant_lantern::host_note(model);
    norrath_ctors_ant_lantern::host_note(header);
    norrath_ctors_ant_lantern::host_note(chunk_model);
    norrath_ctors_ant_lantern::host_note(lump_text);
    norrath_ctors_ant_lantern::host_note(name_a);
    norrath_ctors_ant_lantern::host_note(name_b);
    const float g[9] = {0.5f, 0.25f, 0.75f, 0.125f, 2.5f, 3.5f, 4.5f, 5.5f, 6.5f};
    unsigned char* views[8] = {norrath_ctors_ant_lantern_view_481890, norrath_ctors_ant_lantern_view_481894, norrath_ctors_ant_lantern_view_481898,
        norrath_ctors_ant_lantern_view_48189c, norrath_ctors_ant_lantern_view_4818a0, norrath_ctors_ant_lantern_view_4818a4, norrath_ctors_ant_lantern_view_4818a8, nullptr};
    for (int i = 0; i < 7; ++i) std::memcpy(views[i], &g[i], 4);
    // cache refresh
    put(norrath_ctors_ant_lantern_view_483f88, 7);
    setup_table(3);
    lump_calls = 0;
    run(0, {30, 1, 0});     // scale (30 % 14) + 4 = 6; variant roll 1 % 5 = 1 ... name no match
    expect(seen_scale == 6.0f, "scale");
    expect(lump_calls == 2 && lump_name_arg[0] == lump_text && lump_res_arg[0] == name_a && lump_name_arg[1] == lump_text && lump_res_arg[1] == name_b, "refresh looks up each pair");
    expect(word_at(norrath_ctors_ant_lantern_view_42cfc8 + 12) == bits(resource_a) && word_at(norrath_ctors_ant_lantern_view_42cfc8 + 20) == bits(resource_b) &&
           word_at(norrath_ctors_ant_lantern_view_42cfc8 + 4) == 7u, "resources and the stamp stored");
    lump_calls = 0;
    run(0, {30, 1, 0});
    expect(lump_calls == 0, "same stamp skips the refresh");
    // empty list: the old global is stored, nothing looked up
    std::memset(norrath_ctors_ant_lantern_view_42cfc8, 0, 40);
    put(norrath_ctors_ant_lantern_view_42cfc8, bits(lump_text));
    put(norrath_ctors_ant_lantern_view_483f88, 9);
    lump_calls = 0;
    run(0, {30, 1, 0});
    expect(lump_calls == 0 && word_at(norrath_ctors_ant_lantern_view_42cfc8 + 4) == 9u, "empty list stores the stamp");
    // fixed stores (from the last run)
    expect(object[40] == 40 && word_at(object + 160) == 0x40000u && word_at(object + 176) == fb(60.0f) && word_at(object + 168) == fb(g[0]) && word_at(object + 28) == fb(18.0f) &&
           word_at(object + 248) == 0x01020304u && word_at(object + 72) == bits(norrath_ctors_ant_lantern_view_44b900), "fixed stores");
    expect(word_at(object + 984) == 0 && word_at(object + 236) == 0 && half_at(object + 572) == 0 && word_at(object + 976) == 0 && word_at(object + 1136) == 0 && word_at(object + 1140) == 0, "clears");
    for (unsigned offset = 1100; offset <= 1124; offset += 8) expect(half_at(object + offset) == 0xffff, "0xffff halves");
    expect(object[1096] == 0xff && object[1097] == 0xff && object[1079] == 0x5a && object[1098] == 0x5a, "marker bytes");
    for (unsigned i = 1080; i < 1096; ++i) expect(object[i] == 0, "zero run");
    expect(chunk_class == static_cast<int>(word_at(object + 244)) && chunk_arg == chunk_model && word_at(object + 552) == 0x11223344u && word_at(object + 556) == 0xaabbccddu, "chunks use +244");
    expect(init_state == object + 288 && add_state == object + 288 && add_header == header, "animation with model+108");
    const int idx[6] = {3, 4, 6, 16, 8, 8};
    for (int i = 0; i < 6; ++i) expect(events[i].a == idx[i], "event index");
    expect(events[0].fa == g[4] && events[0].b == 1 && events[0].c == 0, "event 3");
    expect(events[1].fa == g[4] && events[1].b == 2 && events[1].fb == g[5] && events[1].c == 2 && events[1].fc == 1.0f && events[1].d == 1, "event 4");
    expect(events[2].fa == g[5] && events[2].b == 1 && events[2].fb == g[6] && events[2].c == 2, "event 6");
    expect(events[3].fa == 1.5f && events[4].fa == 1.0f && events[4].c == 0 && events[5].fa == 1.0f && events[5].fb == 1.0f && events[5].c == 2, "events 16 and 8");
    for (int i = 0; i < 6; ++i) expect(events[i].fd == 0.0f && events[i].e == 0, "trailing zeros");
    // variant from the roll: 3 -> 0, others stay; names override in order
    run(0, {0, 3, 0});
    expect(word_at(object + 972) == 0 && compares == 6, "roll value 3 becomes variant 0");
    run(0, {0, 4, 0, 0, 0});
    expect(word_at(object + 972) == 4, "roll value 4 stays");
    const unsigned name_variant[6] = {0, 1, 3, 2, 4, 5};
    for (int n = 1; n <= 6; ++n) {
        run(n, {0, 1, 0, 0, 0, 0});
        expect(compares == n && word_at(object + 972) == name_variant[n - 1], "name match sets the variant");
    }
    // variant 0: bit 1 of the roll picks a shifted bit using one more roll; bit 2 sets 2
    run(6 + 1, {0, 0, 3, 7});                 // roll 3: bits 1 and 2 -> 1 << (7 % 3 + 5) | 2
    expect(word_at(object + 244) == ((1u << (7 % 3 + 5)) | 2u) && word_at(object + 992) == 0 && word_at(object + 208) == 0, "variant 0");
    run(7, {0, 0, 2});
    expect(word_at(object + 244) == 2u, "variant 0, bit 2 only");
    // variant 1: roll bits 1, 2, 4 -> 992 = 4, 244 = (1 << (x % 3 + 5)) | 6 | 8; high threshold roll skips 208
    run(2, {0, 0, 7, 4, 0x8000});
    expect(word_at(object + 972) == 1u && word_at(object + 992) == 4u, "variant 1 sets 992");
    expect(word_at(object + 244) == ((1u << (4 % 3 + 5)) | 6u | 8u) && word_at(object + 208) == 0, "variant 1 words");
    run(2, {0, 0, 1, 4, 0x3fff, 5});
    expect(word_at(object + 244) == ((1u << (4 % 3 + 5)) | 2u) && word_at(object + 208) == (5u & 1u) + 1u && word_at(object + 992) == 4u, "variant 1 with the low bit and a low roll");
    run(2, {0, 0, 2, 4, 0x8000});
    expect(word_at(object + 244) == ((1u << (4 % 3 + 5)) | 6u) && word_at(object + 992) == 0, "variant 1 bit 2 only");
    // variants 2 and 3
    for (int n : {4, 3}) {
        run(n, {0, 0, 0, 5, 0x3fff, 6});
        expect(word_at(object + 992) == 4u && word_at(object + 244) == ((1u << ((5 & 1) + 8)) | 0x1fu) && word_at(object + 208) == (6u & 3u) + 1u, "variants 2 and 3");
    }
    // variant 4
    run(5, {0, 0, 0, 0x1fff, 7});
    expect(word_at(object + 244) == 0x4000u && word_at(object + 208) == 7u % 3u + 1u, "variant 4 low roll");
    run(5, {0, 0, 0, 0x2000, 7});
    expect(word_at(object + 244) == 0x4000u && word_at(object + 208) == 0, "variant 4 high roll");
    // variant 5
    run(6, {0, 0, 0});
    expect(word_at(object + 244) == 0x10002u, "variant 5");
    if (fails != 0) return 1;
    std::puts("goblin constructor passed");
    return 0;
}
