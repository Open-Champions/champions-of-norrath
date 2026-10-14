// Host check for shopItemSetAttributes__FiiPcPQ24item11description (includes the source; build the file alone with -fno-builtin).
// No standard headers: the check supplies the C string routines and a small sscanf (literals and %d only).
#include "shop_item_set_attributes.cpp"
alignas(8) unsigned char player[2 * 23984];
alignas(8) unsigned char inventory[464];
const unsigned char norrath_inventory_ability_sound_lump_view[8] = {'f', 'x', '.', 'l', 'm', 'p', 0, 0};

static char logbuf[4096]; static unsigned used;
static void put(const char* s) { while (*s) logbuf[used++] = *s++; logbuf[used] = 0; }
static void put_int(long v) { char t[24]; int n = 0; bool neg = v < 0; unsigned long u = neg ? -v : v; do { t[n++] = char('0' + u % 10); u /= 10; } while (u); if (neg) logbuf[used++] = '-'; while (n) logbuf[used++] = t[--n]; logbuf[used] = 0; }
static bool same(const char* a, const char* b) { while (*a && *a == *b) { ++a; ++b; } return *a == *b; }
static bool has(const char* word) { for (const char* p = logbuf; *p; ++p) { const char* a = p; const char* b = word; while (*b && *a == *b) { ++a; ++b; } if (!*b) return true; } return false; }
extern "C" {
char* strcpy(char* to, const char* from) { char* r = to; while ((*to++ = *from++)) {} return r; }
char* strchr(const char* t, int c) { for (; ; ++t) { if (*t == char(c)) return const_cast<char*>(t); if (!*t) return 0; } }
char* strstr(const char* t, const char* n) { for (; *t; ++t) { const char* a = t; const char* b = n; while (*b && *a == *b) { ++a; ++b; } if (!*b) return const_cast<char*>(t); } return 0; }
void* memset(void* t, int v, unsigned n) { unsigned char* p = static_cast<unsigned char*>(t); while (n--) *p++ = static_cast<unsigned char>(v); return t; }
int sscanf(const char* text, const char* format, ...) {
    __builtin_va_list ap; __builtin_va_start(ap, format);
    int count = 0; int result = -2;
    while (*format) {
        if (*format == '%') {
            format += 2;
            while (*text == ' ') ++text;
            if (!*text) { result = count == 0 ? -1 : count; break; }
            bool neg = false; if (*text == '-' || *text == '+') { neg = *text == '-'; ++text; }
            if (*text < '0' || *text > '9') { result = count; break; }
            int v = 0; while (*text >= '0' && *text <= '9') v = v * 10 + (*text++ - '0');
            *__builtin_va_arg(ap, int*) = neg ? -v : v; ++count;
        } else {
            if (!*text) { result = count == 0 ? -1 : count; break; }
            if (*text != *format) { result = count; break; }
            ++text; ++format;
        }
    }
    __builtin_va_end(ap);
    return result == -2 ? count : result;
}
}
static int local;
int netPlayerIsLocal__Fi(int p) { put("local("); put_int(p); put(")\n"); return local; }
char* lumpFindResource__FPcT0(char* l, char* n) { put("lump("); put(l); put(","); put(n); put(")\n"); return n; }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void*, void* o, float v) { put(o == 0 && v == 1.0f ? "sound\n" : "sound?\n"); }
static unsigned short key_title[1], key_message[1];
unsigned short* textGetString__FPc(char* k) { put("text("); put(k); put(")\n"); return same(k, "CantCombineTitle") ? key_title : key_message; }
void multiHudAddHelpMessage__FiPUsT1i(int p, unsigned short* t, unsigned short* m, int side) { put("help("); put_int(p); put(t == key_title ? ",title" : ",?"); put(m == key_message ? ",message," : ",?,"); put_int(side); put(")\n"); }

static short& id(unsigned char* b, int k) { return *reinterpret_cast<short*>(b + 36 + 8 * k); }
static short& val(unsigned char* b, int k, int j) { return *reinterpret_cast<short*>(b + 38 + 8 * k + 2 * j); }
static unsigned char* slot_of(int owner, int list, int index) { return reinterpret_cast<unsigned char*>(inventory_items::player_lists(owner)) + 4616 * list + 72 * index; }
alignas(8) static unsigned char item[72];
static void reset() {
    for (unsigned char& b : player) b = 0; for (unsigned char& b : item) b = 0; used = 0; logbuf[0] = 0; local = 1;
    for (int k = 0; k < 4; ++k) id(item, k) = -1;
    for (int o = 0; o < 2; ++o) for (int l = 0; l < 3; ++l) for (int s = 0; s < 64; ++s) for (int k = 0; k < 4; ++k) id(slot_of(o, l, s), k) = -1;
    *reinterpret_cast<short*>(item + 34) = 7;
    *reinterpret_cast<int*>(inventory + 56) = 1; *reinterpret_cast<int*>(inventory + 172) = 0;
}
static int run(int owner, int list, const char* text) {
    char copy[200]; strcpy(copy, text);
    return shopItemSetAttributes__FiiPcPQ24item11description(owner, list, copy, item);
}
static bool entry(int k, int want_id, int a, int b, int c) { return id(item, k) == want_id && val(item, k, 0) == a && val(item, k, 1) == b && val(item, k, 2) == c; }
int main() {
    reset();
    if (run(1, 2, "dagger") != 1 || id(item, 0) != -1 || used != 0) return 1;                        // no '+': nothing happens
    reset();
    if (run(1, 2, "dagger+armorbonus-55+requiredlevel1") != 1) return 2;
    if (!entry(0, 13, -55, 0, 0) || !entry(1, 14, 1, 0, 0) || id(item, 2) != -1) return 3;
    reset();
    if (run(1, 2, "x+bugzapper3-9+fireburst50,20-40+poisonburst100,3+resistfire18") != 1) return 4;
    if (!entry(0, 25, 3, 9, 0) || !entry(1, 31, 50, 20, 40) || !entry(2, 33, 100, 3, 0) || !entry(3, 0, 18, 0, 0)) return 5;
    // an existing entry with the same id is overwritten in place
    reset(); id(item, 0) = 5; id(item, 1) = 13; val(item, 1, 0) = 9; val(item, 1, 1) = 9; val(item, 1, 2) = 9;
    if (run(1, 0, "x+armorbonus4") != 1 || !entry(1, 13, 4, 0, 0) || id(item, 0) != 5) return 6;
    // all four entries taken by other ids: nothing stored, still success
    reset(); for (int k = 0; k < 4; ++k) id(item, k) = 20 + k;
    if (run(1, 0, "x+armorbonus4") != 1 || id(item, 0) != 20 || id(item, 3) != 23) return 7;
    // the player's own slot (list 2, slot 7) already has four entries: too many
    reset(); for (int k = 0; k < 4; ++k) id(slot_of(1, 2, 7), k) = k;
    if (run(1, 2, "x+armorbonus4") != 0) return 8;
    if (!same(logbuf, "local(1)\nlump(fx.lmp,game_butn_cantdo.anm)\nsound\ntext(CantCombineTitle)\ntext(CantCombineTooMany)\nhelp(1,title,message,0)\n")) return 9;
    if (id(item, 0) != -1) return 10;
    // a different list or slot does not count
    reset(); for (int k = 0; k < 4; ++k) id(slot_of(1, 1, 7), k) = k;
    if (run(1, 2, "x+armorbonus4") != 1) return 11;
    // fire or cold damage with three used entries refuses; with two it does not
    reset(); for (int k = 0; k < 3; ++k) id(slot_of(1, 2, 7), k) = k + 50;
    if (run(1, 2, "x+firedamage1-2") != 0 || !has("CantCombineTooMany")) return 12;
    reset(); for (int k = 0; k < 3; ++k) id(slot_of(1, 2, 7), k) = k + 50;
    if (run(1, 2, "x+colddamage1-2") != 0 || !has("CantCombineTooMany")) return 13;
    reset(); for (int k = 0; k < 3; ++k) id(slot_of(1, 2, 7), k) = k + 50;
    if (run(1, 2, "x+armorbonus1") != 1) return 14;                                                  // three used entries are fine without them
    reset(); for (int k = 0; k < 2; ++k) id(slot_of(1, 2, 7), k) = k + 50;
    if (run(1, 2, "x+firedamage1-2") != 1 || !entry(0, 21, 1, 2, 0)) return 15;
    // the player's slot already has the attribute with a non-zero first value: refused with the attribute message
    reset(); id(slot_of(1, 2, 7), 1) = 13; val(slot_of(1, 2, 7), 1, 0) = 5;
    if (run(1, 2, "x+armorbonus4+healthbonus3") != 0) return 16;
    if (!same(logbuf, "local(1)\nlump(fx.lmp,game_butn_cantdo.anm)\nsound\ntext(CantCombineTitle)\ntext(CantCombineAttribute)\nhelp(1,title,message,0)\n") || id(item, 0) != -1) return 17;
    reset(); id(slot_of(1, 2, 7), 1) = 13; val(slot_of(1, 2, 7), 1, 0) = -5;                        // a negative value blocks too
    if (run(1, 2, "x+armorbonus4") != 0) return 23;
    reset(); id(slot_of(1, 2, 7), 1) = 13; val(slot_of(1, 2, 7), 1, 0) = 0;                         // value zero does not block
    if (run(1, 2, "x+armorbonus4") != 1) return 18;
    // side and sound choices: the word at inventory+56 decides; a remote player plays no sound
    reset(); local = 0; for (int k = 0; k < 4; ++k) id(slot_of(1, 2, 7), k) = k; *reinterpret_cast<int*>(inventory + 56) = 0; *reinterpret_cast<int*>(inventory + 172) = 1;
    run(1, 2, "x+armorbonus4");
    if (!same(logbuf, "local(1)\ntext(CantCombineTitle)\ntext(CantCombineTooMany)\nhelp(1,title,message,1)\n")) return 19;
    reset(); *reinterpret_cast<int*>(inventory + 56) = 0; *reinterpret_cast<int*>(inventory + 172) = 0; for (int k = 0; k < 4; ++k) id(slot_of(1, 2, 7), k) = k;
    run(1, 2, "x+armorbonus4");
    if (!has("help(1,title,message,0)")) return 22;                                                  // neither word names the player
    // an empty segment reports EOF from sscanf, which counts as a match of the first format
    reset();
    if (run(1, 2, "x+") != 1 || !entry(0, 35, 0, 0, 0)) return 20;
    // an unrecognised segment stores nothing
    reset();
    if (run(1, 2, "x+nonsense9") != 1 || id(item, 0) != -1) return 21;
    return 0;
}
