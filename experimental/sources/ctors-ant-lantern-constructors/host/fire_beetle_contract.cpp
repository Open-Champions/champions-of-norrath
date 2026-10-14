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
static unsigned char model[128];
static unsigned char header[8];
static unsigned char chunk_model[8];
static unsigned char other[160];
static unsigned char table[256];
static unsigned rolls[4];
static int roll_calls;
static float seen_scale; static void* seen_map; static char** seen_args; static short seen_type;
static float params[5]; static int param_flag;
static void* chunk_arg; static int chunk_class;
struct Event { int a; float fa; int b; float fb; int c; float fc; int d; float fd; int e; };
static Event events[8];
static int event_count;
static int virtual_calls; static void* virtual_self; static int virtual_a, virtual_b; static int virtual_result;
static void* add_header; static void* add_state; static void* init_state; static void* random_state;
static unsigned word_at(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static unsigned half_at(const unsigned char* p) { return unsigned(p[0]) | (unsigned(p[1]) << 8); }
static unsigned bits(const void* p) { return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(p)); }
static unsigned fb(float f) { unsigned b; std::memcpy(&b, &f, 4); return b; }
unsigned char norrath_ctors_ant_lantern_view_42c308[152 * 4];
unsigned char norrath_ctors_ant_lantern_view_44bea0[4];
unsigned char norrath_ctors_ant_lantern_view_4814e4[4], norrath_ctors_ant_lantern_view_4814e8[4], norrath_ctors_ant_lantern_view_4814ec[4],
    norrath_ctors_ant_lantern_view_4814f0[4], norrath_ctors_ant_lantern_view_4814f4[4], norrath_ctors_ant_lantern_view_4814f8[4];
unsigned char norrath_ctors_ant_lantern_view_34b474[4];
unsigned char norrath_creature_view_484cac[4];
unsigned char norrath_actor_view_4844a4[4] = {4, 3, 2, 1};
extern "C" unsigned norrath_actor_random(void*) { return rolls[roll_calls++]; }
extern "C" unsigned norrath_creature_construct(void*, const void* point, short type, void* map, float scale, char** args) {
    expect(stage == 0 && roll_calls == 1 && point != nullptr, "creature after the first roll");
    stage = 1; seen_scale = scale; seen_map = map; seen_args = args; seen_type = type;
    std::memset(object + 160, 0, 8);
    std::memset(object + 28, 0, 4);
    const float base = 12.0f;
    std::memcpy(object + 28, &base, 4);   // the original multiplies whatever is at +28 after its own store
    const unsigned m = bits(model);
    std::memcpy(object + 36, &m, 4);
    const unsigned chunk = bits(chunk_model);
    std::memcpy(model + 12, &chunk, 4);
    const unsigned h = bits(header);
    std::memcpy(model + 84, &h, 4);
    std::memset(object + 244, 0, 4);
    return 0;
}
extern "C" unsigned norrath_actor_set_parameters(void*, float a, float b, float c, float d, float e, int f) {
    expect(stage == 1 && roll_calls == 1, "parameters before the second roll");
    stage = 2;
    params[0] = a; params[1] = b; params[2] = c; params[3] = d; params[4] = e; param_flag = f;
    return 0;
}
extern "C" void norrath_actor_animation_initialize(void* s) { expect(stage == 2 && roll_calls == 2, "init after second roll"); stage = 3; init_state = s; }
extern "C" void norrath_actor_animation_add(void* s, void* h, int f) { expect(stage == 3 && f == 1, "add after init"); stage = 4; add_state = s; add_header = h; }
extern "C" void norrath_actor_animation_randomize_time(void* s) { expect(stage == 4, "randomize after add"); stage = 5; random_state = s; }
extern "C" unsigned norrath_actor_add_slow_run_list(void*) { expect(stage == 5 && roll_calls >= 3, "slow list after the optional block"); stage = 6; return 0; }
extern "C" void setAnimEventFlags__8Creatureififififi(void* o, int a, float fa, int b, float fb_, int c, float fc, int d, float fd, int e) {
    expect(stage == 6 && o == object, "events after slow list");
    events[event_count++] = Event{a, fa, b, fb_, c, fc, d, fd, e};
}
extern "C" unsigned long long norrath_actor_model_active_chunks(void* m, int cls) {
    expect(stage == 6 && event_count == 4, "chunks after the events");
    chunk_arg = m; chunk_class = cls;
    return 0xaabbccdd11223344ull;
}
extern "C" unsigned norrath_actor_finish_construction(void*) { expect(stage == 6, "finish last"); stage = 7; return 0xf1f1u; }
static int fake_call(void* self, int a, int b) {
    ++virtual_calls; virtual_self = self; virtual_a = a; virtual_b = b;
    return virtual_result;
}
static void run(unsigned r0, unsigned r1, unsigned r2, unsigned r3, int kind) {
    stage = 0; roll_calls = 0; event_count = 0; virtual_calls = 0;
    rolls[0] = r0; rolls[1] = r1; rolls[2] = r2; rolls[3] = r3;
    std::memset(object, 0x5a, sizeof object);
    unsigned char point[12] = {0};
    char* args[] = {nullptr};
    seen_args = nullptr;
    expect(norrath_construct_FireBeetle(object, point, static_cast<short>(-8), args, kind) == 0xf1f1u && stage == 7, "all calls, finish result");
    expect(seen_type == -8 && seen_args == args, "type and arguments");
}
int main() {
    norrath_ctors_ant_lantern::host_note(model);
    norrath_ctors_ant_lantern::host_note(header);
    norrath_ctors_ant_lantern::host_note(chunk_model);
    norrath_ctors_ant_lantern::host_note(other);
    norrath_ctors_ant_lantern::host_note(table);
    norrath_ctors_ant_lantern::host_note(reinterpret_cast<void*>(+fake_call));
    const float shared = 2.5f, k8 = 0.5f, kc = 1.5f, threshold = 0.25f, k4 = 2.0f, k6 = 7.5f, view_value = 9.75f;
    std::memcpy(norrath_ctors_ant_lantern_view_4814e4, &shared, 4);
    std::memcpy(norrath_ctors_ant_lantern_view_4814e8, &k8, 4);
    std::memcpy(norrath_ctors_ant_lantern_view_4814ec, &kc, 4);
    std::memcpy(norrath_ctors_ant_lantern_view_4814f0, &threshold, 4);
    std::memcpy(norrath_ctors_ant_lantern_view_4814f4, &k4, 4);
    std::memcpy(norrath_ctors_ant_lantern_view_4814f8, &k6, 4);
    std::memcpy(norrath_creature_view_484cac, &view_value, 4);
    const unsigned ob = bits(other), tb = bits(table);
    std::memcpy(norrath_ctors_ant_lantern_view_34b474, &ob, 4);
    std::memcpy(other + 96, &tb, 4);
    table[96] = 0x20; table[97] = 0;
    const unsigned fnb = bits(reinterpret_cast<void*>(+fake_call));
    std::memcpy(table + 100, &fnb, 4);
    virtual_result = 100;
    // roll 4 (index 3) is only consumed inside the block. 0x4000 * 2^-16 = 0.25, not below the threshold: no block
    run(20, 10, 0x4000, 0, 2);   // 20 % 6 + 8 = 10; 10 % 6 = 4
    expect(seen_scale == 10.0f && seen_map == norrath_ctors_ant_lantern_view_42c308 + 152 * 2, "scale and record 2");
    expect(roll_calls == 3 && virtual_calls == 0, "no block at the threshold");
    expect(half_at(object + 1052) == 0xffff && half_at(object + 1044) == 0xffff && half_at(object + 1036) == 0xffff && half_at(object + 1028) == 0xffff, "four 0xffff halves");
    expect(object[1024] == 0xff && object[1025] == 0xff && object[1007] == 0x5a && object[1026] == 0x5a, "marker bytes");
    for (unsigned i = 1008; i < 1024; ++i) expect(object[i] == 0, "zero run");
    expect(params[0] == 1.0f && params[1] == shared && params[2] == shared && params[3] == 1.0f && params[4] == shared && param_flag == 1, "parameters");
    expect(word_at(object + 160) == 0x1000u && word_at(object + 164) == 0x80u, "flag words");
    expect(word_at(object + 168) == fb(20.0f) && word_at(object + 176) == fb(180.0f) && word_at(object + 92) == fb(-0.25f) && word_at(object + 88) == fb(1.0f), "singles");
    expect(object[172] == 18 && word_at(object + 248) == 0x01020304u && word_at(object + 72) == bits(norrath_ctors_ant_lantern_view_44bea0), "byte, global and vtable");
    expect(word_at(object + 236) == 0 && word_at(object + 964) == 0 && half_at(object + 572) == 0, "clears");
    const float factor = 4.0f * k8 + kc;
    expect(word_at(object + 224) == fb(factor) && word_at(object + 28) == fb(12.0f * factor), "factor stored and multiplied into +28");
    expect(init_state == object + 288 && add_state == object + 288 && random_state == object + 288 && add_header == header, "animation with model+84");
    expect(word_at(object + 244) == 1u && chunk_class == 1 && chunk_arg == chunk_model, "bit 1 in +244 and chunk call");
    expect(word_at(object + 552) == 0x11223344u && word_at(object + 556) == 0xaabbccddu, "eight-byte result at 552");
    expect(events[0].a == 3 && events[0].fa == k6 && events[0].b == 1, "event 3");
    for (int i = 1; i < 4; ++i) expect(events[i].a == 3 + i && events[i].fa == 0.5f && events[i].b == 2, "events 4 to 6");
    for (int i = 0; i < 4; ++i) expect(events[i].c == 0 && events[i].fb == 0.0f && events[i].fc == 0.0f && events[i].d == 0 && events[i].fd == 0.0f && events[i].e == 0, "event zeros");
    // block: roll 0x3fff * 2^-16 < 0.25; fourth roll 0x8000 -> 0.5
    run(20, 10, 0x3fff, 0x8000, 0);
    expect(virtual_calls == 1 && virtual_self == other + 0x20 && virtual_a == 0 && virtual_b == 9, "virtual call: adjusted self, 0, truncated view single");
    // product = 100 * 2.0 * 0.5 = 100 -> total 200; first slot (-1) gets the entry
    expect(half_at(object + 1028) == 21 && half_at(object + 1030) == 100 && half_at(object + 1032) == 200 && half_at(object + 1034) == 0, "entry in the first slot");
    expect(half_at(object + 1036) == 0xffff, "second slot untouched");
    if (fails != 0) return 1;
    std::puts("fire beetle constructor passed");
    return 0;
}
