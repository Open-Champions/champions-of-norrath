#include "../detail.hpp"
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <strings.h>
static int fails;
static void expect(bool cond, const char* message) {
    if (!cond) { std::printf("FAIL %s\n", message); ++fails; }
}
unsigned char norrath_ctors_ant_lantern_view_44ebb0[4];
unsigned char norrath_ctors_ant_lantern_view_470ff0[8] = "_a", norrath_ctors_ant_lantern_view_471000[8] = "_b";
unsigned char norrath_ctors_ant_lantern_view_470df8[8], norrath_ctors_ant_lantern_view_471010[8], norrath_ctors_ant_lantern_view_471020[8];
unsigned char norrath_ctors_ant_lantern_view_4854c8[8], norrath_ctors_ant_lantern_view_471150[8];
unsigned char norrath_ctors_ant_lantern_view_485560[16] = "m0", norrath_ctors_ant_lantern_view_485568[16] = "m1",
    norrath_ctors_ant_lantern_view_485570[16] = "m2", norrath_ctors_ant_lantern_view_485578[16] = "m3";
unsigned char norrath_ctors_ant_lantern_view_471030[16 * 18];
unsigned char norrath_ctors_ant_lantern_view_485580[8 * 11];
unsigned char norrath_actors_b_view_4844a0[4];
static unsigned char object[400];
static unsigned char blocks[2][80];
static bool block_null[2];
static int block_calls;
static int stage;
static bool tag_ints[4];     // 470df8, 471010, 471020, 471150
static bool has_gate_string;
static const char* mode_text;
static char lump_texts[2][48]; static int lump_calls; static void* lump_arg;
static int blocker_calls; static void* blocker_object[2]; static float blocker_position[2][3]; static short blocker_type[2]; static char** blocker_args[2];
static int water_result; static float water_radius; static void* water_world; static float* water_output; static unsigned char water_point[12];
static int cos_angle, sin_angle;
static float cos_value = 0.5f, sin_value = 0.25f;
static unsigned word_at(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static unsigned bits(const void* p) { return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(p)); }
static unsigned fb(float f) { unsigned b; std::memcpy(&b, &f, 4); return b; }
extern "C" unsigned norrath_construct_AutoObject(void*, char* name, const void*, short type, char**) {
    std::memset(object + 12, 0, 4);
    std::memset(object + 292, 0, 4);
    std::strcpy(reinterpret_cast<char*>(object + 76), name);
    const float x = 1.0f, y = 2.0f, z = 3.0f;
    std::memcpy(object + 16, &x, 4); std::memcpy(object + 20, &y, 4); std::memcpy(object + 24, &z, 4);
    object[56] = static_cast<unsigned char>(type); object[57] = static_cast<unsigned char>(type >> 8);
    return 0;
}
extern "C" void* norrath_lump_find_resource(char* lump, char* res) {
    lump_arg = lump; std::strcpy(lump_texts[lump_calls], res);
    return lump_calls++ == 0 ? static_cast<void*>(blocks[0]) : static_cast<void*>(blocks[1]);
}
extern "C" int norrath_game_object_find_tag_int(void*, char* tag, int) {
    if (tag == reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_470df8)) return tag_ints[0];
    if (tag == reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_471010)) return tag_ints[1];
    if (tag == reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_471020)) return tag_ints[2];
    if (tag == reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_471150)) return tag_ints[3];
    expect(false, "unknown int tag");
    return 0;
}
extern "C" char* norrath_actor_find_tag_string(void*, char* tag, int) {
    if (tag == reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_4854c8)) return const_cast<char*>(mode_text);
    expect(tag == reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_471150), "gate string tag");
    return has_gate_string ? tag : nullptr;
}
extern "C" int norrath_factory_strcasecmp(const char* l, const char* r) { return ::strcasecmp(l, r); }
extern "C" int strticmp__FPCcT0(const char* l, const char* r) { return std::strcmp(l, r); }
extern "C" int norrath_world_check_for_water(void* world, const void* point, float radius, float* out) {
    water_world = world; water_radius = radius; water_output = out; std::memcpy(water_point, point, 12);
    return water_result;
}
extern "C" float icos__Fi(int a) { cos_angle = a; return cos_value; }
extern "C" float isin__Fi(int a) { sin_angle = a; return sin_value; }
extern "C" unsigned char* norrath_block_alloc(int size) {
    expect(size == 76, "blocker size");
    const int k = block_calls++;
    return block_null[k] ? nullptr : blocks[k];
}
extern "C" unsigned norrath_construct_Blocker(void* o, const void* point, short type, char** args) {
    const int k = blocker_calls++;
    blocker_object[k] = o; blocker_type[k] = type; blocker_args[k] = args;
    std::memcpy(blocker_position[k], point, 12);
    return 0;
}
static void run(const char* name, short type) {
    std::memset(object, 0x5a, sizeof object);
    for (auto& b : blocks) std::memset(b, 0, sizeof b);
    block_calls = blocker_calls = lump_calls = 0;
    unsigned char point[12] = {0};
    char* args[] = {nullptr};
    char nm[24];
    std::strcpy(nm, name);
    expect(norrath_construct_AutoContainer(object, nm, point, type, args) == 0u, "returns zero");
}
int main() {
    norrath_ctors_ant_lantern::host_note(blocks[0]);
    norrath_ctors_ant_lantern::host_note(blocks[1]);
    norrath_ctors_ant_lantern::host_note(object);
    const unsigned wb = bits(object);
    std::memcpy(norrath_actors_b_view_4844a0, &wb, 4);
    for (int k = 0; k < 18; ++k) std::snprintf(reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_471030) + 16 * k, 16, "n%d", k);
    for (int k = 0; k < 11; ++k) std::snprintf(reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_485580) + 8 * k, 8, "p%d", k);
    mode_text = nullptr; water_result = 0x1ff;
    run("plain", 0);
    expect(word_at(object + 72) == bits(norrath_ctors_ant_lantern_view_44ebb0), "vtable");
    expect(std::strcmp(reinterpret_cast<char*>(object + 224), "plain_a") == 0 && std::strcmp(reinterpret_cast<char*>(object + 256), "plain_b") == 0, "copies with suffixes");
    expect(lump_calls == 2 && lump_arg == object + 109 && word_at(object + 216) == bits(blocks[0]) && word_at(object + 220) == bits(blocks[1]), "lump lookups");
    expect(word_at(object + 292) == 0 && object[288] == 0, "no tags, byte 288 zero");
    expect(word_at(object + 12) == 0x10u && word_at(object + 308) == 0xffffffffu && word_at(object + 304) == 0, "flag 0x10, -1, 0");
    expect(word_at(object + 200) == 0xffffffffu && word_at(object + 204) == 0xffffffffu && object[312] == 0xff, "64-bit -1 and water byte");
    expect(water_world == object && water_radius == 12.0f && water_output == nullptr, "water arguments");
    expect(std::memcmp(water_point, object + 16, 12) == 0, "water point is object+16");
    expect(word_at(object + 320) == 0 && word_at(object + 324) == 0 && blocker_calls == 0, "no blockers without the gate");
    // bits in +292 and the 0x10000 flag
    tag_ints[0] = tag_ints[1] = tag_ints[2] = true;
    run("plain", 0);
    expect(word_at(object + 292) == 0xdu, "292 bits 1|4|8");
    expect(word_at(object + 12) == 0x10010u, "bit 4 also ORs 0x10000");
    tag_ints[0] = tag_ints[1] = tag_ints[2] = false;
    // mode texts
    const char* modes[] = {"M0", "M1", "M2", "M3", "other"};
    const int want[] = {0, 2, 2, 1, 0};
    for (int i = 0; i < 5; ++i) {
        mode_text = modes[i];
        run("plain", 0);
        expect(object[288] == want[i], "mode text value");
    }
    mode_text = "m0";
    run("plain", 0);
    expect(object[288] == 0, "case-insensitive first mode");
    mode_text = nullptr;
    // name table: independent expected values
    const unsigned char run0[18] = {0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1};
    const unsigned char run1[11] = {1, 1, 1, 0, 0, 2, 0, 0, 0, 1, 1};
    for (int k = 0; k < 18; ++k) {
        char nm[8]; std::snprintf(nm, sizeof nm, "n%d", k);
        run(nm, 0);
        expect(object[288] == run0[k], "run 0 name value");
    }
    for (int k = 0; k < 11; ++k) {
        char nm[8]; std::snprintf(nm, sizeof nm, "p%d", k);
        run(nm, 0);
        expect(object[288] == run1[k], "run 1 name value");
    }
    // name table overrides the mode text
    mode_text = "M3";
    run("n2", 0);
    expect(object[288] == 2, "table match overrides the mode");
    mode_text = nullptr;
    // blockers
    has_gate_string = true; tag_ints[3] = true;
    run("plain", 0x10);
    expect(cos_angle == 0x4010 && sin_angle == 0x4010, "angle is type + 0x4000");
    expect(blocker_calls == 2 && blocker_object[0] == blocks[0] && blocker_object[1] == blocks[1], "two blockers");
    expect(blocker_position[0][0] == 1.0f + 0.5f * 50.0f && blocker_position[0][1] == 2.0f + 0.25f * 50.0f && blocker_position[0][2] == 3.0f, "first position");
    expect(blocker_position[1][0] == 1.0f + 0.5f * -40.0f && blocker_position[1][1] == 2.0f + 0.25f * -40.0f && blocker_position[1][2] == 3.0f, "second position");
    expect(blocker_type[0] == 0x10 && blocker_type[1] == 0x10 && blocker_args[0] == nullptr, "type half and null arguments");
    expect(word_at(object + 320) == bits(blocks[0]) && word_at(object + 324) == bits(blocks[1]), "pointers stored");
    for (int k = 0; k < 2; ++k) expect(word_at(blocks[k] + 12) == 0x4000u && word_at(blocks[k] + 28) == fb(36.0f), "block flags and 36.0");
    // negative type: sign-extended, angle plus 0x4000
    run("plain", static_cast<short>(-2));
    expect(cos_angle == 0x3ffe && blocker_type[0] == -2, "negative type");
    // allocation failures
    block_null[0] = true;
    run("plain", 0);
    expect(word_at(object + 320) == 0 && word_at(object + 324) == bits(blocks[1]) && blocker_calls == 1, "first allocation fails");
    block_null[0] = false; block_null[1] = true;
    run("plain", 0);
    expect(word_at(object + 320) == bits(blocks[0]) && word_at(object + 324) == 0, "second allocation fails");
    block_null[1] = false;
    // gate with only the string or only the int
    has_gate_string = true; tag_ints[3] = false;
    run("plain", 0);
    expect(blocker_calls == 0 && word_at(object + 320) == 0, "string without int");
    has_gate_string = false; tag_ints[3] = true;
    run("plain", 0);
    expect(blocker_calls == 0 && word_at(object + 324) == 0, "int without string");
    if (fails != 0) return 1;
    std::puts("auto container constructor passed");
    return 0;
}
