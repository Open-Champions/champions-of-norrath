#include "../detail.hpp"
#include <cstdio>
#include <cstdint>
#include <cstring>
static int fails;
static void expect(bool cond, const char* message) {
    if (!cond) { std::printf("FAIL %s\n", message); ++fails; }
}
static int stage;
static unsigned char object[1400];
static unsigned char model[140];
static unsigned char header[8];
static unsigned char chunk_model[8];
static unsigned rolls[4]; static int roll_calls;
static bool king;
static float seen_scale; static void* seen_map; static char** seen_args; static short seen_type;
static float params[5]; static int param_flag; static int param_calls;
static int chunk_class; static void* chunk_arg;
struct Event { int a; float fa; int b; float fb; int c; float fc; int d; float fd; int e; };
static Event events[12]; static int event_count;
static void* add_header; static void* add_state; static void* init_state;
static unsigned word_at(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static unsigned half_at(const unsigned char* p) { return unsigned(p[0]) | (unsigned(p[1]) << 8); }
static unsigned bits(const void* p) { return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(p)); }
static unsigned fb(float f) { unsigned b; std::memcpy(&b, &f, 4); return b; }
#define VIEW(name, size) unsigned char norrath_ctors_ant_lantern_view_##name[size]
VIEW(42ae60, 4); VIEW(44dbe8, 4); VIEW(46d300, 16);
VIEW(480b20, 4); VIEW(480b24, 4); VIEW(480b28, 4); VIEW(480b2c, 4); VIEW(480b30, 4); VIEW(480b34, 4); VIEW(480b38, 4); VIEW(480b3c, 4); VIEW(480b40, 4);
VIEW(480b44, 4); VIEW(480b48, 4); VIEW(480b4c, 4);
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
extern "C" int strticmp__FPCcT0(const char* l, const char* r) { expect(stage == 1 && r == reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_46d300) && l != nullptr, "name compare"); return king ? 0 : 1; }
extern "C" unsigned norrath_actor_random(void*) { return rolls[roll_calls++]; }
extern "C" unsigned norrath_actor_set_parameters(void*, float a, float b, float c, float d, float e, int f) {
    expect(stage == 1 && param_calls == 0, "one parameters call");
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
extern "C" unsigned norrath_actor_finish_construction(void*) { expect(stage == 6 && event_count == 11, "finish last"); stage = 7; return 0xf1f1u; }
static void run(bool is_king, unsigned r0, unsigned r1) {
    stage = 0; roll_calls = 0; event_count = 0; param_calls = 0; king = is_king;
    rolls[0] = r0; rolls[1] = r1;
    std::memset(object, 0x5a, sizeof object);
    unsigned char point[12] = {0};
    char* args[] = {nullptr};
    char name[] = "nm";
    expect(norrath_construct_CloudGiant(object, point, static_cast<short>(-3), args, name) == 0xf1f1u && stage == 7, "finish result");
    expect(seen_scale == 10.0f && seen_map == norrath_ctors_ant_lantern_view_42ae60 && seen_type == -3 && seen_args == args, "creature arguments");
}
int main() {
    norrath_ctors_ant_lantern::host_note(model);
    norrath_ctors_ant_lantern::host_note(header);
    norrath_ctors_ant_lantern::host_note(chunk_model);
    float gv[13];
    unsigned char* views[13] = {norrath_ctors_ant_lantern_view_480b20, norrath_ctors_ant_lantern_view_480b24, norrath_ctors_ant_lantern_view_480b28,
        norrath_ctors_ant_lantern_view_480b2c, norrath_ctors_ant_lantern_view_480b30, norrath_ctors_ant_lantern_view_480b34, norrath_ctors_ant_lantern_view_480b38,
        norrath_ctors_ant_lantern_view_480b3c, norrath_ctors_ant_lantern_view_480b40, norrath_ctors_ant_lantern_view_480b44, norrath_ctors_ant_lantern_view_480b48,
        norrath_ctors_ant_lantern_view_480b4c, nullptr};
    for (int i = 0; i < 12; ++i) { gv[i] = 0.125f * static_cast<float>(i + 3); std::memcpy(views[i], &gv[i], 4); }
    gv[4] = 0.5f; std::memcpy(norrath_ctors_ant_lantern_view_480b30, &gv[4], 4);     // threshold
    // not the king: roll gives +244 = 1 << (roll % 3); the threshold roll 0x7fff is below 0.5
    run(false, 5, 0x7fff);
    expect(word_at(object + 244) == (1u << (5 % 3)) && chunk_class == (1 << 2) && chunk_arg == chunk_model, "variant bit");
    expect(params[0] == 6.0f && params[1] == 2.0f && params[2] == gv[3] && params[3] == 1.0f && params[4] == 1.0f && param_flag == 1, "plain parameters");
    expect(word_at(object + 208) == 1u && word_at(object + 976) == 0x5a5a5a5au && word_at(object + 788) == 0x5a5a5a5au, "208 from the threshold roll only");
    expect(word_at(object + 72) == bits(norrath_ctors_ant_lantern_view_44dbe8) && object[40] == 75 && object[41] == 50 && word_at(object + 160) == 0x40000u && word_at(object + 164) == 2u, "fixed stores");
    expect(word_at(object + 248) == 0x01020304u && word_at(object + 168) == fb(gv[0]) && word_at(object + 28) == fb(35.0f) && word_at(object + 104) == fb(gv[1]) && word_at(object + 1308) == 0xffffffffu, "singles");
    expect(word_at(object + 176) == 0 && word_at(object + 192) == 0 && word_at(object + 236) == 0 && half_at(object + 572) == 0 && word_at(object + 256) == 0 &&
           word_at(object + 984) == 1u && word_at(object + 1304) == 0, "clears");
    for (unsigned offset = 1092; offset <= 1116; offset += 8) expect(half_at(object + offset) == 0xffff, "first block halves");
    expect(object[1088] == 0xff && object[1089] == 0xff && object[1071] == 0x5a && object[1090] == 0x5a, "first block markers");
    for (unsigned i = 1072; i < 1088; ++i) expect(object[i] == 0, "first zero run");
    for (unsigned offset = 1228; offset <= 1252; offset += 8) expect(half_at(object + offset) == 0xffff || offset == 1228, "second block halves");
    expect(object[1224] == 0xff && object[1225] == 0xff && object[1207] == 0x5a && object[1226] == 0x5a, "second block markers");
    for (unsigned i = 1208; i < 1224; ++i) expect(object[i] == 0, "second zero run");
    expect(word_at(object + 224) == fb(2.0f) && init_state == object + 288 && add_state == object + 288 && add_header == header, "224 and animation with model+132");
    expect(word_at(object + 552) == 0x11223344u && word_at(object + 556) == 0xaabbccddu, "chunks");
    const int idx[11] = {18, 19, 4, 20, 12, 13, 7, 8, 6, 10, 11};
    const float fa[11] = {gv[7], gv[8], gv[9], gv[10], gv[5], gv[5], gv[11], 1.0f, 0.5f, 2.0f, 2.0f};
    for (int i = 0; i < 11; ++i)
        expect(events[i].a == idx[i] && events[i].fa == fa[i] && events[i].b == 1 && events[i].fc == 0.0f && events[i].d == 0 && events[i].fd == 0.0f && events[i].e == 0 &&
               events[i].fb == (i >= 9 ? gv[6] : 0.0f) && events[i].c == (i >= 9 ? 2 : 0), "event");
    run(false, 4, 0x8000);
    expect(word_at(object + 244) == (1u << 1) && word_at(object + 208) == 0u, "high roll leaves 208");
    // the king: the first free entry at 1228 and the other king words
    run(true, 0x8000, 0);
    expect(roll_calls == 1 && word_at(object + 244) == 0u && chunk_class == 0, "king uses no variant roll");
    expect(word_at(object + 976) == 1u && word_at(object + 980) == 0u && word_at(object + 208) == 1u, "king words (208 is also set by the first branch)");
    expect(half_at(object + 1228) == 24 && half_at(object + 1230) == 10 && half_at(object + 1232) == 10 && half_at(object + 1234) == 0 && half_at(object + 1236) == 0xffff, "first entry written");
    expect(word_at(object + 160) == (0x40000u | 0x20000000u) && word_at(object + 788) == fb(2000.0f), "king flags and 2000.0");
    expect(params[0] == 60.0f && params[1] == 2.0f && params[2] == 4.0f && params[3] == gv[2] && params[4] == 10.5f && param_flag == 1, "king parameters");
    if (fails != 0) return 1;
    std::puts("cloud giant constructor passed");
    return 0;
}
