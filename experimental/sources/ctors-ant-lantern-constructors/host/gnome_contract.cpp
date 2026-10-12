#include "../detail.hpp"
#include <cstdio>
#include <cstdint>
#include <cstring>
static int fails;
static void expect(bool cond, const char* message) {
    if (!cond) { std::printf("FAIL %s\n", message); ++fails; }
}
static int stage;
static unsigned char object[1024];
static unsigned char model[64];
static unsigned char chunk_model[8];
static unsigned char header[8];
static unsigned rand_value;
static int compares;
static int match_at;
static const char* seen_name;
static void* seen_map; static float seen_scale; static char** seen_args; static short seen_type;
static float params[5]; static int param_flag;
static void* chunk_arg; static int chunk_class;
static void* add_header; static int add_flag; static void* add_state; static void* init_state;
static unsigned word_at(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static unsigned half_at(const unsigned char* p) { return unsigned(p[0]) | (unsigned(p[1]) << 8); }
static unsigned bits(const void* p) { return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(p)); }
static unsigned fb(float f) { unsigned b; std::memcpy(&b, &f, 4); return b; }
unsigned char norrath_ctors_ant_lantern_view_42ba00[4];
unsigned char norrath_ctors_ant_lantern_view_44b9f0[4];
unsigned char norrath_ctors_ant_lantern_view_480b10[4];
unsigned char norrath_ctors_ant_lantern_view_480b14[4];
unsigned char norrath_ctors_ant_lantern_view_46f310[4];
unsigned char norrath_ctors_ant_lantern_view_46f320[4];
unsigned char norrath_ctors_ant_lantern_view_46f330[4];
unsigned char norrath_actor_view_4844a4[4] = {1, 2, 3, 4};
extern "C" unsigned norrath_creature_construct(void* o, const void* point, short type, void* map, float scale, char** args) {
    expect(stage == 0 && point != nullptr, "creature first");
    stage = 1; seen_map = map; seen_scale = scale; seen_args = args; seen_type = type;
    std::memset(object + 12, 0, 4);
    std::memset(object + 160, 0, 8);
    const unsigned m = bits(model);
    std::memcpy(object + 36, &m, 4);
    const unsigned chunk = bits(chunk_model);
    std::memcpy(model + 12, &chunk, 4);
    const unsigned h = bits(header);
    std::memcpy(model + 28, &h, 4);
    (void)o;
    return 0;
}
extern "C" unsigned norrath_actor_set_parameters(void*, float a, float b, float c, float d, float e, int f) {
    expect(stage == 1, "parameters after creature");
    stage = 2;
    params[0] = a; params[1] = b; params[2] = c; params[3] = d; params[4] = e; param_flag = f;
    expect(word_at(object + 12) == 0x01000000u && word_at(object + 160) == 0u && word_at(object + 164) == 0x00040004u, "flags, high word");
    expect(half_at(object + 792) == 0xffff && object[172] == 9 && object[40] == 15 && word_at(object + 28) == fb(15.0f), "small stores");
    expect(word_at(object + 972) == 0 && word_at(object + 968) == 0 && word_at(object + 236) == 0, "clears before parameters");
    return 0;
}
extern "C" unsigned norrath_actor_random(void*) { expect(stage == 2, "random after parameters"); stage = 3; return rand_value; }
extern "C" int strticmp__FPCcT0(const char* left, const char* right) {
    ++compares;
    seen_name = left;
    const unsigned char* want = compares == 1 ? norrath_ctors_ant_lantern_view_46f310 : compares == 2 ? norrath_ctors_ant_lantern_view_46f320 : norrath_ctors_ant_lantern_view_46f330;
    expect(right == reinterpret_cast<const char*>(want), "compare order");
    return compares == match_at ? 0 : 1;
}
extern "C" void initType__5Gnome(void*) {
    expect(stage == 3, "initType after the name compare");
    stage = 4;
    std::memcpy(object + 244, "\x09\x00\x00\x00", 4);
}
extern "C" unsigned long long norrath_actor_model_active_chunks(void* m, int cls) {
    expect(stage == 4, "chunks after initType");
    stage = 5; chunk_arg = m; chunk_class = cls;
    return 0x1122334455667788ull;
}
extern "C" void norrath_actor_animation_initialize(void* s) { expect(stage == 5, "init after chunks"); stage = 6; init_state = s; }
extern "C" void norrath_actor_animation_add(void* s, void* h, int f) { expect(stage == 6, "add after init"); stage = 7; add_state = s; add_header = h; add_flag = f; }
extern "C" unsigned norrath_actor_add_slow_run_list(void*) { expect(stage == 7, "slow list after add"); stage = 8; return 0; }
extern "C" unsigned norrath_actor_finish_construction(void*) { expect(stage == 8, "finish last"); stage = 9; return 0xf1f1u; }
static unsigned run(unsigned roll, int match) {
    stage = 0; compares = 0; match_at = match; rand_value = roll;
    std::memset(object, 0x5a, sizeof object);
    unsigned char point[12] = {0};
    char* args[] = {nullptr};
    char name[] = "somename";
    const unsigned r = norrath_construct_Gnome(object, point, static_cast<short>(-4), args, name);
    expect(stage == 9 && seen_name == name && seen_args == args && seen_type == -4, "all calls and arguments");
    expect(seen_map == norrath_ctors_ant_lantern_view_42ba00 && seen_scale == 10.0f, "map and scale");
    return r;
}
int main() {
    norrath_ctors_ant_lantern::host_note(model);
    norrath_ctors_ant_lantern::host_note(chunk_model);
    norrath_ctors_ant_lantern::host_note(header);
    const float p0 = 2.5f, p1 = 4.5f;
    std::memcpy(norrath_ctors_ant_lantern_view_480b14, &p0, 4);
    std::memcpy(norrath_ctors_ant_lantern_view_480b10, &p1, 4);
    unsigned r = run(7, 0);
    expect(r == 0xf1f1u && compares == 3 && word_at(object + 964) == 1, "no name match keeps eRand % 3");
    expect(params[0] == p0 && params[1] == p1 && params[2] == 1.0f && params[3] == 1.0f && params[4] == 1.0f && param_flag == 1, "parameters");
    expect(word_at(object + 168) == fb(p1) && word_at(object + 72) == bits(norrath_ctors_ant_lantern_view_44b9f0) && word_at(object + 248) == 0x04030201u, "globals and vtable");
    expect(chunk_arg == chunk_model && chunk_class == 9, "chunks use model+12 and word 244");
    expect(word_at(object + 552) == 0x55667788u && word_at(object + 556) == 0x11223344u, "64-bit store at 552");
    expect(add_header == header && add_flag == 1 && add_state == object + 288 && init_state == object + 288, "animation");
    run(8, 1);
    expect(compares == 1 && word_at(object + 964) == 0, "first name -> 0");
    run(8, 2);
    expect(compares == 2 && word_at(object + 964) == 1, "second name -> 1");
    run(7, 3);
    expect(compares == 3 && word_at(object + 964) == 2, "third name -> 2");
    if (fails != 0) return 1;
    std::puts("gnome constructor passed");
    return 0;
}
