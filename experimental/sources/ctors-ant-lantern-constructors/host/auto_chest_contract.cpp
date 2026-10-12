#include "../detail.hpp"
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <strings.h>
static int fails;
static void expect(bool cond, const char* message) {
    if (!cond) { std::printf("FAIL %s\n", message); ++fails; }
}
static int stage;
static unsigned char object[300];
static unsigned char model[16];
static unsigned rolls[3];
static int roll_calls;
static float ramp_value;
static bool has_mode, has_scale;
static const char* mode_text;
static float scale_value;
static int int_tag;
static void* seen_name; static char** seen_args; static short seen_type; static unsigned char seen_point[12];
static char lump_text[64]; static void* lump_name_arg;
static void* chunk_arg; static int chunk_class;
static unsigned word_at(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static unsigned half_at(const unsigned char* p) { return unsigned(p[0]) | (unsigned(p[1]) << 8); }
static unsigned bits(const void* p) { return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(p)); }
unsigned char norrath_ctors_ant_lantern_view_44ece8[4], norrath_ctors_ant_lantern_view_46fd38[16] = "special";
unsigned char norrath_ctors_ant_lantern_view_4854c8[8], norrath_ctors_ant_lantern_view_4854f0[8], norrath_ctors_ant_lantern_view_4854f8[8];
unsigned char norrath_ctors_ant_lantern_view_4854d0[16] = "mode0", norrath_ctors_ant_lantern_view_4854d8[16] = "mode1",
    norrath_ctors_ant_lantern_view_4854e0[16] = "mode2", norrath_ctors_ant_lantern_view_4854e8[16] = "flagged",
    norrath_ctors_ant_lantern_view_485500[16] = ".sfx";
unsigned char norrath_ctors_ant_lantern_view_4844c0[4];
extern "C" unsigned norrath_construct_AutoObject(void* o, char* name, const void* point, short type, char** args) {
    expect(stage == 0 && o == object, "AutoObject first");
    stage = 1; seen_name = name; seen_args = args; seen_type = type;
    std::memcpy(seen_point, point, 12);
    const_cast<unsigned char*>(static_cast<const unsigned char*>(point))[0] = 0xee;  // must not matter to later stores
    std::memset(object + 12, 0, 4);
    const unsigned m = bits(model);
    std::memcpy(object + 144, &m, 4);
    std::memcpy(object + 76, "snd", 4);
    return 0;
}
extern "C" char* norrath_actor_find_tag_string(void*, char* tag, int occurrence) {
    expect(occurrence == 0, "string tag occurrence");
    if (tag == reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_4854c8)) return has_mode ? const_cast<char*>(mode_text) : nullptr;
    expect(tag == reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_4854f8), "scale tag text");
    return has_scale ? tag : nullptr;
}
extern "C" float norrath_player_find_tag_float(void*, char*, int) { expect(has_scale, "float tag only with its string"); return scale_value; }
extern "C" int norrath_game_object_find_tag_int(void*, char* tag, int) { expect(tag == reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_4854f0), "int tag text"); return int_tag; }
extern "C" int norrath_factory_strcasecmp(const char* l, const char* r) { return ::strcasecmp(l, r); }
extern "C" unsigned norrath_actor_random(void*) { return rolls[roll_calls++]; }
extern "C" float norrath_player_treasure_ramp() { return ramp_value; }
extern "C" void* norrath_lump_find_resource(char* lump, char* res) {
    lump_name_arg = lump;
    std::strcpy(lump_text, res);
    return model + 4;
}
extern "C" unsigned long long norrath_actor_model_active_chunks(void* m, int cls) {
    chunk_arg = m; chunk_class = cls;
    return 0xaabbccdd11223344ull;
}
static void run(const char* name_text, unsigned r0, unsigned r1, unsigned r2, float ramp, bool mode, const char* mode_value, bool scale, float scale_v, int tag, unsigned level_flags) {
    stage = 0; roll_calls = 0; rolls[0] = r0; rolls[1] = r1; rolls[2] = r2; ramp_value = ramp;
    has_mode = mode; mode_text = mode_value; has_scale = scale; scale_value = scale_v; int_tag = tag;
    std::memcpy(norrath_ctors_ant_lantern_view_4844c0, &level_flags, 4);
    std::memset(object, 0x5a, sizeof object);
    unsigned char point[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    char* args[] = {nullptr};
    static char name[32];
    std::strcpy(name, name_text);
    expect(norrath_construct_AutoChest(object, name, point, static_cast<short>(-3), args) == 0u, "returns zero");
    expect(seen_name == name && seen_args == args && seen_type == -3 && std::memcmp(seen_point, point, 12) == 0, "AutoObject arguments");
}
int main() {
    norrath_ctors_ant_lantern::host_note(model);
    run("plain", 1999, 5, 20000, 1.0f, false, "", false, 1.0f, 0, 0);
    expect(word_at(object + 12) == 0x00100018u && word_at(object + 72) == bits(norrath_ctors_ant_lantern_view_44ece8), "flags and vtable");
    expect(word_at(object + 232) == 0 && object[240] == 0 && word_at(object + 236) == 0, "clears");
    // byte 228 = 1 from the first roll; amount = 17 -> class 1, but byte 228 set -> half 220 cleared, class 8
    expect(object[228] == 1 && half_at(object + 220) == 0 && chunk_class == 8, "byte 228 forces class 8");
    expect(std::strcmp(lump_text, "snd.sfx") == 0 && lump_name_arg == object + 109, "lump lookup text and name argument");
    expect(word_at(object + 244) == bits(model + 4) && chunk_arg == model, "lookup result and chunk model");
    expect(word_at(object + 200) == 0x11223344u && word_at(object + 204) == 0xaabbccddu, "64-bit result at 200");
    run("plain", 2000, 5, 20000, 1.0f, false, "", false, 1.0f, 0, 0);
    expect(object[228] == 0 && half_at(object + 220) == 17 && chunk_class == 1, "17 -> class 1");
    run("plain", 2000, 5, 9999, 1.0f, false, "", false, 1.0f, 0, 0);
    expect(half_at(object + 220) == 0 && chunk_class == 0, "roll below 10000 clears the amount");
    run("plain", 2000, 28, 20000, 2.5f, false, "", false, 1.0f, 0, 0);
    expect(half_at(object + 220) == 40 && chunk_class == 2, "40 -> class 2");
    run("plain", 2000, 11, 20000, 1.0f, false, "", true, 2.0f, 0, 0x10000u);
    expect(half_at(object + 220) == 92 && chunk_class == 4, "doubled, 92 -> class 4");
    run("plain", 2000, 11, 20000, 1.0f, false, "", true, 1.0f, 0, 0x8000u);
    expect(half_at(object + 220) == 69, "tripled");
    run("special", 1, 5, 20000, 1.0f, false, "", false, 1.0f, 0, 0);
    expect(object[228] == 0 && half_at(object + 220) == 17 && chunk_class == 1, "special name clears byte 228 only");
    run("plain", 2000, 5, 20000, 1.0f, false, "", false, 1.0f, 3, 0);
    expect(object[228] == 1 && chunk_class == 8, "int tag sets byte 228");
    run("flagged", 2000, 5, 20000, 1.0f, false, "", false, 1.0f, 0, 0);
    expect(word_at(object + 236) == 1, "flagged name sets word 236");
    run("plain", 2000, 5, 20000, 1.0f, true, "MODE0", false, 1.0f, 0, 0);
    expect(object[240] == 0, "first mode text");
    run("plain", 2000, 5, 20000, 1.0f, true, "Mode1", false, 1.0f, 0, 0);
    expect(object[240] == 1, "second mode text, case-insensitive");
    run("plain", 2000, 5, 20000, 1.0f, true, "mode2", false, 1.0f, 0, 0);
    expect(object[240] == 2, "third mode text");
    run("plain", 2000, 5, 20000, 1.0f, true, "other", false, 1.0f, 0, 0);
    expect(object[240] == 0, "no mode match leaves 0");
    if (fails != 0) return 1;
    std::puts("auto chest constructor passed");
    return 0;
}
