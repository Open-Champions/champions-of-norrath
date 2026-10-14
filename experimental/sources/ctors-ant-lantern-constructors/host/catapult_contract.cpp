#include "../detail.hpp"
#include <cstdio>
#include <cstdint>
#include <cstring>
static int fails;
static void expect(bool cond, const char* message) {
    if (!cond) { std::printf("FAIL %s\n", message); ++fails; }
}
#define VIEW(name, size) unsigned char norrath_ctors_ant_lantern_view_##name[size]
VIEW(438eb8, 4); VIEW(4506b0, 4); VIEW(4777c8, 16); VIEW(4860e0, 16); VIEW(4860e8, 16); VIEW(4860f0, 16); VIEW(4860f8, 16);
VIEW(477798, 16); VIEW(4777d8, 16); VIEW(4825bc, 4); VIEW(4825c0, 4);
static unsigned char object[400];
static unsigned char blocks[40][100];
static int block_calls; static int null_block = -1;
static int stage;
static bool has_speed, has_delay, has_flag, tag_cut;
static int speed_int, delay_int, flag_int;
static char hide_text[] = "hide";
static bool has_hide;
static int run_list_calls, anim_init_calls, anim_add_calls, link_calls;
static void* anim_state; static void* anim_header; static int anim_flag;
static void* seen_map; static char** seen_args;
static void* lump_a; static void* lump_b;
static int blocker_calls; static short blocker_type[40]; static char** blocker_args[40]; static unsigned char blocker_origin[40][12];
static void* link_head[40]; static void* link_a[40]; static void* link_b[40];
static unsigned word_at(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static unsigned half_at(const unsigned char* p) { return unsigned(p[0]) | (unsigned(p[1]) << 8); }
static unsigned bits(const void* p) { return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(p)); }
static unsigned fb(float f) { unsigned b; std::memcpy(&b, &f, 4); return b; }
static float fl(const unsigned char* p) { float f; std::memcpy(&f, p, 4); return f; }
extern "C" unsigned norrath_game_object_construct(void* o, const void* point, void* map, char** args) {
    expect(stage == 0 && o == object && point != nullptr, "game object first");
    stage = 1; seen_map = map; seen_args = args;
    std::memset(object + 12, 0xcc, 8);
    std::memcpy(object + 16, "\x00\x00\xa0\x40\x00\x00\xc0\x40\x00\x00\xe0\x40", 12);   // 5.0, 6.0, 7.0
    return 0;
}
extern "C" char* norrath_actor_find_tag_string(void*, char* tag, int) {
    if (tag == reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_4777c8)) return has_speed ? tag : nullptr;
    if (tag == reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_4860e0)) return has_delay ? tag : nullptr;
    if (tag == reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_4860e8)) return has_hide ? hide_text : nullptr;
    if (tag == reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_4860f0)) return has_flag ? tag : nullptr;
    expect(false, "unknown string tag");
    return nullptr;
}
extern "C" int norrath_game_object_find_tag_int(void*, char* tag, int) {
    if (tag == reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_4777c8)) return speed_int;
    if (tag == reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_4860e0)) return delay_int;
    if (tag == reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_4860f0)) return flag_int;
    if (tag == reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_4860f8)) return tag_cut ? 1 : 0;
    expect(false, "unknown int tag");
    return 0;
}
extern "C" unsigned norrath_actor_add_run_list(void*) { ++run_list_calls; return 0; }
extern "C" void norrath_actor_animation_initialize(void* s) { ++anim_init_calls; anim_state = s; }
extern "C" void* norrath_lump_find_resource(char* a, char* b) { lump_a = a; lump_b = b; return blocks[39]; }
extern "C" void norrath_actor_animation_add(void* s, void* h, int f) { ++anim_add_calls; anim_state = s; anim_header = h; anim_flag = f; }
extern "C" float icos__Fi(int angle) { return 0.125f * static_cast<float>(angle % 100); }
extern "C" float isin__Fi(int angle) { return 0.0625f * static_cast<float>(angle % 50); }
extern "C" unsigned char* norrath_block_alloc(int size) {
    expect(size == 76, "block size");
    const int k = block_calls++;
    if (k == null_block) return nullptr;
    return blocks[k];
}
extern "C" unsigned norrath_construct_Blocker(void* b, const void* point, short type, char** args) {
    const int k = blocker_calls++;
    blocker_type[k] = type; blocker_args[k] = args;
    std::memcpy(blocker_origin[k], point, 12);
    std::memcpy(static_cast<unsigned char*>(b) + 16, point, 12);
    return 0;
}
extern "C" void* norrath_list_oneway_link(void* head, void* a, void* b) { const int k = link_calls++; link_head[k] = head; link_a[k] = a; link_b[k] = b; return nullptr; }
static void run(short type) {
    stage = 0; block_calls = blocker_calls = link_calls = 0; run_list_calls = anim_init_calls = anim_add_calls = 0;
    std::memset(object, 0x5a, sizeof object);
    for (auto& b : blocks) std::memset(b, 0, sizeof b);
    unsigned char point[12] = {0};
    char* args[] = {nullptr};
    seen_args = nullptr;
    expect(norrath_construct_Catapult(object, point, type, args) == 0u, "returns zero");
    expect(seen_map == norrath_ctors_ant_lantern_view_438eb8 && seen_args == args, "game object map and arguments");
}
int main() {
    for (auto& b : blocks) norrath_ctors_ant_lantern::host_note(b);
    const float sp = 2.0f, sd = 3.0f;
    std::memcpy(norrath_ctors_ant_lantern_view_4825bc, &sp, 4);
    std::memcpy(norrath_ctors_ant_lantern_view_4825c0, &sd, 4);
    // tags absent
    run(0x10);
    expect(word_at(object + 72) == bits(norrath_ctors_ant_lantern_view_4506b0) && object[97] == 4 && word_at(object + 248) == 0, "vtable, byte 97 and the list head");
    expect(object[40] == 72 && half_at(object + 56) == 0x10 && word_at(object + 28) == 0 && word_at(object + 12) == 0 && word_at(object + 88) == 0, "stores");
    expect(object[85] == 0 && object[84] == 0 && object[78] == 0 && object[87] == 0, "byte clears");
    expect(word_at(object + 240) == fb(sd) && word_at(object + 244) == 0 && word_at(object + 80) == 0 && object[86] == 0, "defaults");
    expect(run_list_calls == 1 && anim_init_calls == 1 && anim_state == object + 92 && anim_add_calls == 1 && anim_flag == 1 && anim_header == blocks[39], "run list and animation");
    expect(lump_a == norrath_ctors_ant_lantern_view_477798 && lump_b == norrath_ctors_ant_lantern_view_4777d8, "lump names");
    // blocks: 8 columns, rows -1..1; middle rows skipped for columns -2..3 -> 2*3 + 6*2 = 18 blocks
    expect(block_calls == 18 && blocker_calls == 18 && link_calls == 18, "eighteen blocks");
    {
        int k = 0;
        for (int i = -3; i < 5; ++i) {
            for (int j = -1; j < 2; ++j) {
                if (i >= -2 && i <= 3 && j == 0) continue;
                const float c = 0.125f * static_cast<float>(0x10 % 100), s = 0.0625f * static_cast<float>(0x10 % 50);
                const float c2 = 0.125f * static_cast<float>((0x10 + 16384) % 100), s2 = 0.0625f * static_cast<float>((0x10 + 16384) % 50);
                const float x = 5.0f + (c * (static_cast<float>(i) * 22.5f) + c2 * (static_cast<float>(j) * 22.5f));
                const float y = 6.0f + (s * (static_cast<float>(i) * 22.5f) + s2 * (static_cast<float>(j) * 22.5f));
                expect(word_at(blocks[k] + 16) == fb(x) && word_at(blocks[k] + 20) == fb(y) && word_at(blocks[k] + 24) == fb(7.0f), "block position");
                expect(word_at(blocks[k] + 28) == fb(i >= 0 ? 30.0f : 38.0f) && (word_at(blocks[k] + 12) & 0x4000u) == 0x4000u, "height and flag");
                expect(blocker_type[k] == 0x10 && blocker_args[k] == nullptr && link_head[k] == object + 248 && link_a[k] == blocks[k] && link_b[k] == blocks[k], "blocker and link arguments");
                expect(std::memcmp(blocker_origin[k], object + 16, 12) == 0, "blocker origin is object+16");
                ++k;
            }
        }
        expect(k == 18, "expected count");
    }
    // tags present
    has_speed = has_delay = has_flag = has_hide = true;
    speed_int = 4; delay_int = 5; flag_int = 0;
    run(0);
    expect(word_at(object + 240) == fb(4.0f * sp) && word_at(object + 244) == 300 && word_at(object + 80) == bits(hide_text) && object[86] == 0, "tags");
    expect(block_calls == 18, "flag zero still builds blocks");
    // cut tag: clears 244 and skips the lump animation
    tag_cut = true;
    run(0);
    expect(word_at(object + 244) == 0 && anim_add_calls == 0, "cut tag");
    tag_cut = false;
    // nonzero flag byte returns before the blocks
    flag_int = 0x101;
    run(0);
    expect(object[86] == 1 && block_calls == 0 && link_calls == 0 && anim_add_calls == 1, "flag byte ends the constructor");
    flag_int = 0x80;
    run(0);
    expect(object[86] == 0x80 && block_calls == 0, "negative flag byte ends it too");
    flag_int = 0;
    // allocation failure skips only that block
    null_block = 5;
    run(0);
    expect(block_calls == 18 && blocker_calls == 17 && link_calls == 17, "one failed allocation");
    null_block = -1;
    if (fails != 0) return 1;
    std::puts("catapult constructor passed");
    return 0;
}
