#include "../detail.hpp"
#include <cstdarg>
#include <cstdio>
#include <cstdint>
#include <cstring>
static int fails;
static void expect(bool cond, const char* message) {
    if (!cond) { std::printf("FAIL %s\n", message); ++fails; }
}
#define VIEW(name, size) unsigned char norrath_ctors_ant_lantern_view_##name[size]
VIEW(4494c0, 4); VIEW(476b70, 16); VIEW(486038, 16); VIEW(486040, 16); VIEW(476b80, 16) = "hurtname"; VIEW(476b90, 16) = "gatename"; VIEW(486048, 16);
VIEW(486050, 16) = "%s.A"; VIEW(486058, 16) = "%s.B"; VIEW(486060, 16) = "%s.C"; VIEW(48239c, 4);
VIEW(486030, 4); VIEW(486da4, 4); VIEW(484538, 4); VIEW(483e5c, 4); VIEW(484f38, 32); VIEW(484ca8, 4); VIEW(429410, 12 * 8);
static unsigned char object[400];
static unsigned char blocks[2][8];
static int stage;
static void* seen_map; static char** seen_args; static unsigned char local_after[12];
static unsigned char light_copy[48]; static int light_calls;
static int icon_calls; static unsigned char icon_point[12]; static char* icon_text; static int icon_a, icon_b; static char *icon_p3, *icon_p4, *icon_p5, *icon_p6;
static int deactivate_calls, activate_calls, run_list_calls;
static int tag_cut, tag_flag; static const char* tag_string; static bool has_name_tag;
static char lump_texts[2][64]; static int lump_calls; static void* lump_arg[2];
static int find_calls; static unsigned char find_points[8][12]; static int find_args[8][2]; static char* find_third[8];
static const unsigned char py_expected[8] = {0, 0, 0xa4, 0x41, 0, 0, 0xf4, 0x41};
static unsigned word_at(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static unsigned half_at(const unsigned char* p) { return unsigned(p[0]) | (unsigned(p[1]) << 8); }
static unsigned bits(const void* p) { return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(p)); }
static unsigned fb(float f) { unsigned b; std::memcpy(&b, &f, 4); return b; }
extern "C" unsigned norrath_game_object_construct(void*, const void* point, void* map, char** args) {
    expect(stage == 0 && point != nullptr, "game object first");
    stage = 1; seen_map = map; seen_args = args;
    std::memset(object + 12, 0, 4);
    const float x = 1.5f, y = 2.5f, z = 3.5f;
    std::memcpy(object + 16, &x, 4); std::memcpy(object + 20, &y, 4); std::memcpy(object + 24, &z, 4);
    const_cast<unsigned char*>(static_cast<const unsigned char*>(point))[0] = 0x00;
    return 0;
}
extern "C" void norrath_light_add(void* light) {
    expect(stage == 1 && light == object + 256, "light after game object");
    stage = 2; ++light_calls;
    std::memcpy(light_copy, light, sizeof light_copy);
}
extern "C" int norrath_game_object_find_tag_int(void*, char* tag, int) {
    if (tag == reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_476b70)) return tag_cut;
    if (tag == reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_486040)) return tag_flag;
    expect(false, "unknown int tag");
    return 0;
}
extern "C" char* norrath_actor_find_tag_string(void*, char* tag, int) {
    expect(tag == reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_486048), "name tag");
    return has_name_tag ? const_cast<char*>(tag_string) : nullptr;
}
extern "C" void addIcon__3mapP6Point3iPciT3N33(void* p, int a, char* text, int b, char* c, char* d, char* e, char* f) {
    ++icon_calls; std::memcpy(icon_point, p, 12); icon_a = a; icon_text = text; icon_b = b; icon_p3 = c; icon_p4 = d; icon_p5 = e; icon_p6 = f;
}
extern "C" void lightDeactivate__FP5Light(void* l) { ++deactivate_calls; expect(l == object + 256, "deactivate light"); }
extern "C" void lightActivate__FP5Light(void* l) { ++activate_calls; expect(l == object + 256, "activate light"); }
extern "C" unsigned norrath_actor_add_run_list(void*) { ++run_list_calls; return 0; }
extern "C" int norrath_factory_sprintf(char* dst, const char* fmt, ...) {
    va_list ap; va_start(ap, fmt);
    const int n = std::vsnprintf(dst, 128, fmt, ap);
    va_end(ap);
    return n;
}
extern "C" void* norrath_lump_find_resource(char* lump, char* res) {
    lump_arg[lump_calls] = lump; std::strcpy(lump_texts[lump_calls], res);
    return blocks[lump_calls++];
}
extern "C" void objectFindEmptySpot__FG6Point3iiPc(void* result, const void* point, int a, int b, char* c) {
    const int k = find_calls++;
    std::memcpy(find_points[k], point, 12); find_args[k][0] = a; find_args[k][1] = b; find_third[k] = c;
    for (int i = 0; i < 12; ++i) static_cast<unsigned char*>(result)[i] = static_cast<unsigned char>(0x10 * (k + 1) + i);
}
static void set_word(unsigned char* p, unsigned v) { std::memcpy(p, &v, 4); }
static void run(short type) {
    stage = 0; light_calls = icon_calls = deactivate_calls = activate_calls = run_list_calls = lump_calls = find_calls = 0;
    std::memset(object, 0x5a, sizeof object);
    unsigned char point[12];
    const float px = 10.5f, py = 20.5f, pz = 30.5f;
    std::memcpy(point, &px, 4); std::memcpy(point + 4, &py, 4); std::memcpy(point + 8, &pz, 4);
    char* args[] = {nullptr};
    seen_args = nullptr;
    expect(norrath_construct_JumpGate(object, point, type, args) == 0u, "returns zero");
    expect(seen_map == nullptr && seen_args == args, "game object map and arguments");
}
int main() {
    norrath_ctors_ant_lantern::host_note(blocks[0]);
    norrath_ctors_ant_lantern::host_note(blocks[1]);
    const float tint = 0.75f;
    std::memcpy(norrath_ctors_ant_lantern_view_48239c, &tint, 4);
    set_word(norrath_ctors_ant_lantern_view_486030, 0);
    tag_cut = 0; tag_flag = 0; has_name_tag = false;
    run(0x21);
    expect(word_at(object + 72) == bits(norrath_ctors_ant_lantern_view_4494c0) && object[221] == 1 && object[40] == 72 && word_at(object + 12) == 0x10008u, "vtable, byte 221, flags");
    const unsigned char* l = light_copy;
    expect(l[8] == 0 && l[9] == 0 && half_at(l + 10) == 140 && half_at(l + 12) == 140 && half_at(l + 18) == 140 && half_at(l + 20) == 140 &&
           half_at(l + 14) == 300 && half_at(l + 22) == 300 && half_at(l + 16) == 0xfff0 && half_at(l + 24) == 0xfff0, "light halves at lightAdd");
    expect(word_at(l + 28) == fb(240.0f) && word_at(l + 36) == fb(240.0f) && word_at(l + 32) == fb(tint) && word_at(l + 40) == fb(tint) && word_at(l + 44) == 0, "light singles");
    expect(word_at(object + 304) == fb(10.5f) && word_at(object + 308) == fb(20.5f) && word_at(object + 312) == fb(30.5f + 84.0f), "lifted position at +304");
    expect(icon_calls == 1 && icon_a == 0 && icon_b == 0 && icon_text == reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_486038) &&
           icon_p3 == nullptr && icon_p4 == nullptr && icon_p5 == nullptr && icon_p6 == nullptr && icon_point[0] == 0x00 && std::memcmp(icon_point + 4, &py_expected, 8) == 0, "icon without the cut tag");
    expect(half_at(object + 56) == 0x21 && object[77] == 0 && object[78] == 0 && object[208] == 0 && object[79] == 0, "small stores");
    expect(deactivate_calls == 1 && run_list_calls == 1 && activate_calls == 0 && word_at(object + 212) == 0x5a5a5a5au, "deactivate and run list");
    expect(std::memcmp(object + 80, "gatename", 9) == 0, "zero byte 79 copies the second text");
    expect(std::strcmp(reinterpret_cast<char*>(object + 144), "gatename.A") == 0 && std::strcmp(lump_texts[0], "gatename.B") == 0 && std::strcmp(lump_texts[1], "gatename.C") == 0, "formatted names");
    expect(lump_arg[0] == object + 144 && lump_arg[1] == object + 144 && word_at(object + 348) == bits(blocks[0]) && word_at(object + 352) == bits(blocks[1]), "lump lookups");
    expect(find_calls == 0, "no spot search with the flag word zero");
    // cut tag skips the icon; flag selects the other text and the light
    tag_cut = 1; tag_flag = 0x7f;
    run(0x21);
    expect(icon_calls == 0 && half_at(object + 56) == 0x21, "cut tag skips addIcon");
    expect(object[79] == 0x7f && std::memcmp(object + 80, "hurtname", 9) == 0 && activate_calls == 1 && word_at(object + 212) == fb(60.0f), "flag copies the first text and activates");
    expect(std::strcmp(lump_texts[0], "hurtname.B") == 0, "format uses the copied name");
    // a negative flag byte counts as nonzero too
    tag_flag = 0x80;
    run(1);
    expect(object[79] == 0x80 && activate_calls == 1, "byte flag");
    tag_flag = 0;
    // name tag replaces the text
    has_name_tag = true; tag_string = "custom";
    run(1);
    expect(std::strcmp(reinterpret_cast<char*>(object + 80), "custom") == 0 && std::strcmp(lump_texts[0], "custom.B") == 0, "name tag replaces the name");
    has_name_tag = false;
    // global gates
    set_word(norrath_ctors_ant_lantern_view_486030, 1);
    set_word(norrath_ctors_ant_lantern_view_486da4, 1);
    set_word(norrath_ctors_ant_lantern_view_484538, 3);
    norrath_ctors_ant_lantern_view_484ca8[0] = 0;
    run(1);
    expect(find_calls == 0 && norrath_ctors_ant_lantern_view_484ca8[0] == 0, "second global set exits before the search");
    set_word(norrath_ctors_ant_lantern_view_486da4, 0);
    set_word(norrath_ctors_ant_lantern_view_484538, 0);
    run(1);
    expect(find_calls == 0 && norrath_ctors_ant_lantern_view_484ca8[0] == 3, "count zero only sets the byte");
    set_word(norrath_ctors_ant_lantern_view_484538, 3);
    set_word(norrath_ctors_ant_lantern_view_483e5c, 1);
    std::memset(norrath_ctors_ant_lantern_view_429410, 0xaa, 12 * 8);
    std::memset(norrath_ctors_ant_lantern_view_484f38, 0xbb, 32);
    norrath_ctors_ant_lantern_view_484ca8[0] = 0;
    run(1);
    expect(find_calls == 3 && norrath_ctors_ant_lantern_view_484ca8[0] == 3, "three searches and the byte");
    for (int k = 0; k < 3; ++k) {
        expect(std::memcmp(find_points[k], object + 16, 12) == 0 && find_args[k][0] == 36 && find_args[k][1] == 24 && find_third[k] == nullptr, "search arguments");
        for (int i = 0; i < 12; ++i) expect(norrath_ctors_ant_lantern_view_429410[12 * k + i] == 0x10 * (k + 1) + i, "result stored in the table");
    }
    expect(norrath_ctors_ant_lantern_view_429410[36] == 0xaa, "table entry 3 untouched");
    unsigned state = 1u;
    for (int k = 0; k < 3; ++k) {
        state = state * 0x19660du + 0x3c6ef35fu;
        expect(half_at(norrath_ctors_ant_lantern_view_484f38 + 2 * k) == (state >> 16), "generator high half");
    }
    expect(word_at(norrath_ctors_ant_lantern_view_483e5c) == state && norrath_ctors_ant_lantern_view_484f38[6] == 0xbb, "generator state saved, fourth half untouched");
    if (fails != 0) return 1;
    std::puts("jump gate constructor passed");
    return 0;
}
