// Observed createRandom__4itemPQ24item11descriptioniiii at 0x00104440, 4220 bytes
// (item::createRandom). Arguments: the output item record (72 bytes), a value
// limit, a category, a flag-like value (extra) and a level. Returns 1 when an item
// was written to the output record and 0 when none was; the variable names below
// describe observed use only.
// Tables (all from the image, all unresolved imports here):
//  - item table (_4item$itemTable, 64-byte rows, ended by a row whose name word is
//    0; named norrath_item_table_view by the unit that defines it): +0 name
//    pointer, +4 signed halfword minimum level, +28 type byte, +29 category byte,
//    +30 subtype byte, +32 word, +40 64-bit flag word.
//  - _4item$prefixTable (40-byte records ended by a record whose first word is 0):
//    +4 signed halfword minimum level, +12 + 4*category a text pointer or 0, +24
//    a 64-bit mask, +32 a word mask.
//  - _4item$suffixTable (20-byte records, 91 of them in the image): +4 signed
//    halfword minimum level, +8 + 4*category a text pointer or 0.
// Random numbers come from the word syncRandState: each draw replaces it with
// state * 1664525 + 1013904223 and uses state >> 16.
// Steps:
//  1. wizards = number of the nmPlayers players whose record+80 word is 4;
//     fraction = wizards / nmPlayers (single precision). The player record is 23984
//     bytes (record.hpp). flagA = flagB = 0, kindA = tierB = -1.
//  2. Category 1: a draw r1 (bits 16.. modulo 65535) is compared with (int)(fraction *
//     65536): below it, flagA = 1; otherwise, when fraction > 0, another draw
//     sets flagB when its remainder is at most 0xbfff. Category 0: u = (state >> 16)
//     * 2^-16; kindA = 0 below 0.05, 1 below 0.1, else 2. Category 1 also draws
//     u the same way for tierB: 0 below 0.16, then 1, 2, 3, 4 below 0.32, 0.48, 0.64, 0.8, else
//     5. (Thresholds are the single-precision words 0x3d4ccccc, 0x3dcccccc,
//     0x3e23d70a, 0x3ea3d70a, 0x3ef5c28f, 0x3f23d70a, 0x3f4ccccc.)
//  3. A row is eligible when its flag word (+40) lacks bits 0x10000 and bit 35, the
//     category is -1 or equals the row's, the level is not below the row's minimum,
//     and: with flagB, a category-1 row without flag 0x80000 must have "circlet" in
//     its name; with flagA, a category-1 row with flag 0x80000 is dropped; a
//     category-0 row needs classA(row) == kindA (classA: subtype 6 -> 0, other
//     than 3 -> 2, subtype 3 -> 1 when word+32 has bit 8, else 2); a category-1 row
//     needs classB(row) == tierB (classB from the type byte: 11 -> 0, 4 -> 1, 9 ->
//     2, 12 -> 3, 3 -> 4, else 5). The eligible rows are counted; none -> return 0.
//  4. extra: when it is -1, category 2 sets it to 1 and otherwise a draw below 6500
//     (state >> 16) sets it to 1.
//  5. cnt2 = number of prefix records whose minimum level is not above the level
//     (counted in order up to the end marker); cnt3 = number of suffix records.
//     Rounds = 15 for category 0, else 10; best = -1.
//  6. Each round re-initialises the local item template (the four attribute
//     halfword ids at +36,+44,+52,+60 = -1, bytes +32 and +33 = 0xff, halfwords
//     +16..+22 and +24..+30 = 0), draws k = (draw % eligible count), and walks the
//     rows in order, counting eligible ones down until the k-th. It then draws a
//     suffix index r (draw % cnt3), redrawing while the level is below that
//     record's minimum level.
//     Category 2: the row name is copied to a buffer; the text appended is the
//     suffix record's text for the row category when extra > 0, the word is not 0
//     and the name has no "potion"; otherwise the text of suffix record 0 for that
//     category. item::create(buffer, template, -1, 0) runs; template byte +32 is -1
//     and +33 is r when extra > 0, that text exists and the name has no "potion",
//     else 0. value = getValue(template).
//     Other categories: for each prefix record in order up to cnt2, skipping those
//     above the level: the name is copied; from the second round on a prefix text
//     (record+12+4*category, when the row flags & record mask is non-zero or the
//     mask is 0) and, when extra > 0 and the suffix text exists and the name has no
//     "potion", the suffix text are appended; item::create runs; from the second
//     round on template +32 becomes the record index (as the text rule says), or
//     record word +32 against row word +32 selects index or -1, and +33 may become r;
//     value = getValue(template).
//     In both: unless the name contains "potion", an attribute id 14 is added to
//     the first free or equal entry with first value min(2 * (int)powf(max(value
//     - 1200, 0) / 2000, 0.66f) + 1, 50). A template is kept (copied to the
//     output, best = value) when value >= best and value <= the limit argument.
//  7. After all rounds: best < 0 -> return 0. Otherwise with n = (row flags >> 11)
//     & 31 of the output's row (taken from the output's first word), bits 11..15
//     of the output's 64-bit word at +8 are replaced by (draw % n) << 11 (a draw
//     only when n > 0, else 0), and the result is 1.
// Unknown: what the categories, flags, prefix/suffix words and the bit meanings
// are; the item::create call is a different unit (a2 = -1, a3 = 0 are observed).
#include "../inventory-items-record/record.hpp"
extern "C" {
extern unsigned char norrath_item_table_view[];
extern unsigned char _4item$prefixTable[];
extern unsigned char _4item$suffixTable[];
extern unsigned syncRandState;
extern int nmPlayers;
char* strcpy(char* to, const char* from);
char* strcat(char* to, const char* from);
char* strstr(const char* text, const char* pattern);
float powf(float base, float exponent);
int getValue__4itemPQ24item11description(const void* item);
void create__4itemPcPQ24item11descriptionii(char* name, void* description, int a, int b);
}

namespace {
float bits(unsigned value) { return __builtin_bit_cast(float, value); }
unsigned draw() {
    syncRandState = syncRandState * 1664525u + 1013904223u;
    return syncRandState >> 16;
}
unsigned word(const unsigned char* base, unsigned offset) { return *reinterpret_cast<const unsigned*>(base + offset); }
unsigned long long wide(const unsigned char* base, unsigned offset) { return *reinterpret_cast<const unsigned long long*>(base + offset); }
int half(const unsigned char* base, unsigned offset) { return *reinterpret_cast<const short*>(base + offset); }
const char* text_at(unsigned pointer_word) { return inventory_items::resolve<char>(pointer_word); }
const char* name_of(const unsigned char* row) { return text_at(word(row, 0)); }

int class_a(const unsigned char* row) {
    const int subtype = static_cast<signed char>(row[30]);
    if (subtype == 6) return 0;
    if (subtype != 3) return 2;
    return (word(row, 32) & 8u) ? 1 : 2;
}
int class_b(const unsigned char* row) {
    const int type = static_cast<signed char>(row[28]);
    if (type == 11) return 0;
    if (type == 4) return 1;
    if (type == 9) return 2;
    if (type == 12) return 3;
    return type == 3 ? 4 : 5;
}

struct Filter {
    int category;
    int level;
    int flag_a;
    int flag_b;
    int kind_a;
    int tier_b;
    bool eligible(const unsigned char* row) const {
        const unsigned long long flags = wide(row, 40);
        if (flags & 0x10000ull) return false;
        const int row_category = static_cast<signed char>(row[29]);
        if (category != -1 && row_category != category) return false;
        if (level < half(row, 4)) return false;
        if (flags & 0x800000000ull) return false;
        if (flag_b != 0 && row_category == 1 && (flags & 0x80000ull) == 0 && strstr(name_of(row), "circlet") == 0) return false;
        if (flag_a != 0 && row_category == 1 && (flags & 0x80000ull) != 0) return false;
        if (row_category == 0) {
            if (class_a(row) != kind_a) return false;
        } else if (row_category == 1) {
            if (class_b(row) != tier_b) return false;
        }
        return true;
    }
};

// Value bonus: an attribute id 14 in the first free or equal entry of the template.
void add_level_requirement(unsigned char* item, int value) {
    int base = value - 1200;
    if (base < 0) base = 0;
    base /= 2000;
    int amount = static_cast<int>(powf(static_cast<float>(base), bits(0x3f28f5c2))) * 2 + 1;
    if (amount >= 51) amount = 50;
    for (int entry = 0; entry < 4; ++entry) {
        short* id = reinterpret_cast<short*>(item + 36 + 8 * entry);
        if (*id == -1 || *id == 14) {
            *id = 14;
            *reinterpret_cast<short*>(item + 38 + 8 * entry) = static_cast<short>(amount);
            *reinterpret_cast<short*>(item + 40 + 8 * entry) = 0;
            *reinterpret_cast<short*>(item + 42 + 8 * entry) = 0;
            break;
        }
    }
}

void copy_item(unsigned char* to, const unsigned char* from) {
    for (int part = 0; part < 9; ++part) {
        reinterpret_cast<unsigned long long*>(to)[part] = reinterpret_cast<const unsigned long long*>(from)[part];
    }
}
}

extern "C" int createRandom__4itemPQ24item11descriptioniiii(unsigned char* out, int limit, int category, int extra, int level) {
    using namespace inventory_items;
    alignas(8) unsigned char item[72];
    char buffer[256];
    const unsigned char* table = norrath_item_table_view;
    int wizards = 0;
    for (int index = 0; index < nmPlayers; ++index) {
        if (*reinterpret_cast<int*>(player_record(index) + 80) == 4) ++wizards;
    }
    const float fraction = static_cast<float>(wizards) / static_cast<float>(nmPlayers);
    Filter filter = {category, level, 0, 0, -1, -1};
    if (category == 1) {
        const unsigned first = draw() % 65535u;
        if (static_cast<int>(fraction * 65536.0f) < static_cast<int>(first)) {
            filter.flag_a = 1;
        } else if (0.0f < fraction) {
            filter.flag_b = (draw() % 65535u) <= 0xbfffu ? 1 : 0;
        }
    }
    if (category == 0) {
        const float u = static_cast<float>(draw()) * bits(0x37800000);
        filter.kind_a = 0;
        if (!(u < bits(0x3d4ccccc))) {
            filter.kind_a = 2;
            if (u < bits(0x3dcccccc)) filter.kind_a = 1;
        }
    }
    if (category == 1) {
        const float u = static_cast<float>(draw()) * bits(0x37800000);
        filter.tier_b = 0;
        if (!(u < bits(0x3e23d70a))) {
            filter.tier_b = 1;
            if (!(u < bits(0x3ea3d70a))) {
                filter.tier_b = 2;
                if (!(u < bits(0x3ef5c28f))) {
                    filter.tier_b = 3;
                    if (!(u < bits(0x3f23d70a))) {
                        filter.tier_b = 4;
                        if (!(u < bits(0x3f4ccccc))) filter.tier_b = 5;
                    }
                }
            }
        }
    }
    int eligible = 0;
    if (word(table, 0) != 0) {
        for (const unsigned char* row = table; word(row, 0) != 0; row += 64) {
            if (filter.eligible(row)) ++eligible;
        }
    }
    if (eligible == 0) return 0;
    if (extra == -1) {
        if (category == 2) {
            extra = 1;
        } else {
            const unsigned r = draw();
            if (r < 6500u) extra = 1;
        }
    }
    int prefix_count = 0;
    if (word(_4item$prefixTable, 0) != 0) {
        for (const unsigned char* record = _4item$prefixTable; ; record += 40) {
            if (!(level < half(record, 4))) ++prefix_count;
            if (word(record + 40, 0) == 0) break;
        }
    }
    int suffix_count = 0;
    if (word(_4item$suffixTable, 0) != 0) {
        for (const unsigned char* record = _4item$suffixTable; word(record, 0) != 0; record += 20) ++suffix_count;
    }
    const int rounds = category == 0 ? 15 : 10;
    int best = -1;
    for (int round = 0; round < rounds; ++round) {
        for (int pair = 3; pair >= 0; --pair) *reinterpret_cast<short*>(item + 36 + 8 * pair) = -1;
        item[32] = 0xff;
        item[33] = 0xff;
        for (int offset = 16; offset < 24; offset += 2) {
            *reinterpret_cast<short*>(item + offset) = 0;
            *reinterpret_cast<short*>(item + 8 + offset) = 0;
        }
        int pick = static_cast<int>(draw() % static_cast<unsigned>(eligible));
        const unsigned char* row = table;
        for (;; row += 64) {
            if (!filter.eligible(row)) continue;
            if (--pick < 0) break;
        }
        const int row_category = static_cast<signed char>(row[29]);
        unsigned suffix = draw() % static_cast<unsigned>(suffix_count);
        while (level < half(_4item$suffixTable + 20 * suffix, 4)) suffix = draw() % static_cast<unsigned>(suffix_count);
        const unsigned suffix_text = word(_4item$suffixTable + 20 * suffix, 8 + 4 * row_category);
        if (category == 2) {
            strcpy(buffer, name_of(row));
            const char* appended = text_at(word(_4item$suffixTable, 8 + 4 * row_category));
            if (extra > 0 && suffix_text != 0) {
                if (strstr(name_of(row), "potion") == 0) appended = text_at(suffix_text);
            }
            strcat(buffer, appended);
            create__4itemPcPQ24item11descriptionii(buffer, item, -1, 0);
            item[32] = 0xff;
            item[33] = 0;
            if (extra > 0 && suffix_text != 0 && strstr(name_of(row), "potion") == 0) item[33] = static_cast<unsigned char>(suffix);
            const int value = getValue__4itemPQ24item11description(item);
            if (strstr(name_of(row), "potion") == 0) add_level_requirement(item, value);
            if (value < best) continue;
            if (limit < value) continue;
            copy_item(out, item);
            best = value;
        } else {
            for (int index = 0; index < prefix_count; ++index) {
                const unsigned char* prefix = _4item$prefixTable + 40 * index;
                if (level < half(prefix, 4)) continue;
                strcpy(buffer, name_of(row));
                if (round != 0) {
                    const unsigned long long mask = wide(prefix, 24);
                    if ((wide(row, 40) & mask) != 0 || mask == 0) {
                        const unsigned prefix_text = word(prefix, 12 + 4 * row_category);
                        if (prefix_text != 0) strcat(buffer, text_at(prefix_text));
                    }
                    if (extra > 0 && suffix_text != 0 && strstr(name_of(row), "potion") == 0) strcat(buffer, text_at(suffix_text));
                }
                create__4itemPcPQ24item11descriptionii(buffer, item, -1, 0);
                if (round != 0) {
                    const unsigned long long mask = wide(prefix, 24);
                    if ((wide(row, 40) & mask) != 0 || mask == 0) {
                        if (word(prefix, 12 + 4 * row_category) != 0) item[32] = static_cast<unsigned char>(index);
                    }
                    const unsigned second_mask = word(prefix, 32);
                    if (second_mask != 0) item[32] = (word(row, 32) & second_mask) != 0 ? static_cast<unsigned char>(index) : 0xff;
                    if (extra > 0 && suffix_text != 0 && strstr(name_of(row), "potion") == 0) item[33] = static_cast<unsigned char>(suffix);
                }
                const int value = getValue__4itemPQ24item11description(item);
                if (strstr(name_of(row), "potion") == 0) add_level_requirement(item, value);
                if (value < best) continue;
                if (limit < value) continue;
                copy_item(out, item);
                best = value;
            }
        }
    }
    if (best < 0) return 0;
    const unsigned char* chosen = resolve<unsigned char>(word(out, 0));
    int charges = static_cast<int>((wide(chosen, 40) >> 11) & 0x1f);
    if (charges > 0) charges = static_cast<int>(draw() % static_cast<unsigned>(charges));
    *reinterpret_cast<unsigned long long*>(out + 8) = (static_cast<unsigned long long>(static_cast<long long>(charges << 11))) | (wide(out, 8) & 0xffffffffffff07ffull);
    return 1;
}

// The same function under the name the weapon-rack constructor imports. On the target it is a
// genuine alias (one body); on the host build it forwards.
#if defined(__mips__)
extern "C" int norrath_item_create_random(unsigned char* out, int limit, int category, int extra, int level)
    __attribute__((alias("createRandom__4itemPQ24item11descriptioniiii")));
#else
extern "C" int norrath_item_create_random(unsigned char* out, int limit, int category, int extra, int level) {
    return createRandom__4itemPQ24item11descriptioniiii(out, limit, category, extra, level);
}
#endif
