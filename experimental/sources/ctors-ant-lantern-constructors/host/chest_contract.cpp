#include "../detail.hpp"
#include <cstdio>
#include <cstdint>
#include <cstring>
static int fails;
static void expect(bool cond, const char* message) {
    if (!cond) { std::printf("FAIL %s\n", message); ++fails; }
}
static int stage;
static unsigned char object[256];
static unsigned char model[64];
static unsigned char marker[8];
static unsigned char chunk_model[8];
static unsigned rand_values[2];
static int rand_calls;
static float ramp_value;
static bool has_scale_tag;
static float scale_value;
static int tag_int_value;
static void* seen_map;
static char** seen_args;
static void* seen_chunk_model;
static int seen_chunk_class;
static unsigned long long chunk_result;
static float seen_scale_float_calls;
static unsigned word_at(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static unsigned half_at(const unsigned char* p) { return unsigned(p[0]) | (unsigned(p[1]) << 8); }
static unsigned bits(const void* p) { return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(p)); }
unsigned char norrath_ctors_ant_lantern_view_42c8a0[64];
unsigned char norrath_ctors_ant_lantern_view_44cb98[4];
unsigned char norrath_ctors_ant_lantern_view_34b6b1[32];
unsigned char norrath_ctors_ant_lantern_view_485398[32] = "alpha";
unsigned char norrath_ctors_ant_lantern_view_485420[32] = "beta";
unsigned char norrath_ctors_ant_lantern_view_485438[8];
unsigned char norrath_ctors_ant_lantern_view_485440[8];
unsigned char norrath_ctors_ant_lantern_view_4844c0[4];
extern "C" unsigned norrath_game_object_construct(void*, const void* point, void* map, char** args) {
    expect(stage == 0 && point != nullptr, "game object first");
    stage = 1; seen_map = map; seen_args = args;
    std::memset(object + 36, 0, 4);
    const unsigned m = bits(model);
    std::memcpy(object + 36, &m, 4);
    std::memset(object + 12, 0, 4);
    return 0;
}
extern "C" unsigned norrath_actor_random(void*) { return rand_values[rand_calls++]; }
extern "C" char* norrath_actor_find_tag_string(void*, char* tag, int occurrence) {
    expect(occurrence == 0 && tag == reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_485438), "scale tag text");
    return has_scale_tag ? tag : nullptr;
}
extern "C" float norrath_player_find_tag_float(void*, char* tag, int occurrence) {
    expect(has_scale_tag && occurrence == 0 && tag == reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_485438), "float tag only when the string exists");
    return scale_value;
}
extern "C" int norrath_game_object_find_tag_int(void*, char* tag, int occurrence) {
    expect(occurrence == 0 && tag == reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_485440), "int tag text");
    return tag_int_value;
}
extern "C" float norrath_player_treasure_ramp() { return ramp_value; }
extern "C" unsigned long long norrath_actor_model_active_chunks(void* m, int cls) {
    expect(stage == 1, "chunks after game object");
    stage = 2; seen_chunk_model = m; seen_chunk_class = cls;
    return chunk_result;
}
extern "C" void norrath_actor_animation_initialize(void* state) {
    expect(stage == 2 && state == object + 92, "animation state last");
    stage = 3;
    expect(word_at(object + 84) == static_cast<unsigned>(chunk_result), "low word of the chunk result stored first");
}
static void run(const char* level, unsigned r0, unsigned r1, float ramp, bool tag, float scale, int tint, unsigned level_flags, int kind) {
    stage = 0; rand_calls = 0; rand_values[0] = r0; rand_values[1] = r1; ramp_value = ramp;
    has_scale_tag = tag; scale_value = scale; tag_int_value = tint;
    std::memset(object, 0x5a, sizeof object);
    std::memcpy(norrath_ctors_ant_lantern_view_34b6b1, level, std::strlen(level) + 1);
    std::memcpy(norrath_ctors_ant_lantern_view_4844c0, &level_flags, 4);
    const unsigned entry = bits(marker);
    std::memcpy(norrath_ctors_ant_lantern_view_42c8a0 + kind * 4, &entry, 4);
    const unsigned chunk = bits(chunk_model);
    std::memcpy(model + 12, &chunk, 4);
    chunk_result = 0xffffffff00000000ull | 0x1234u;
    unsigned char point[12] = {0};
    char* args[] = {nullptr};
    seen_args = nullptr;
    (void)norrath_construct_Chest(object, point, static_cast<short>(-2), args, kind);
    expect(stage == 3 && seen_map == marker && seen_args == args, "table entry as map, arguments forwarded");
    expect(seen_chunk_model == chunk_model, "chunk model is word 12 of the model");
}
int main() {
    norrath_ctors_ant_lantern::host_note(model);
    norrath_ctors_ant_lantern::host_note(marker);
    norrath_ctors_ant_lantern::host_note(chunk_model);
    run("plain", 1999, 5, 1.0f, false, 1.0f, 0, 0, 3);
    expect(object[88] == 1 && object[89] == 3 && object[97] == 1 && half_at(object + 56) == 0xfffe, "random below 2000, kind and type");
    expect(word_at(object + 72) == bits(norrath_ctors_ant_lantern_view_44cb98) && word_at(object + 12) == 0x00100018u, "vtable and flags");
    expect(word_at(object + 28) == 0x41b00000u && word_at(object + 32) == 0x41c80000u, "singles");
    // amount = (5 + 12) * 1.0 * 1.0 = 17 -> class 1, but byte 88 is set -> half 76 cleared and class 8
    expect(half_at(object + 76) == 0 && seen_chunk_class == 8, "byte 88 forces class 8");
    expect(word_at(object + 84) == 0x1234u, "low word of the chunk result");
    run("plain", 2000, 5, 1.0f, false, 1.0f, 0, 0, 1);
    expect(object[88] == 0 && half_at(object + 76) == 17 && seen_chunk_class == 1, "17 -> class 1");
    run("plain", 2000, 28, 1.0f, false, 1.0f, 0, 0, 1);   // 28 % 12 = 4 -> 16; 16 * 2.5 = 40 -> class 2
    run("plain", 2000, 28, 2.5f, false, 1.0f, 0, 0, 1);
    expect(half_at(object + 76) == 40 && seen_chunk_class == 2, "40 -> class 2");
    run("plain", 2000, 11, 1.0f, true, 2.0f, 0, 0x10000u, 1);   // 23 * 2 * 2 = 92 -> class 4
    expect(half_at(object + 76) == 92 && seen_chunk_class == 4, "doubled scale, 92 -> class 4");
    run("plain", 2000, 11, 1.0f, true, 1.0f, 0, 0x8000u, 1);    // 23 * 3 = 69 -> class 2
    expect(half_at(object + 76) == 69 && seen_chunk_class == 2, "tripled scale");
    run("plain", 2000, 0, 0.0f, false, 1.0f, 0, 0, 1);          // zero amount -> class 0
    expect(half_at(object + 76) == 0 && seen_chunk_class == 0, "zero amount, class 0");
    run("alpha", 1, 5, 1.0f, false, 1.0f, 0, 0, 1);
    expect(object[88] == 0 && half_at(object + 76) == 17, "first level text clears byte 88 only");
    run("beta", 1, 5, 1.0f, false, 1.0f, 0, 0, 1);
    expect(object[88] == 0 && half_at(object + 76) == 0 && seen_chunk_class == 0, "second level text clears byte 88 and half 76");
    run("alpha", 1, 5, 1.0f, false, 1.0f, 7, 0, 1);
    expect(object[88] == 1 && half_at(object + 76) == 0 && seen_chunk_class == 8, "int tag sets byte 88 after the level checks");
    if (fails != 0) return 1;
    std::puts("chest constructor passed");
    return 0;
}
