#include "../detail.hpp"
#include <cstdio>
#include <cstdint>
#include <cstring>
static int fails;
static void expect(bool cond, const char* message) {
    if (!cond) { std::printf("FAIL %s\n", message); ++fails; }
}
static int stage;
static unsigned char object[1280];
static unsigned char model[128];
static unsigned char header[16];
static unsigned char light_copy[48];
static unsigned char seen_point[12];
static float seen_params[5];
static int seen_param_flag;
static void* seen_light;
static void* init_state;
static void* add_state; static void* add_header; static int add_flag;
static void* random_state;
static unsigned word_at(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static unsigned half_at(const unsigned char* p) { return unsigned(p[0]) | (unsigned(p[1]) << 8); }
static unsigned bits(const void* p) { return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(p)); }
static unsigned float_bits(float f) { unsigned b; std::memcpy(&b, &f, 4); return b; }
extern "C" unsigned norrath_construct_FireBeetle(void* o, const void* point, short type, char** args, int flag) {
    expect(stage == 0 && flag == 0 && type == -5 && args == nullptr && o == object, "FireBeetle first with flag 0");
    stage = 1;
    std::memcpy(seen_point, point, 12);
    unsigned char* raw = const_cast<unsigned char*>(static_cast<const unsigned char*>(point));
    raw[0] = 0xab;
    const unsigned flags = 0x100u, w160 = 2u, m = bits(model);
    std::memcpy(object + 160, &w160, 4);
    std::memcpy(object + 36, &m, 4);
    (void)flags;
    return 0;
}
extern "C" void norrath_light_add(void* light) {
    expect(stage == 1 && light == object + 1064, "light after FireBeetle");
    stage = 2;
    seen_light = light;
    std::memcpy(light_copy, light, sizeof light_copy);
}
extern "C" unsigned norrath_actor_set_parameters(void*, float a, float b, float c, float d, float e, int f) {
    expect(stage == 2, "parameters after light");
    stage = 3;
    seen_params[0] = a; seen_params[1] = b; seen_params[2] = c; seen_params[3] = d; seen_params[4] = e; seen_param_flag = f;
    return 0;
}
extern "C" void norrath_actor_animation_initialize(void* state) {
    expect(stage == 3, "animation init after parameters");
    stage = 4; init_state = state;
}
extern "C" void norrath_actor_animation_add(void* state, void* h, int flags) {
    expect(stage == 4, "animation add after init");
    stage = 5; add_state = state; add_header = h; add_flag = flags;
}
extern "C" void norrath_actor_animation_randomize_time(void* state) {
    expect(stage == 5, "randomize after add");
    stage = 6; random_state = state;
}
extern "C" unsigned norrath_actor_add_slow_run_list(void*) {
    expect(stage == 6, "slow list after randomize");
    stage = 7;
    expect(word_at(object + 28) == float_bits(12.0f) && word_at(object + 224) == float_bits(0.5f), "singles stored before the slow list");
    return 0;
}
extern "C" unsigned norrath_actor_finish_construction(void* o) {
    expect(stage == 7 && o == object, "finish last");
    stage = 8;
    expect(half_at(object + 1082) == 51 && half_at(object + 1084) == 39 && half_at(object + 1086) == 18 && half_at(object + 1088) == 0, "halves before finish");
    return 0xf1f1u;
}
unsigned char norrath_ctors_ant_lantern_view_44bdb0[4];
unsigned char norrath_ctors_ant_lantern_view_481540[4];
unsigned char norrath_ctors_ant_lantern_view_481544[4];
unsigned char norrath_actor_view_4844a4[4] = {9, 8, 7, 6};
int main() {
    const float tint = 0.75f, first = 3.5f;
    std::memcpy(norrath_ctors_ant_lantern_view_481540, &tint, 4);
    std::memcpy(norrath_ctors_ant_lantern_view_481544, &first, 4);
    const unsigned hb = bits(header);
    std::memcpy(model + 68, &hb, 4);
    norrath_ctors_ant_lantern::host_note(model);
    norrath_ctors_ant_lantern::host_note(header);
    std::memset(object, 0x5a, sizeof object);
    unsigned char point[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    const unsigned r = norrath_construct_FireFly(object, point, static_cast<short>(-5), nullptr);
    expect(r == 0xf1f1u && stage == 8, "finish result");
    expect(seen_light == object + 1064, "light");
    const unsigned char* l = light_copy;
    expect(l[8] == 0 && l[9] == 0 && half_at(l + 16) == 0 && half_at(l + 24) == 0, "light zero fields");
    expect(word_at(l + 28) == float_bits(120.0f) && word_at(l + 36) == float_bits(120.0f) && word_at(l + 44) == float_bits(1.0f), "light singles");
    expect(word_at(l + 32) == float_bits(tint) && word_at(l + 40) == float_bits(tint), "light tint twice");
    expect(half_at(l + 10) == 200 && half_at(l + 12) == 150 && half_at(l + 14) == 50 &&
           half_at(l + 18) == 200 && half_at(l + 20) == 150 && half_at(l + 22) == 50, "light colours");
    expect(object[1112] == 0xab && std::memcmp(object + 1113, point + 1, 11) == 0, "mutated snapshot copied to 1112");
    expect(seen_params[0] == first && seen_params[1] == 1.0f && seen_params[2] == 2.0f && seen_params[3] == 1.0f &&
           seen_params[4] == 1.0f && seen_param_flag == 1, "parameters");
    expect(word_at(object + 72) == bits(norrath_ctors_ant_lantern_view_44bdb0), "vtable");
    expect(word_at(object + 176) == float_bits(160.0f) && word_at(object + 168) == float_bits(20.0f), "160 and 20");
    expect(word_at(object + 248) == 0x06070809u && word_at(object + 160) == 0x00101002u, "global word and flag bits");
    expect(half_at(object + 572) == 1 && object[172] == 18 && word_at(object + 256) == 360 && word_at(object + 236) == 0, "small stores");
    expect(init_state == object + 288 && add_state == object + 288 && random_state == object + 288, "animation state");
    expect(add_header == header && add_flag == 1, "animation header from model+68");
    if (fails != 0) return 1;
    std::puts("fire fly constructor passed");
    return 0;
}
