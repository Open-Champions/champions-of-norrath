#include "../detail.hpp"
#include <cstdio>
#include <cstdint>
#include <cstring>
static int fails;
static void expect(bool cond, const char* message) {
    if (!cond) { std::printf("FAIL %s\n", message); ++fails; }
}
static int stage;
static unsigned char object[1200];
static unsigned char model[80];
static unsigned char header[8];
static unsigned char chunk_model[8];
static unsigned rolls[2]; static int roll_calls;
static int compares; static int match_at;
static float seen_scale; static void* seen_map; static char** seen_args; static short seen_type;
static float params[5]; static int param_flag; static int variant_calls;
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
unsigned char norrath_ctors_ant_lantern_view_42b958[4], norrath_ctors_ant_lantern_view_44bcc0[4], norrath_ctors_ant_lantern_view_46f120[16], norrath_ctors_ant_lantern_view_46f130[16];
unsigned char norrath_ctors_ant_lantern_view_48109c[4], norrath_ctors_ant_lantern_view_4810a0[4], norrath_ctors_ant_lantern_view_4810a4[4],
    norrath_ctors_ant_lantern_view_4810a8[4], norrath_ctors_ant_lantern_view_4810ac[4], norrath_ctors_ant_lantern_view_4810b0[4];
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
    std::memcpy(model + 60, &h, 4);
    return 0;
}
extern "C" int strticmp__FPCcT0(const char* l, const char* r) {
    ++compares;
    const unsigned char* want = compares == 1 ? norrath_ctors_ant_lantern_view_46f120 : norrath_ctors_ant_lantern_view_46f130;
    expect(stage == 1 && r == reinterpret_cast<const char*>(want) && l != nullptr, "compare order and texts");
    return compares == match_at ? 0 : 1;
}
extern "C" unsigned norrath_actor_random(void*) { return rolls[roll_calls++]; }
extern "C" void initAttackVariant__8Froglock(void*) { expect(stage == 1, "variant after the name logic"); stage = 2; ++variant_calls; }
extern "C" unsigned norrath_actor_set_parameters(void*, float a, float b, float c, float d, float e, int f) {
    expect(stage == 2, "parameters after the variant call");
    stage = 3;
    params[0] = a; params[1] = b; params[2] = c; params[3] = d; params[4] = e; param_flag = f;
    return 0;
}
extern "C" unsigned long long norrath_actor_model_active_chunks(void* m, int cls) { expect(stage == 3, "chunks after parameters"); stage = 4; chunk_arg = m; chunk_class = cls; return 0xaabbccdd11223344ull; }
extern "C" void norrath_actor_animation_initialize(void* s) { expect(stage == 4, "init after chunks"); stage = 5; init_state = s; }
extern "C" void norrath_actor_animation_add(void* s, void* h, int f) { expect(stage == 5 && f == 1, "add after init"); stage = 6; add_state = s; add_header = h; }
extern "C" unsigned norrath_actor_add_slow_run_list(void*) { expect(stage == 6, "slow list after add"); stage = 7; return 0; }
extern "C" void setAnimEventFlags__8Creatureififififi(void*, int a, float fa, int b, float fb_, int c, float fc, int d, float fd, int e) {
    expect(stage == 7, "events after slow list");
    events[event_count++] = Event{a, fa, b, fb_, c, fc, d, fd, e};
}
extern "C" unsigned norrath_actor_finish_construction(void*) { expect(stage == 7 && event_count == 5, "finish last"); stage = 8; return 0xf1f1u; }
static void run(int match, unsigned r0, unsigned r1) {
    stage = 0; roll_calls = 0; event_count = 0; compares = 0; match_at = match; variant_calls = 0;
    rolls[0] = r0; rolls[1] = r1;
    std::memset(object, 0x5a, sizeof object);
    unsigned char point[12] = {0};
    char* args[] = {nullptr};
    char name[] = "nm";
    expect(norrath_construct_Froglock(object, point, static_cast<short>(-3), args, name) == 0xf1f1u && stage == 8, "finish result");
    expect(seen_scale == 10.0f && seen_map == norrath_ctors_ant_lantern_view_42b958 && seen_type == -3 && seen_args == args && variant_calls == 1, "creature arguments");
}
int main() {
    norrath_ctors_ant_lantern::host_note(model);
    norrath_ctors_ant_lantern::host_note(header);
    norrath_ctors_ant_lantern::host_note(chunk_model);
    float g[7];
    unsigned char* views[7] = {norrath_ctors_ant_lantern_view_48109c, norrath_ctors_ant_lantern_view_4810a0, norrath_ctors_ant_lantern_view_4810a4,
        norrath_ctors_ant_lantern_view_4810a8, norrath_ctors_ant_lantern_view_4810ac, norrath_ctors_ant_lantern_view_4810b0, nullptr};
    g[0] = 0.5f; g[1] = 1.5f; g[2] = 0.5f; g[3] = 0.25f; g[4] = 3.5f; g[5] = 4.5f;
    for (int i = 0; i < 6; ++i) std::memcpy(views[i], &g[i], 4);
    run(0, 0, 0);
    expect(compares == 2 && word_at(object + 960) == 17u && word_at(object + 208) == 0x5a5a5a5au, "no name match keeps 17 and skips rolls");
    expect(word_at(object + 72) == bits(norrath_ctors_ant_lantern_view_44bcc0) && object[40] == 35 && word_at(object + 164) == 2u && word_at(object + 160) == 0u, "vtable, byte 40, flag words");
    expect(word_at(object + 168) == fb(g[0]) && word_at(object + 28) == fb(18.0f) && word_at(object + 100) == fb(g[1]) && object[172] == 9 && word_at(object + 248) == 0x01020304u, "stores");
    expect(word_at(object + 176) == 0 && word_at(object + 192) == 0 && word_at(object + 236) == 0 && half_at(object + 572) == 0 && word_at(object + 244) == 0, "clears");
    for (unsigned offset = 1068; offset <= 1092; offset += 8) expect(half_at(object + offset) == 0xffff, "0xffff halves");
    expect(object[1064] == 0xff && object[1065] == 0xff && object[1047] == 0x5a && object[1066] == 0x5a, "marker bytes");
    for (unsigned i = 1048; i < 1064; ++i) expect(object[i] == 0, "zero run");
    expect(params[0] == g[4] && params[1] == 1.0f && params[2] == 1.0f && params[3] == 1.0f && params[4] == 1.0f && param_flag == 1, "parameters");
    expect(chunk_class == 0 && chunk_arg == chunk_model && word_at(object + 552) == 0x11223344u && word_at(object + 556) == 0xaabbccddu, "chunks");
    expect(init_state == object + 288 && add_state == object + 288 && add_header == header, "animation with model+60");
    const int idx[5] = {12, 3, 13, 4, 5};
    for (int i = 0; i < 5; ++i) expect(events[i].a == idx[i], "event index");
    for (int i = 0; i < 3; ++i) expect(events[i].fa == g[5] && events[i].b == 1, "first three events");
    expect(events[3].fa == 1.0f && events[3].b == 16 && events[4].fa == 1.0f && events[4].b == 16, "last two events");
    for (int i = 0; i < 5; ++i) expect(events[i].fb == 0.0f && events[i].c == 0 && events[i].fc == 0.0f && events[i].d == 0 && events[i].fd == 0.0f && events[i].e == 0, "zeros");
    // first name: the roll (scaled by 2^-16) must be below the first threshold
    run(1, 0x7fff, 0x0005);   // 0.49998 < 0.5 -> (5 & 1) + 1 = 2
    expect(compares == 1 && word_at(object + 960) == 17u && word_at(object + 208) == 2u, "first name with a low roll");
    run(1, 0x8000, 0x0005);
    expect(word_at(object + 208) == 0x5a5a5a5au && roll_calls == 1, "first name with a high roll");
    // second name
    run(2, 0x3fff, 0x0006);   // 0.2499 < 0.25 -> (6 & 3) + 1 = 3
    expect(compares == 2 && word_at(object + 960) == 16u && word_at(object + 208) == 3u, "second name with a low roll");
    run(2, 0x4000, 0x0006);
    expect(word_at(object + 960) == 16u && word_at(object + 208) == 0x5a5a5a5au, "second name with a high roll");
    if (fails != 0) return 1;
    std::puts("froglock constructor passed");
    return 0;
}
