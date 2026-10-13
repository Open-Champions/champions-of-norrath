// Host check for createRandom__4itemPQ24item11descriptioniiii (includes the source; build the file alone with -fno-builtin).
// The item, prefix and suffix tables and the callees are small stand-ins. Expected random draws are computed here with the
// same linear congruential step the image uses.
#define NORRATH_INVENTORY_HOST 1
#include "create_random.cpp"
alignas(8) unsigned char player[4 * 23984];
alignas(8) unsigned char norrath_item_table_view[64 * 8];
alignas(8) unsigned char _4item$prefixTable[40 * 4];
alignas(8) unsigned char _4item$suffixTable[20 * 4];
unsigned syncRandState;
int nmPlayers;
static const void* table[64]; static unsigned used = 0;
static unsigned intern(const void* p) { table[used] = p; return 0x1000 + 0x10 * used++; }
const void* inventory_items::host_resolve(unsigned w) { return table[(w - 0x1000) / 0x10]; }

static char logbuf[1 << 15]; static unsigned logn;
static void put(const char* s) { while (*s) logbuf[logn++] = *s++; logbuf[logn] = 0; }
static void put_int(long v) { char t[24]; int n = 0; bool neg = v < 0; unsigned long u = neg ? -v : v; do { t[n++] = char('0' + u % 10); u /= 10; } while (u); if (neg) logbuf[logn++] = '-'; while (n) logbuf[logn++] = t[--n]; logbuf[logn] = 0; }
static bool starts(const char* a, const char* b) { while (*b && *a == *b) { ++a; ++b; } return *b == 0; }
static int count_of(const char* word) { int n = 0; for (const char* p = logbuf; *p; ++p) if (starts(p, word)) ++n; return n; }
static int create_calls; static int value_for_call[64]; static unsigned sword_word;
extern "C" {
char* strcpy(char* to, const char* from) { char* r = to; while ((*to++ = *from++)) {} return r; }
char* strcat(char* to, const char* from) { char* r = to; while (*to) ++to; while ((*to++ = *from++)) {} return r; }
char* strstr(const char* t, const char* n) { for (; *t; ++t) if (starts(t, n)) return const_cast<char*>(t); return 0; }
float powf(float x, float y) { put("powf("); put_int(static_cast<long>(x)); put(","); put_int(static_cast<long>(y * 100000.0f + 0.5f)); put(")\n"); return x * 3.0f + 0.5f; }
void create__4itemPcPQ24item11descriptionii(char* name, void* d, int a, int b) {
    put("create("); put(name); put(","); put_int(a); put(","); put_int(b); put(")\n");
    unsigned char* item = static_cast<unsigned char*>(d);
    item[4] = static_cast<unsigned char>(++create_calls);           // call number marker
    *reinterpret_cast<unsigned*>(item) = sword_word;                 // row pointer word: the first table row
    item[32] = 0xee; item[33] = 0xee;
    for (int i = 8; i < 16; ++i) item[i] = 0;                       // the 64-bit word at +8 starts clear
}
int getValue__4itemPQ24item11description(const void* d) { return value_for_call[static_cast<const unsigned char*>(d)[4]]; }
}
static unsigned char* row_at(int i) { return norrath_item_table_view + 64 * i; }
static void set_row(int i, unsigned name_word, int minlevel, int type, int category, int subtype, unsigned word32, unsigned long long flags) {
    unsigned char* r = row_at(i);
    *reinterpret_cast<unsigned*>(r) = name_word;
    *reinterpret_cast<short*>(r + 4) = static_cast<short>(minlevel);
    r[28] = static_cast<unsigned char>(type); r[29] = static_cast<unsigned char>(category); r[30] = static_cast<unsigned char>(subtype);
    *reinterpret_cast<unsigned*>(r + 32) = word32; *reinterpret_cast<unsigned long long*>(r + 40) = flags;
}
static void clear() {
    for (unsigned char& b : norrath_item_table_view) b = 0; for (unsigned char& b : _4item$prefixTable) b = 0; for (unsigned char& b : _4item$suffixTable) b = 0;
    for (unsigned char& b : player) b = 0; logn = 0; logbuf[0] = 0; create_calls = 0; for (int& v : value_for_call) v = 0; nmPlayers = 1;
}
static void set_suffix(int i, int minlevel, unsigned text_for_category) { *reinterpret_cast<unsigned*>(_4item$suffixTable + 20 * i) = 1; *reinterpret_cast<short*>(_4item$suffixTable + 20 * i + 4) = static_cast<short>(minlevel); for (int c = 0; c < 3; ++c) *reinterpret_cast<unsigned*>(_4item$suffixTable + 20 * i + 8 + 4 * c) = text_for_category; }
static void set_prefix(int i, int minlevel, unsigned text, unsigned long long mask, unsigned mask32) {
    unsigned char* p = _4item$prefixTable + 40 * i; *reinterpret_cast<unsigned*>(p) = 1; *reinterpret_cast<short*>(p + 4) = static_cast<short>(minlevel);
    for (int c = 0; c < 3; ++c) *reinterpret_cast<unsigned*>(p + 12 + 4 * c) = text; *reinterpret_cast<unsigned long long*>(p + 24) = mask; *reinterpret_cast<unsigned*>(p + 32) = mask32;
}
static unsigned lcg(unsigned s) { return s * 1664525u + 1013904223u; }
static unsigned advance(unsigned s, int n) { while (n--) s = lcg(s); return s; }
alignas(8) static unsigned char out[72];
static unsigned long long out_flags() { return *reinterpret_cast<unsigned long long*>(out + 8); }
int main() {
    const unsigned name_sword = intern("sword"), name_potion = intern("healpotion"), text_def = intern("_def"), text_suf = intern("_suf"), text_pre = intern("_pre"), text_pre2 = intern("_pr2");
    sword_word = intern(norrath_item_table_view);                     // create() leaves the first table row's pointer word in the record
    // 1. no eligible row (the only row has flag 0x10000): nothing is drawn, nothing created, the output is untouched
    clear(); set_row(0, name_sword, 0, 0, 0, 0, 0, 0x10000); syncRandState = 77;
    for (unsigned char& b : out) b = 0x5a;
    if (createRandom__4itemPQ24item11descriptioniiii(out, 1000, -1, 0, 5) != 0 || syncRandState != 77 || create_calls != 0 || out[0] != 0x5a) return 1;
    // 2. category 2, extra 0, one suffix record: ten rounds, the default suffix text, best value not above the limit, charge bits
    clear(); set_row(0, name_sword, 0, 0, 2, 0, 0, 0x2000ull); set_suffix(0, 0, text_def); syncRandState = 5;
    for (int k = 1; k <= 10; ++k) value_for_call[k] = 100 * k;
    if (createRandom__4itemPQ24item11descriptioniiii(out, 600, 2, 0, 3) != 1 || create_calls != 10 || count_of("create(sword_def,-1,0)") != 10) return 2;
    if (out[4] != 6) return 3;                                                           // value 600 equals the limit and is kept; 700 and above are not
    if (count_of("powf(") != 10) return 4;
    // 20 draws for ten rounds (pick, suffix), 1 for the charge count 4 = (0x2000 >> 11) & 31
    if (syncRandState != advance(5, 21)) return 5;
    if (((out_flags() >> 11) & 31) != (advance(5, 21) >> 16) % 4 || (out_flags() & 0x7ff) != 0) return 6;
    // 3. value bonus: id 14 in the first free entry with min(2 * (int)powf(max(v - 1200, 0) / 2000, 0.66) + 1, 50)
    clear(); set_row(0, name_sword, 0, 0, 2, 0, 0, 0); set_suffix(0, 0, text_def); syncRandState = 1;
    for (int k = 1; k <= 10; ++k) value_for_call[k] = 5200;          // base 2 -> stub powf 6.5 -> 6 -> 13
    createRandom__4itemPQ24item11descriptioniiii(out, 100000, 2, 0, 3);
    if (*reinterpret_cast<short*>(out + 36) != 14 || *reinterpret_cast<short*>(out + 38) != 13 || *reinterpret_cast<short*>(out + 40) != 0 || *reinterpret_cast<short*>(out + 42) != 0) return 7;
    if (count_of("powf(2,66000)") != 10) return 8;                       // exponent 0.66
    clear(); set_row(0, name_sword, 0, 0, 2, 0, 0, 0); set_suffix(0, 0, text_def); syncRandState = 1;
    for (int k = 1; k <= 10; ++k) value_for_call[k] = 100000;       // capped at 50
    createRandom__4itemPQ24item11descriptioniiii(out, 1000000, 2, 0, 3);
    if (*reinterpret_cast<short*>(out + 38) != 50) return 9;
    clear(); set_row(0, name_sword, 0, 0, 2, 0, 0, 0); set_suffix(0, 0, text_def); syncRandState = 1;
    for (int k = 1; k <= 10; ++k) value_for_call[k] = 10;           // below 1200: base 0 -> 0.5 -> 0 -> 1
    createRandom__4itemPQ24item11descriptioniiii(out, 1000000, 2, 0, 3);
    if (*reinterpret_cast<short*>(out + 38) != 1) return 10;
    // 4. potions never get the attribute and never take a random suffix text
    clear(); set_row(0, name_potion, 0, 0, 2, 0, 0, 0); set_suffix(0, 0, text_def); set_suffix(1, 0, text_suf); syncRandState = 3;
    for (int k = 1; k <= 10; ++k) value_for_call[k] = 5200;
    createRandom__4itemPQ24item11descriptioniiii(out, 100000, 2, 1, 3);
    if (count_of("powf(") != 0 || count_of("create(healpotion_def,-1,0)") != 10 || *reinterpret_cast<short*>(out + 36) != -1) return 11;
    // extra -1 in category 2 becomes 1: the chosen suffix text is appended (two suffix records, all rounds create either text)
    clear(); set_row(0, name_sword, 0, 0, 2, 0, 0, 0); set_suffix(0, 0, text_def); set_suffix(1, 0, text_suf); syncRandState = 11;
    for (int k = 1; k <= 10; ++k) value_for_call[k] = 10;
    createRandom__4itemPQ24item11descriptioniiii(out, 100000, 2, -1, 3);
    if (count_of("create(sword_def,-1,0)") + count_of("create(sword_suf,-1,0)") != 10 || count_of("create(sword_suf,-1,0)") == 0 || count_of("create(sword_def,-1,0)") == 0) return 12;
    // a suffix record above the level is redrawn: with record 1 at minimum level 50 and level 3, only record 0 is used
    clear(); set_row(0, name_sword, 0, 0, 2, 0, 0, 0); set_suffix(0, 0, text_def); set_suffix(1, 50, text_suf); syncRandState = 11;
    for (int k = 1; k <= 10; ++k) value_for_call[k] = 10;
    createRandom__4itemPQ24item11descriptioniiii(out, 100000, 2, 1, 3);
    if (count_of("create(sword_suf,-1,0)") != 0 || count_of("create(sword_def,-1,0)") != 10 || out[33] != 0) return 13;
    // 5. category 0 needs kindA 0: a seed whose first draw gives u < 0.05; fifteen rounds with two prefix records
    int found = 0;
    for (unsigned seed = 12345, tries = 0; tries < 4000 && !found; ++tries, seed = lcg(seed)) {
        const float u = static_cast<float>(lcg(seed) >> 16) * bits(0x37800000);
        if (!(u < bits(0x3d4ccccc))) continue;
        found = 1;
        clear(); set_row(0, name_sword, 0, 0, 0, 6, 0x10, 0x2000ull); set_suffix(0, 0, text_def);
        set_prefix(0, 0, text_pre, 0, 0); set_prefix(1, 0, text_pre2, 0x4, 0x10);
        for (int k = 1; k <= 30; ++k) value_for_call[k] = 10;
        syncRandState = seed;
        if (createRandom__4itemPQ24item11descriptioniiii(out, 1000, 0, 0, 3) != 1) return 14;
        if (create_calls != 30 || count_of("create(sword,-1,0)") != 16) return 15;      // round 0 has no prefix text; prefix 1 never matches the row flags
        if (!starts(logbuf, "create(sword,-1,0)\npowf(0,66000)\ncreate(sword,-1,0)\npowf(0,66000)\ncreate(sword_pre,-1,0)\npowf(0,66000)\ncreate(sword,-1,0)\n")) return 16;   // round 0 has no prefix text
        if (count_of("create(sword_pre,-1,0)") != 14) return 16;                        // prefix 0 (mask 0) appends from round 1 on
        if (count_of("create(sword_pr2") != 0) return 17;                               // prefix 1's mask 4 is not in the row flags
        // the last kept template is call 30 (prefix 1, last round): word +32 mask 0x10 matches the row word 0x10 -> index 1
        if (out[4] != 30 || out[32] != 1) return 18;
    }
    if (!found) return 19;
    // 6. category 1 with two tiers: only rows whose type class matches the drawn tier are eligible
    //    (a seed is searched for each of the first drawn flagA/flagB/tier situations and the count of created items checked)
    int matched = 0, m_tier0 = 0, m_tier1 = 0, m_tier5 = 0;
    for (unsigned seed = 777, tries = 0; tries < 6000 && (m_tier0 == 0 || m_tier1 == 0 || m_tier5 == 0); ++tries, seed = lcg(seed)) {
        clear(); set_row(0, name_sword, 0, 11, 1, 0, 0, 0x2000ull); set_row(1, name_potion, 0, 4, 1, 0, 0, 0x2000ull); set_suffix(0, 0, text_def); set_prefix(0, 0, text_pre, 0, 0);
        for (int k = 1; k <= 30; ++k) value_for_call[k] = 10;
        syncRandState = seed; nmPlayers = 1;
        const unsigned first = (lcg(seed) >> 16) % 65535u;
        const bool flag_a = 0 < first;                                  // fraction 0: (int)(0 * 65536) = 0 < first
        (void)flag_a;
        const unsigned after = advance(seed, 2);
        const float u = static_cast<float>(after >> 16) * bits(0x37800000);
        const int tier = u < bits(0x3e23d70a) ? 0 : (u < bits(0x3ea3d70a) ? 1 : 5);
        const int result = createRandom__4itemPQ24item11descriptioniiii(out, 1000, 1, 0, 3);
        if (tier == 0 && first != 0) { if (result != 1 || count_of("create(sword") != 10 || count_of("create(healpotion") != 0) return 23; ++matched; ++m_tier0; }      // type 11 -> class 0 matches tier 0
        else if (tier == 1 && first != 0) { if (count_of("create(healpotion") != 10 || result != 1 || count_of("create(sword") != 0) return 20; ++matched; ++m_tier1; }  // type 4 -> class 1
        else if (tier == 5 && first != 0) { if (result != 0 || create_calls != 0) return 21; ++matched; ++m_tier5; }
    }
    if (m_tier0 == 0 || m_tier1 == 0 || m_tier5 == 0) return 22;
    return 0;
}
