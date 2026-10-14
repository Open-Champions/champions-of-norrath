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
static unsigned char object[1100];
static unsigned char model[128];
static unsigned char header[8];
static unsigned char chunk_model[8];
static unsigned rolls[8]; static int roll_calls;
static int compares; static int match_at;
static int color_tag, type_tag;
static float seen_scale; static void* seen_map; static char** seen_args; static short seen_type;
static float params[5]; static int param_flag; static int param_calls;
static int chunk_class; static void* chunk_arg;
static int dp_calls[3];
struct Event { int a; float fa; int b; float fb; int c; float fc; int d; float fd; int e; };
static Event events[8]; static int event_count;
static void* add_header; static void* add_state; static void* init_state;
static unsigned word_at(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static unsigned half_at(const unsigned char* p) { return unsigned(p[0]) | (unsigned(p[1]) << 8); }
static unsigned bits(const void* p) { return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(p)); }
static unsigned fb(float f) { unsigned b; std::memcpy(&b, &f, 4); return b; }
static double to_double(unsigned long long b) { double d; std::memcpy(&d, &b, 8); return d; }
static unsigned long long from_double(double d) { unsigned long long b; std::memcpy(&b, &d, 8); return b; }
#define VIEW(name, size) unsigned char norrath_ctors_ant_lantern_view_##name[size]
VIEW(42be08, 4); VIEW(42beb0, 4); VIEW(44c530, 4); VIEW(48124c, 4); VIEW(481250, 4); VIEW(481254, 4); VIEW(481258, 4); VIEW(48125c, 4); VIEW(481260, 4);
VIEW(481264, 4); VIEW(481268, 4); VIEW(48126c, 4); VIEW(481270, 4); VIEW(481274, 4); VIEW(481278, 4); VIEW(48127c, 4); VIEW(481280, 4); VIEW(481284, 4);
VIEW(481288, 4); VIEW(48128c, 4); VIEW(481290, 4); VIEW(481294, 4); VIEW(481298, 4);
VIEW(46fa28, 16); VIEW(46fa38, 16); VIEW(46fa48, 16); VIEW(46fa58, 16); VIEW(4852d8, 16); VIEW(4852e0, 16);
unsigned char norrath_actor_view_4844a4[4] = {4, 3, 2, 1};
static unsigned char* const name_views[4] = {norrath_ctors_ant_lantern_view_46fa28, norrath_ctors_ant_lantern_view_46fa38, norrath_ctors_ant_lantern_view_46fa48, norrath_ctors_ant_lantern_view_46fa58};
static float g(unsigned char* view) { float f; std::memcpy(&f, view, 4); return f; }
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
    std::memcpy(model + 84, &h, 4);
    return 0;
}
extern "C" int norrath_game_object_find_tag_int(void*, char* tag, int) {
    if (tag == reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_4852d8)) return color_tag;
    expect(tag == reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_4852e0), "type tag");
    return type_tag;
}
extern "C" int strticmp__FPCcT0(const char* l, const char* r) {
    ++compares;
    expect(l != nullptr && r == reinterpret_cast<const char*>(name_views[compares - 1]), "compare order and texts");
    return compares == match_at ? 0 : 1;
}
extern "C" unsigned long long norrath_entity_fptodp(float v) { ++dp_calls[0]; return from_double(static_cast<double>(v)); }
extern "C" unsigned long long dpadd(unsigned long long l, unsigned long long r) { ++dp_calls[1]; expect(to_double(r) == 0.75, "the added double is 0.75"); return from_double(to_double(l) + to_double(r)); }
extern "C" float dptofp(unsigned long long v) { ++dp_calls[2]; return static_cast<float>(to_double(v)); }
extern "C" unsigned norrath_actor_set_parameters(void*, float a, float b, float c, float d, float e, int f) {
    expect(stage == 1, "parameters after creature");
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
extern "C" unsigned norrath_actor_finish_construction(void*) { expect(stage == 6 && event_count == 6, "finish last"); stage = 7; return 0xf1f1u; }
static void run(char kind, int match, int color, int type_value, std::initializer_list<unsigned> list) {
    stage = 0; roll_calls = 0; event_count = 0; compares = 0; match_at = match; param_calls = 0;
    color_tag = color; type_tag = type_value;
    std::memset(dp_calls, 0, sizeof dp_calls);
    int i = 0; for (unsigned v : list) rolls[i++] = v;
    std::memset(object, 0x5a, sizeof object);
    unsigned char point[12] = {0};
    char* args[] = {nullptr};
    char name[] = "nm";
    expect(norrath_construct_Ant(object, point, static_cast<short>(-3), args, name, kind) == 0xf1f1u && stage == 7, "finish result");
    expect(seen_type == -3 && seen_args == args && param_calls == 1, "creature arguments and one parameters call");
}
int main() {
    norrath_ctors_ant_lantern::host_note(model);
    norrath_ctors_ant_lantern::host_note(header);
    norrath_ctors_ant_lantern::host_note(chunk_model);
    unsigned char* views[19] = {norrath_ctors_ant_lantern_view_48124c, norrath_ctors_ant_lantern_view_481250, norrath_ctors_ant_lantern_view_481254, norrath_ctors_ant_lantern_view_481258,
        norrath_ctors_ant_lantern_view_48125c, norrath_ctors_ant_lantern_view_481260, norrath_ctors_ant_lantern_view_481264, norrath_ctors_ant_lantern_view_481268,
        norrath_ctors_ant_lantern_view_48126c, norrath_ctors_ant_lantern_view_481270, norrath_ctors_ant_lantern_view_481274, norrath_ctors_ant_lantern_view_481278,
        norrath_ctors_ant_lantern_view_48127c, norrath_ctors_ant_lantern_view_481280, norrath_ctors_ant_lantern_view_481284, norrath_ctors_ant_lantern_view_481288,
        norrath_ctors_ant_lantern_view_48128c, norrath_ctors_ant_lantern_view_481290, norrath_ctors_ant_lantern_view_481294};
    float gv[19];
    for (int i = 0; i < 19; ++i) { gv[i] = 0.125f * static_cast<float>(i + 2); std::memcpy(views[i], &gv[i], 4); }
    gv[17] = 0.5f; std::memcpy(norrath_ctors_ant_lantern_view_481290, &gv[17], 4);    // roll threshold
    const float last = 7.75f; std::memcpy(norrath_ctors_ant_lantern_view_481298, &last, 4);
    // kind 0, no name match, color tag 0 -> byte 960 = 0, type tag 0 -> byte 968 = 0 ; branch B/ not 968==1
    run(0, 0, 0, 0, {30, 1, 4, 0x3fff, 5});
    expect(seen_scale == 6.0f && seen_map == norrath_ctors_ant_lantern_view_42be08, "kind 0 map and scale");
    expect(object[40] == 20 && word_at(object + 160) == 0x1000u && word_at(object + 164) == 0x80u && word_at(object + 176) == fb(140.0f) && word_at(object + 168) == fb(gv[0]) &&
           word_at(object + 248) == 0x01020304u && word_at(object + 72) == bits(norrath_ctors_ant_lantern_view_44c530) && object[172] == 18, "fixed stores");
    expect(word_at(object + 192) == 0 && word_at(object + 236) == 0 && half_at(object + 572) == 0 && word_at(object + 972) == 0 && word_at(object + 980) == 0, "clears");
    expect(object[960] == 0 && object[968] == 0 && compares == 4, "tags override the roll bit");
    // branch B, 968 != 1: factor = float((double)((4 % 6) * gv[6]) + 0.75), parameters from views 7, 8
    {
        const float scaled = 4.0f * gv[6];
        const float want = static_cast<float>(static_cast<double>(scaled) + 0.75);
        expect(word_at(object + 224) == fb(want) && dp_calls[0] == 1 && dp_calls[1] == 1 && dp_calls[2] == 1, "branch B factor through the double routines");
        expect(params[0] == gv[7] && params[1] == gv[8] && params[2] == 0.5f && params[3] == 1.0f && params[4] == 1.0f && param_flag == 1, "branch B parameters");
        expect(word_at(object + 88) == fb(gv[9]), "branch B +88");
        // byte 968 is 0 -> a roll below the threshold sets +208: 0x3fff*2^-16 < 0.5 -> (5 & 1) + 1 = 2 and byte 156 = 2
        expect(word_at(object + 208) == 2u && object[156] == 2, "208 and byte 156");
        expect(word_at(object + 28) == fb(18.0f * want), "+28 times the factor");
    }
    expect(chunk_class == static_cast<int>(word_at(object + 244)) && chunk_arg == chunk_model && word_at(object + 552) == 0x11223344u && word_at(object + 556) == 0xaabbccddu, "chunks");
    expect(init_state == object + 288 && add_state == object + 288 && add_header == header, "animation with model+84");
    const int idx[6] = {4, 5, 14, 6, 7, 8};
    const float fa[6] = {gv[18], last, gv[18], 0.5f, 0.5f, 0.5f};
    for (int i = 0; i < 6; ++i)
        expect(events[i].a == idx[i] && events[i].fa == fa[i] && events[i].b == 1 && events[i].fb == 0.0f && events[i].c == 0 && events[i].fc == 0.0f && events[i].d == 0 && events[i].fd == 0.0f && events[i].e == 0, "event");
    // kind 5: the second map
    run(5, 0, 0, 0, {30, 1, 4, 0x8000});
    expect(seen_map == norrath_ctors_ant_lantern_view_42beb0 && word_at(object + 208) == 0u && object[156] == 0, "kind 5 map, a high roll leaves 208");
    // roll bit when the color tag is nonzero: the tag value is stored whole
    run(0, 0, 3, 1, {30, 1, 4, 0x8000});
    expect(object[960] == 3 && object[968] == 1, "tag values stored as bytes");
    // branch B with 968 == 1 and 960 == 0: 244 = 1, 224 from view 3, parameters from views 4, 5
    run(0, 0, 0, 1, {30, 1, 4});
    expect(word_at(object + 244) == 1u && word_at(object + 224) == fb(gv[3]) && params[0] == 1.5f && params[1] == gv[4] && params[2] == gv[5] && word_at(object + 88) == fb(gv[9]) && dp_calls[0] == 0, "branch B, byte 968 = 1");
    expect(word_at(object + 208) == 0u && object[156] == 0, "byte 968 nonzero skips the 208 roll");
    // branch C: byte 960 == 1 -> 244 = 1, 224 from view 10, parameters from views 11, 12, +88 from view 16
    run(0, 0, 1, 2, {30, 1, 4});
    expect(word_at(object + 244) == 1u && word_at(object + 224) == fb(gv[10]) && params[0] == 1.5f && params[1] == gv[11] && params[2] == gv[12] && word_at(object + 88) == fb(gv[16]), "branch C, byte 960 = 1");
    // branch C: byte 960 other (2): factor from view 13, parameters from views 14, 15
    run(0, 0, 2, 2, {30, 1, 4});
    {
        const float want = static_cast<float>(static_cast<double>(4.0f * gv[13]) + 0.75);
        expect(word_at(object + 224) == fb(want) && params[0] == gv[14] && params[1] == gv[15] && params[2] == 0.5f && dp_calls[0] == 1, "branch C, other 960");
    }
    // names
    for (int n = 1; n <= 4; ++n) {
        run(1, n, 0, 0, {30, 1, 4});
        expect(compares == n && word_at(object + 244) == 0u, "kind 1 clears 244 with an even roll");
        expect(object[960] == 0, "kind 1 clears byte 960 after the name logic");
    }
    // kind 1 details: scale, roll low bit, parameters, byte 968 == 2
    run(1, 0, 0, 0, {30, 1, 5});
    expect(seen_map == norrath_ctors_ant_lantern_view_42beb0 && word_at(object + 244) == 1u && word_at(object + 224) == fb(gv[1]) && word_at(object + 28) == fb(20.0f * gv[1]), "kind 1 stores");
    expect(params[0] == 3.0f && params[1] == 1.5f && params[2] == 1.5f && params[3] == 1.0f && params[4] == 1.0f, "kind 1 parameters");
    run(1, 4, 0, 0, {30, 1, 4});    // name 4 -> (960, 968) = (0, 2): 28 = 25.0 then multiplied by +224
    expect(object[968] == 2 && word_at(object + 224) == fb(gv[2]) && word_at(object + 28) == fb(25.0f * gv[2]), "kind 1 with byte 968 = 2");
    // name effects on bytes: check with kind 0 (bytes are not cleared)
    run(0, 1, 0, 0, {30, 1, 4, 0x8000});
    expect(object[960] == 1 && object[968] == 0, "first name: byte 960 = 1");
    run(0, 2, 0, 0, {30, 1, 4});
    expect(object[960] == 1 && object[968] == 1, "second name: (1, 1)");
    run(0, 3, 0, 0, {30, 1, 4});
    expect(object[960] == 0 && object[968] == 1, "third name: (0, 1)");
    run(0, 4, 0, 0, {30, 1, 4, 0x3fff, 5});
    expect(object[960] == 0 && object[968] == 2, "fourth name: (0, 2)");
    if (fails != 0) return 1;
    std::puts("ant constructor passed");
    return 0;
}
