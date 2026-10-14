#define NORRATH_SQ_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
static std::vector<std::string> pool;
extern "C" int objectSaveAddToStringPool__FPCc(const char* text) {
    pool.push_back(text);
    return 1000 + static_cast<int>(pool.size());
}
#include "creature_save.cpp"
using sq::write_word;
using sq::read_word;
static unsigned char object[1024];
static unsigned char table[8 * 6];
static char buf[64];
static char name_text[] = "orcName";
static char idle_text[] = "idle";
static char walk_text[] = "walk";
static char die_text[] = "die";
static void setup(float health) {
    std::memset(object, 0, sizeof object);
    std::memset(table, 0, sizeof table);
    std::memset(buf, 0x77, sizeof buf);
    pool.clear();
    write_word(object + 244, 0xdeadbeefu);
    sq::write_single(object + 560, health);
    write_word(object + 892, sq::pointer_bits(name_text));
    write_word(object + 36, sq::pointer_bits(table));
    object[570] = 0x5c;
}
static void entry(int i, char* text, unsigned header) {
    write_word(table + i * 8, text ? sq::pointer_bits(text) : 0);
    write_word(table + i * 8 + 4, header);
}
int main() {
    // alive, no flags
    setup(25.9f);
    int n = msg_save__8CreaturePc(object, buf);
    if (n != 16) return 1;
    if (read_word(buf) != 0xdeadbeefu || read_word(buf + 4) != 25u || read_word(buf + 8) != 1001u) return 2;
    if ((unsigned char)buf[12] != 0x5c || buf[13] != 0 || buf[14] != 0 || buf[15] != 0 || buf[16] != 0x77) return 3;
    if (pool.size() != 1 || pool[0] != "orcName") return 4;
    // a negative health below one truncates toward zero and takes the dead path
    setup(-3.9f);
    entry(0, idle_text, 1);
    entry(1, nullptr, 0);
    n = msg_save__8CreaturePc(object, buf);
    if (n != 20 || read_word(buf + 4) != (unsigned)-3 || read_word(buf + 12) != 0xffffffffu) return 5;
    // dead: header matches the third entry; selector 1 reads object+296
    setup(0.0f);
    entry(0, idle_text, 11);
    entry(1, walk_text, 22);
    entry(2, die_text, 33);
    object[292] = 1;
    write_word(object + 296, 33);
    write_word(object + 264, 22);  // selector 0 would have picked walk
    n = msg_save__8CreaturePc(object, buf);
    if (n != 20 || read_word(buf + 12) != 1002u || pool.size() != 2 || pool[1] != "die") return 6;
    // entry 0 is never matched
    setup(0.0f);
    entry(0, idle_text, 11);
    entry(1, walk_text, 22);
    write_word(object + 264, 11);
    msg_save__8CreaturePc(object, buf);
    if (read_word(buf + 12) != 0xffffffffu || pool.size() != 1) return 7;
    // a negative selector byte indexes backwards
    setup(0.0f);
    entry(1, walk_text, 22);
    object[292] = 0xff;
    write_word(object + 264 - 32, 22);
    msg_save__8CreaturePc(object, buf);
    if (read_word(buf + 12) != 1002u) return 8;
    // NaN health skips the dead word
    setup(0.0f);
    write_word(object + 560, 0x7fc00000u);
    n = msg_save__8CreaturePc(object, buf);
    if (n != 16) return 9;
    // flag bit 0 adds the converted single and the sign-extended byte
    setup(10.0f);
    write_word(object + 160, 1u);
    sq::write_single(object + 564, -7.8f);
    object[76] = 0xfe;
    n = msg_save__8CreaturePc(object, buf);
    if (n != 24 || read_word(buf + 12) != (unsigned)-7 || read_word(buf + 16) != 0xfffffffeu) return 10;
    if ((unsigned char)buf[20] != 0x5c || buf[21] != 0) return 11;
    // mask bits, each alone
    struct Case { unsigned lo, hi; unsigned char mask; } cases[] = {
        {0x01000000u, 0, 1}, {0, 1u, 2}, {0x400000u, 0, 4}, {0x800000u, 0, 8},
        {0x01c00000u, 1u, 15}, {0x3e3fffffu, 0, 0}, {0xfe000000u, 0xfffffffeu, 0}};
    for (const Case& c : cases) {
        setup(10.0f);
        write_word(object + 160, c.lo & ~1u);
        write_word(object + 164, c.hi);
        n = msg_save__8CreaturePc(object, buf);
        if (n != 16 || (unsigned char)buf[13] != c.mask) return 12;
    }
    std::puts("ok");
    return 0;
}
