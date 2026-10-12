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
static unsigned char model[96];
static unsigned char header[8];
static unsigned rolls[4];
static int roll_calls;
static void* seen_map; static float seen_scale; static char** seen_args; static short seen_type;
static float params[5]; static int param_flag;
struct Event { int a; float fa; int b; float fb; int c; float fc; int d; float fd; int e; };
static Event events[8]; static int event_count;
static void* add_header; static void* add_state; static void* init_state;
static int dp_calls[3];
static float fptodp_input;
static unsigned word_at(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static unsigned half_at(const unsigned char* p) { return unsigned(p[0]) | (unsigned(p[1]) << 8); }
static unsigned bits(const void* p) { return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(p)); }
static unsigned fb(float f) { unsigned b; std::memcpy(&b, &f, 4); return b; }
static double to_double(unsigned long long b) { double d; std::memcpy(&d, &b, 8); return d; }
static unsigned long long from_double(double d) { unsigned long long b; std::memcpy(&b, &d, 8); return b; }
unsigned char norrath_ctors_ant_lantern_view_438a38[4], norrath_ctors_ant_lantern_view_450160[4];
unsigned char norrath_ctors_ant_lantern_view_482328[4], norrath_ctors_ant_lantern_view_48232c[4], norrath_ctors_ant_lantern_view_482330[4],
    norrath_ctors_ant_lantern_view_482334[4], norrath_ctors_ant_lantern_view_482338[4], norrath_ctors_ant_lantern_view_48233c[4],
    norrath_ctors_ant_lantern_view_482340[4];
unsigned char norrath_ctors_ant_lantern_view_476ac8[8];
unsigned char norrath_actor_view_4844a4[4] = {4, 3, 2, 1};
extern "C" unsigned norrath_actor_random(void*) { return rolls[roll_calls++]; }
extern "C" unsigned norrath_creature_construct(void*, const void* point, short type, void* map, float scale, char** args) {
    expect(stage == 0 && roll_calls == 0 && point != nullptr, "creature first");
    stage = 1; seen_map = map; seen_scale = scale; seen_args = args; seen_type = type;
    std::memset(object + 12, 0, 4);
    std::memset(object + 160, 0, 8);
    const unsigned m = bits(model);
    std::memcpy(object + 36, &m, 4);
    const unsigned h = bits(header);
    std::memcpy(model + 76, &h, 4);
    const unsigned base = 5000u;
    std::memcpy(object + 808, &base, 4);
    return 0;
}
extern "C" unsigned long long norrath_entity_fptodp(float v) { ++dp_calls[0]; fptodp_input = v; return from_double(static_cast<double>(v)); }
extern "C" unsigned long long dpadd(unsigned long long l, unsigned long long r) { ++dp_calls[1]; return from_double(to_double(l) + to_double(r)); }
extern "C" float dptofp(unsigned long long v) { ++dp_calls[2]; return static_cast<float>(to_double(v)); }
extern "C" unsigned norrath_actor_set_parameters(void*, float a, float b, float c, float d, float e, int f) {
    expect(stage == 1 && roll_calls == 4, "parameters after four rolls");
    stage = 2;
    params[0] = a; params[1] = b; params[2] = c; params[3] = d; params[4] = e; param_flag = f;
    return 0;
}
extern "C" void norrath_actor_animation_initialize(void* s) { expect(stage == 2, "init after parameters"); stage = 3; init_state = s; }
extern "C" void norrath_actor_animation_add(void* s, void* h, int f) { expect(stage == 3 && f == 1, "add after init"); stage = 4; add_state = s; add_header = h; }
extern "C" unsigned norrath_actor_add_slow_run_list(void*) { expect(stage == 4, "slow list after add"); stage = 5; return 0; }
extern "C" void setAnimEventFlags__8Creatureififififi(void*, int a, float fa, int b, float fb_, int c, float fc, int d, float fd, int e) {
    expect(stage == 5, "events after slow list");
    events[event_count++] = Event{a, fa, b, fb_, c, fc, d, fd, e};
}
extern "C" unsigned norrath_actor_finish_construction(void*) { expect(stage == 5 && event_count == 7, "finish last"); stage = 7; return 0xf1f1u; }
int main() {
    norrath_ctors_ant_lantern::host_note(model);
    norrath_ctors_ant_lantern::host_note(header);
    const float g[7] = {2.0f, 1.25f, 2.25f, 3.25f, 4.25f, 5.25f, 6.25f};
    unsigned char* views[7] = {norrath_ctors_ant_lantern_view_482328, norrath_ctors_ant_lantern_view_48232c, norrath_ctors_ant_lantern_view_482330,
        norrath_ctors_ant_lantern_view_482334, norrath_ctors_ant_lantern_view_482338, norrath_ctors_ant_lantern_view_48233c, norrath_ctors_ant_lantern_view_482340};
    for (int i = 0; i < 7; ++i) std::memcpy(views[i], &g[i], 4);
    const double constant = 0.3;
    std::memcpy(norrath_ctors_ant_lantern_view_476ac8, &constant, 8);
    rolls[0] = 0x8000; rolls[1] = 1300; rolls[2] = 7; rolls[3] = 9;
    std::memset(object, 0x5a, sizeof object);
    unsigned char point[12] = {0};
    char* args[] = {nullptr};
    const unsigned r = norrath_construct_Cthulu(object, point, static_cast<short>(-3), args);
    expect(r == 0xf1f1u && stage == 7, "all calls, finish result");
    expect(seen_scale == 10.0f && seen_map == norrath_ctors_ant_lantern_view_438a38 && seen_type == -3 && seen_args == args, "creature arguments");
    expect(dp_calls[0] == 1 && dp_calls[1] == 1 && dp_calls[2] == 1, "one widen, one add, one narrow");
    expect(fptodp_input == 0.5f * g[0], "roll scaled by 2^-16 and the first single");
    const float value = static_cast<float>(static_cast<double>(0.5f * g[0]) + 0.3);
    expect(word_at(object + 224) == fb(value), "value stored at +224");
    expect(word_at(object + 28) == fb(20.0f * value), "+28 is 20.0 times the value");
    expect(word_at(object + 228) == fb(value * -15.0f), "behind value");
    expect(word_at(object + 960) == 5000u + 1300u % 600u && word_at(object + 968) == 0 && word_at(object + 964) == 1u && word_at(object + 972) == 1u, "word stores from rolls");
    expect(word_at(object + 72) == bits(norrath_ctors_ant_lantern_view_450160) && word_at(object + 248) == 0x01020304u, "vtable and global");
    expect(word_at(object + 168) == fb(1.0f) && word_at(object + 176) == 0 && word_at(object + 192) == 0 && word_at(object + 236) == 0 && word_at(object + 256) == 0 && half_at(object + 572) == 0, "singles and clears");
    expect(word_at(object + 160) == 0x2000u && word_at(object + 164) == 2u, "flag words");
    expect(params[0] == g[3] && params[1] == g[4] && params[2] == 1.0f && params[3] == 1.0f && params[4] == 1.0f && param_flag == 1, "parameters");
    expect(init_state == object + 288 && add_state == object + 288 && add_header == header, "animation with model+76");
    const int idx[7] = {3, 4, 5, 6, 11, 18, 14};
    const float want[7] = {g[1], g[2], g[2], g[5], g[6], g[1], 1.0f};
    for (int i = 0; i < 7; ++i)
        expect(events[i].a == idx[i] && events[i].fa == want[i] && events[i].b == (i == 6 ? 4 : 1) && events[i].fb == 0.0f && events[i].c == 0 &&
               events[i].fc == 0.0f && events[i].d == 0 && events[i].fd == 0.0f && events[i].e == 0, "event");
    if (fails != 0) return 1;
    std::puts("cthulu constructor passed");
    return 0;
}
