#define NORRATH_VSLOTS_B_HOST 1
#include "msg_get_poke_name.cpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
extern "C" unsigned char player[23984u * 2u] = {};
static int failures = 0;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }

static std::vector<std::string> trace;
static bool has_cone = false;
static float cone_value = 0.5f;
static int shop_tag = 0;
static int in_shop = 0;
static float cos_value = 1.0f, sin_value = 0.0f;
static int cos_angle = 0, sin_angle = 0;
static bool type_equal = false;
static int type_calls = 0, eq_calls = 0;
static void* mummy_marker = reinterpret_cast<void*>(0x1234);
static void* type_marker = reinterpret_cast<void*>(0x5678);
static void* eq_left = nullptr; static void* eq_right = nullptr;
static unsigned short up[2] = {1, 0}, cannot[2] = {2, 0}, talk[2] = {3, 0}, name_text[2] = {4, 0};
static char name_tag[] = "Bob";
static std::string last_key;
static unsigned short* printf_dest = nullptr;
static unsigned short* printf_format = nullptr;
static float printf_args[8];
static unsigned short copied_to_buffer = 0;
static unsigned short* copy_dest = nullptr; static unsigned short* copy_src = nullptr;
static char* cat_char = nullptr;
static unsigned short* cat_dest_c = nullptr; static unsigned short* cat_dest_u = nullptr; static unsigned short* cat_src_u = nullptr;
static float box_low[3], box_high[3]; static int box_count = 0; static int box_calls = 0;
static void* box_result[3];
static unsigned box_n = 0;
static unsigned char fake_hero[1];
static char find_tag_text[] = "x";

extern "C" void* __tf9MummyKing() { trace.push_back("mummy"); return mummy_marker; }
extern "C" int __eq__C9type_infoRC9type_info(void* l, void* r) { ++eq_calls; eq_left = l; eq_right = r; trace.push_back("eq"); return type_equal ? 1 : 0; }
extern "C" void __throw_bad_typeid() { trace.push_back("throw"); }
extern "C" void* type_function() { ++type_calls; trace.push_back("type"); return type_marker; }
extern "C" char* norrath_actor_find_tag_string(void*, char* tag, int occurrence) {
    if (std::strcmp(tag, "talkCone") == 0 && occurrence == 0) return has_cone ? find_tag_text : nullptr;
    return nullptr;
}
extern "C" float findTagFloat__10GameObjectPci(void*, char* tag, int occurrence) {
    if (std::strcmp(tag, "talkCone") != 0 || occurrence != 0) failures += 100;
    return cone_value;
}
extern "C" int norrath_game_object_find_tag_int(void*, char* tag, int occurrence) {
    if (std::strcmp(tag, "shop") != 0 || occurrence != 0) failures += 100;
    return shop_tag;
}
extern "C" int gameSomeoneInShop__Fv() { return in_shop; }
extern "C" unsigned short* textGetString__FPc(char* key) {
    last_key = key;
    trace.push_back(std::string("text:") + key);
    if (last_key == "UpToShop") return up;
    if (last_key == "CannotTalk") return cannot;
    if (last_key == "TalkTo") return talk;
    return name_text;
}
extern "C" void usprintf__FPUsT0ffffffff(unsigned short* d, unsigned short* f, float a, float b, float c, float dd, float e, float ff, float g, float h) {
    printf_dest = d; printf_format = f;
    float v[8] = {a, b, c, dd, e, ff, g, h};
    for (int i = 0; i < 8; ++i) printf_args[i] = v[i];
    trace.push_back("usprintf");
}
extern "C" float icos__Fi(int angle) { cos_angle = angle; return cos_value; }
extern "C" float isin__Fi(int angle) { sin_angle = angle; return sin_value; }
extern "C" void objectFindInBox__FG6Point3T0PP10GameObjecti(const float* lo, const float* hi, void** objects, int count) {
    ++box_calls; box_count = count;
    for (int i = 0; i < 3; ++i) { box_low[i] = lo[i]; box_high[i] = hi[i]; }
    for (unsigned i = 0; i < box_n; ++i) objects[i] = box_result[i];
    objects[box_n] = nullptr;
}
extern "C" void ustrcpy__FPUsT0(unsigned short* d, unsigned short* s) { copy_dest = d; copy_src = s; trace.push_back("ustrcpy"); }
extern "C" void ustrcat__FPUsPc(unsigned short* d, char* s) { cat_dest_c = d; cat_char = s; trace.push_back("cat-char"); }
extern "C" void ustrcat__FPUsT0(unsigned short* d, unsigned short* s) { cat_dest_u = d; cat_src_u = s; trace.push_back("cat-text"); }

static unsigned char creature[1024];
static unsigned char entity[64];
static unsigned char type_table[16];
static unsigned char others[3][1024];
static void set_single(unsigned char* base, unsigned offset, float value) {
    unsigned bits; std::memcpy(&bits, &value, 4);
    vslots_b::write_word(vslots_b::address(base) + offset, bits);
}
static void reset() {
    std::memset(creature, 0, sizeof creature);
    std::memset(entity, 0, sizeof entity);
    for (auto& o : others) std::memset(o, 0, sizeof o);
    std::memset(player, 0, sizeof player);
    trace.clear();
    has_cone = false; cone_value = 0.5f; shop_tag = 0; in_shop = 0; cos_value = 1.0f; sin_value = 0.0f;
    type_equal = false; type_calls = eq_calls = 0; box_calls = 0; box_n = 0;
    printf_dest = nullptr; copy_dest = nullptr; cat_dest_c = cat_dest_u = nullptr;
    vslots_b::write_word(vslots_b::address(creature) + 892u, vslots_b::address_bits(name_tag));
    vslots_b::write_word(vslots_b::address(player) + 21872u, vslots_b::address_bits(entity));
    vslots_b::write_word(vslots_b::address(player) + 23984u + 21872u, vslots_b::address_bits(entity));
    set_single(creature, 16, 10.0f); set_single(creature, 20, 20.0f); set_single(creature, 24, 30.0f);
    set_single(entity, 16, 13.0f); set_single(entity, 20, 20.0f); set_single(entity, 24, 30.0f);   // straight ahead on x
    vslots_b::write_u16(vslots_b::address(creature) + 56u, 0x1111u);
}
static unsigned short buffer[300];
int main() {
    vslots_b::host_note(creature); vslots_b::host_note(entity); vslots_b::host_note(type_table);
    vslots_b::host_note(name_tag);
    vslots_b::host_note(reinterpret_cast<void*>(type_function));
    for (auto& o : others) vslots_b::host_note(o);
    vslots_b::write_word(vslots_b::address(type_table) + 4u, vslots_b::address_bits(reinterpret_cast<void*>(type_function)));
    auto prime = [&]() {
        reset();
        std::memset(buffer, 0x77, sizeof buffer);
        vslots_b::write_word(vslots_b::address(creature) + 72u, vslots_b::address_bits(type_table));
    };

    // early outs leave the buffer alone
    prime(); vslots_b::write_word(vslots_b::address(creature) + 892u, 0u);
    expect(msg_getPokeName__8CreaturePUsi(creature, buffer, 0) == 0 && buffer[0] == 0x7777, "no name word returns 0");
    prime(); vslots_b::write_word(vslots_b::address(player) + 21872u, 0u);
    expect(msg_getPokeName__8CreaturePUsi(creature, buffer, 0) == 0 && buffer[0] == 0x7777, "no entity returns 0");
    prime(); vslots_b::write_word(vslots_b::address(creature) + 160u, 0x00400000u);
    expect(msg_getPokeName__8CreaturePUsi(creature, buffer, 0) == 0 && buffer[0] == 0x7777, "bit 22 of +160 returns 0");
    prime(); vslots_b::write_word(vslots_b::address(creature) + 160u, 0x00800000u);
    expect(msg_getPokeName__8CreaturePUsi(creature, buffer, 0) == 0 && buffer[0] == 0x7777, "bit 23 of +160 returns 0");
    prime(); vslots_b::write_word(vslots_b::address(creature) + 164u, 0x00c00000u);
    box_n = 0;
    expect(msg_getPokeName__8CreaturePUsi(creature, buffer, 0) == 1, "high half of +160 is not part of the mask");

    // type check with bit 3
    prime(); vslots_b::write_word(vslots_b::address(creature) + 12u, 8u); type_equal = true;
    expect(msg_getPokeName__8CreaturePUsi(creature, buffer, 0) == 0 && buffer[0] == 0x7777, "MummyKing returns 0");
    expect(type_calls == 1 && eq_left == type_marker && eq_right == mummy_marker, "type compared with MummyKing");
    prime(); vslots_b::write_word(vslots_b::address(creature) + 12u, 8u); type_equal = false;
    expect(msg_getPokeName__8CreaturePUsi(creature, buffer, 0) == 1, "other type continues");
    prime(); type_equal = true;
    expect(msg_getPokeName__8CreaturePUsi(creature, buffer, 0) == 1 && type_calls == 0, "no type query without bit 3");

    // shop text
    prime(); shop_tag = 1; in_shop = 1;
    expect(msg_getPokeName__8CreaturePUsi(creature, buffer, 0) == 1, "shop text returns 1");
    expect(printf_dest == buffer && printf_format == up, "UpToShop formatted into the buffer");
    bool zeros = true; for (float f : printf_args) zeros = zeros && f == 0.0f;
    expect(zeros && box_calls == 0, "eight zero singles, no box query");
    prime(); shop_tag = 1; in_shop = 0;
    expect(msg_getPokeName__8CreaturePUsi(creature, buffer, 0) == 1 && printf_format == talk, "shop tag without anyone in the shop falls through");
    prime(); shop_tag = 0; in_shop = 1;
    expect(msg_getPokeName__8CreaturePUsi(creature, buffer, 0) == 1 && printf_format == talk, "someone in shop without the tag falls through");

    // facing: default cone 0.5
    prime(); cos_value = 1.0f; sin_value = 0.0f;
    expect(msg_getPokeName__8CreaturePUsi(creature, buffer, 0) == 1, "facing the entity talks");
    expect(cos_angle == 0x1111 && sin_angle == 0x1111, "angle is the halfword at +56");
    prime(); cos_value = 0.5f; sin_value = 0.0f;
    expect(msg_getPokeName__8CreaturePUsi(creature, buffer, 0) == 0 && buffer[0] == 0 && buffer[1] == 0x7777, "dot equal to the cone fails and clears the first halfword");
    prime(); cos_value = 0.0f; sin_value = 1.0f; set_single(entity, 20, 25.0f); set_single(entity, 16, 10.0f);
    expect(msg_getPokeName__8CreaturePUsi(creature, buffer, 0) == 1, "y term uses sin");
    // tag cone widens
    prime(); cos_value = 0.5f; has_cone = true; cone_value = 0.25f;
    expect(msg_getPokeName__8CreaturePUsi(creature, buffer, 1) == 1, "talkCone tag value replaces 0.5 (record 1)");
    prime(); cos_value = 1.0f; has_cone = true; cone_value = 2.0f;
    expect(msg_getPokeName__8CreaturePUsi(creature, buffer, 0) == 0, "talkCone tag can be stricter");

    // box query
    prime(); box_n = 0;
    msg_getPokeName__8CreaturePUsi(creature, buffer, 0);
    expect(box_calls == 1 && box_count == 64, "one box query for 64");
    expect(box_low[0] == 10.0f - 180.0f && box_low[1] == 20.0f - 180.0f && box_low[2] == 30.0f, "box minimum");
    expect(box_high[0] == 10.0f + 180.0f && box_high[1] == 20.0f + 180.0f && box_high[2] == 30.0f + 180.0f, "box maximum");
    expect(copy_dest == buffer && cat_dest_c == buffer && cat_dest_u == buffer, "talk line is built in the buffer");
    expect(cat_char && std::strcmp(cat_char, " ") == 0 && cat_src_u == name_text && last_key == "Bob", "space then the name text");
    expect(printf_format == talk && printf_dest != buffer && copy_src == printf_dest, "TalkTo formatted into a local then copied");
    expect(trace.back() == "cat-text", "name appended last");

    // blocking objects
    auto blocker = [&](unsigned char* o) {
        vslots_b::write_word(vslots_b::address(o) + 12u, 0x20u);
        vslots_b::write_u16(vslots_b::address(o) + 792u, 0u);
        vslots_b::write_word(vslots_b::address(o) + 160u, 0u);
        vslots_b::write_word(vslots_b::address(o) + 164u, 0xffffffffu);
    };
    prime(); blocker(others[0]); box_result[0] = others[0]; box_n = 1;
    expect(msg_getPokeName__8CreaturePUsi(creature, buffer, 0) == 0, "a blocker returns 0");
    expect(copy_dest == buffer && copy_src == cannot && printf_dest == nullptr, "CannotTalk copied, no TalkTo line");
    const unsigned masks[] = {0x8u, 0x40u, 0x01000000u};
    for (unsigned m : masks) {
        prime(); blocker(others[0]); vslots_b::write_word(vslots_b::address(others[0]) + 12u, 0x20u | m);
        box_result[0] = others[0]; box_n = 1;
        expect(msg_getPokeName__8CreaturePUsi(creature, buffer, 0) == 1, "an object with a skip bit does not block");
    }
    prime(); blocker(others[0]); vslots_b::write_word(vslots_b::address(others[0]) + 12u, 0u);
    box_result[0] = others[0]; box_n = 1;
    expect(msg_getPokeName__8CreaturePUsi(creature, buffer, 0) == 1, "bit 5 clear does not block");
    prime(); blocker(others[0]); vslots_b::write_u16(vslots_b::address(others[0]) + 792u, 1u);
    box_result[0] = others[0]; box_n = 1;
    expect(msg_getPokeName__8CreaturePUsi(creature, buffer, 0) == 1, "nonzero halfword at +792 does not block");
    prime(); blocker(others[0]); vslots_b::write_word(vslots_b::address(others[0]) + 160u, 0x04000000u);
    box_result[0] = others[0]; box_n = 1;
    expect(msg_getPokeName__8CreaturePUsi(creature, buffer, 0) == 1, "bit 26 of +160 does not block");
    prime(); blocker(others[0]); blocker(others[1]); vslots_b::write_word(vslots_b::address(others[0]) + 12u, 0u);
    box_result[0] = others[0]; box_result[1] = others[1]; box_n = 2;
    expect(msg_getPokeName__8CreaturePUsi(creature, buffer, 0) == 0, "a later blocker still blocks");
    return failures ? 1 : 0;
}
