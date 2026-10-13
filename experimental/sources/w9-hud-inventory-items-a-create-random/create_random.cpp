// Observed createRandom__4itemPQ24item11descriptioniiii at 0x00104440, 4220 bytes, in the
// pinned SLUS_205.65. Arguments after the output description (72 bytes; its first word is a
// pointer to a row of the item table): limit, mode, extra, level. The callers in this
// package's neighbours pass (description, a rolled word, a random bit, -1, a difficulty).
// Result: 1 when a candidate was stored into the output, else 0.
//
// Tables used (all in the image's data):
//  - item table at 0x32b320, 64-byte rows ending with a row whose first word is zero. Fields
//    used: +0 name text, +4 signed halfword minimum level, +28/+29/+30 signed bytes (+29 is
//    the category compared with mode), +32 word, +40 64-bit flag word.
//  - prefix table at 0x331e60, 40-byte rows ending with a zero first word: +0 text, +4 signed
//    halfword minimum level, +12 three text pointers indexed by category, +24 a 64-bit mask,
//    +32 a word mask.
//  - suffix table at 0x332770, 20-byte rows ending with a zero first word: +0 text, +4 signed
//    halfword minimum level, +8 three text pointers indexed by category.
// The word syncRandState advances as state = state * 0x19660d + 0x3c6ef35f and the value
// used is state >> 16 (an unsigned shift).
//
// Steps:
//  1. Counts the player records (23984 bytes at the player array) whose word at +80 is 4;
//     frac = count / nmPlayers as a float. flagA and flagB start at 0.
//     mode 1: one step; when (int)(frac * 65536) < (step % 0xffff) flagA = 1; otherwise when
//     frac > 0 a second step sets flagB = ((step % 0xffff) <= 0xbfff).
//     mode 0: one step scaled by 2^-16 to a float u; classKind = 0 when u < c0, 1 when u < c1,
//     else 2. mode 1 (also): another step scaled the same way gives slotKind 0..5 by the five
//     float cut points c2..c6. Other modes leave both at -1.
//  2. Eligible rows are counted, then one is picked by a step modulo the count; the same
//     eligibility test is used in both passes. A row is skipped when its flag word has bit 16
//     or bit 35, when mode is not -1 and its category differs, or when the level argument is
//     below its minimum level. flagB: a category 1 row without flag bit 19 must contain the
//     text "circlet". flagA: a category 1 row with flag bit 19 is skipped. Category 0 rows must
//     map to classKind (byte +30 equal to 6 gives 0; +30 not 3 gives 2; +30 equal to 3 gives 1
//     when word +32 has bit 3, else 2). Category 1 rows must map to slotKind from the byte at
//     +28 (11:0, 4:1, 9:2, 12:3, 3:4, others:5). No eligible row returns 0.
//  3. When extra is -1 it becomes 1 for mode 2, otherwise 1 when a step's high part is below
//     6500 (else stays -1).
//  4. n1 = prefix rows with minimum level <= level; n2 = suffix rows. Attempts: 15 for mode 0,
//     else 10. Each attempt builds a candidate description (option halfwords at +36, +44,
//     +52, +60 set to -1, bytes +32 and +33 set to -1, halfwords 16..31 zeroed), picks the
//     row, then picks a suffix row by a step modulo n2, repeating the roll while the row's
//     minimum level is above the level argument.
//     mode 2: text = row name, then the text of suffix row 0 for the category (or, when
//     extra > 0, the picked suffix row's text if it is set and the name has no "potion")
//     appended; create(text, desc, -1, 0); byte +32 = -1; byte +33 = the suffix index when
//     extra > 0 and that text was set and the name has no "potion", else 0.
//     other modes: for each prefix row p < n1 whose minimum level is <= level: text = row
//     name; for attempts after the first the prefix text for the category is appended when
//     the row's flags and the prefix mask intersect; when extra > 0 the picked suffix text is
//     appended when set and the name has no "potion"; create; then (attempts after the first)
//     byte +32 and +33 are set from the prefix and suffix choices as the code below spells out.
//     Each candidate is valued with getValue; when the name has no "potion" a power of
//     (max(value - 1200, 0) / 2000) with a float exponent gives an option value
//     min(2 * int + 1, 50) stored in the first option slot that is -1 or 14 (set to id 14).
//     The candidate replaces the output when its value is >= the best so far (starting at -1)
//     and <= limit.
//  5. If no candidate was stored return 0. Otherwise the 5-bit field (bits 11..15) of the
//     output's 64-bit word at +8 is replaced by a step modulo the item row's count field
//     (flag word >> 11 & 0x1f), or 0 when that count is not positive; return 1.
// Unknown: the meaning of categories, kinds, the modes, and the flag bits.
// Not executed on hardware; callees and objects are defined elsewhere.
#ifdef NORRATH_HOST_CONTRACT
extern unsigned char* norrath_host_base;
#define NORRATH_PTR(w) (norrath_host_base + (w))
#else
#define NORRATH_PTR(w) reinterpret_cast<unsigned char*>(w)
#endif
// Pointers inside the tables and the description are 32-bit words.
extern "C" {
struct ItemRow {
    unsigned name;
    short level;
    unsigned char gap6[22];
    signed char detail;
    signed char category;
    signed char sub;
    unsigned char gap31;
    unsigned word32;
    unsigned gap36;
    unsigned long long flags;
    unsigned char tail[16];
};
struct PrefixRow {
    unsigned head;
    short level;
    unsigned char gap6[2];
    unsigned gap8;
    unsigned text[3];
    unsigned long long mask;
    unsigned word32;
    unsigned gap36;
};
struct SuffixRow {
    unsigned head;
    short level;
    unsigned char gap6[2];
    unsigned text[3];
};
struct Description {
    unsigned long long w[9];
};
extern ItemRow itemTable[] asm("_4item$itemTable");
extern PrefixRow prefixTable[] asm("_4item$prefixTable");
extern SuffixRow suffixTable[] asm("_4item$suffixTable");
extern unsigned syncRandState;
extern int nmPlayers;
extern unsigned char player[];
char* strcpy(char*, const char*);
char* strcat(char*, const char*);
char* strstr(const char*, const char*);
float powf(float, float);
void create__4itemPcPQ24item11descriptionii(char* name, Description* out, int kind, int flags);
int getValue__4itemPQ24item11description(Description* description);
}

static inline float single_from_bits(unsigned bits) {
    union { unsigned u; float f; } v;
    v.u = bits;
    return v.f;
}

static inline const char* textAt(unsigned word) { return reinterpret_cast<const char*>(NORRATH_PTR(word)); }

static unsigned step() {
    syncRandState = syncRandState * 0x19660du + 0x3c6ef35fu;
    return syncRandState >> 16;
}

static inline unsigned char* bytes(Description& d) { return reinterpret_cast<unsigned char*>(&d); }
static inline short& half(Description& d, int offset) { return *reinterpret_cast<short*>(bytes(d) + offset); }

static bool isPotion(const ItemRow& row) { return strstr(textAt(row.name), "potion") != 0; }

static int classOf(const ItemRow& row) {
    if (row.category != 0)
        return -1;
    if (row.sub == 6)
        return 0;
    if (row.sub != 3)
        return 2;
    if (row.word32 & 8)
        return 1;
    return 2;
}

static int slotOf(const ItemRow& row) {
    if (row.category != 1)
        return -1;
    switch (row.detail) {
    case 11: return 0;
    case 4: return 1;
    case 9: return 2;
    case 12: return 3;
    case 3: return 4;
    default: return 5;
    }
}

static bool eligible(const ItemRow& row, int mode, int level, int flagA, int flagB, int classKind, int slotKind) {
    if (row.flags & 0x10000ull)
        return false;
    if (mode != -1 && row.category != mode)
        return false;
    if (level < row.level)
        return false;
    if (row.flags & (1ull << 35))
        return false;
    if (flagB != 0 && row.category == 1 && !(row.flags & 0x80000ull) && !strstr(textAt(row.name), "circlet"))
        return false;
    if (flagA != 0 && row.category == 1 && (row.flags & 0x80000ull))
        return false;
    if (row.category == 0 && classOf(row) != classKind)
        return false;
    if (row.category == 1 && slotOf(row) != slotKind)
        return false;
    return true;
}

static void clearCandidate(Description& d) {
    for (int i = 0; i < 4; ++i)
        half(d, 60 - 8 * i) = -1;
    bytes(d)[32] = 0xff;
    bytes(d)[33] = 0xff;
    for (int i = 0; i < 4; ++i) {
        half(d, 16 + 2 * i) = 0;
        half(d, 24 + 2 * i) = 0;
    }
}

static void addOption(Description& d, int value) {
    for (int i = 0; i < 4; ++i) {
        short* slot = reinterpret_cast<short*>(bytes(d) + 36 + 8 * i);
        if (slot[0] == -1 || slot[0] == 14) {
            slot[0] = 14;
            slot[1] = static_cast<short>(value);
            slot[2] = 0;
            slot[3] = 0;
            return;
        }
    }
}

static void optionFromValue(Description& d, int value, float exponent) {
    int base = value - 1200;
    if (base < 0)
        base = 0;
    int power = static_cast<int>(powf(static_cast<float>(base / 2000), exponent));
    int option = power * 2 + 1;
    if (option >= 51)
        option = 50;
    addOption(d, option);
}

extern "C" int createRandom__4itemPQ24item11descriptioniiii(Description* out, int limit, int mode, int extra, int level) {
    int eligibleCount = 0;
    int flagA = 0;
    int flagB = 0;
    int fours = 0;
    for (int i = 0; i < nmPlayers; ++i)
        if (*reinterpret_cast<int*>(player + 80 + 23984 * i) == 4)
            ++fours;
    const float frac = static_cast<float>(fours) / static_cast<float>(nmPlayers);
    if (mode == 1) {
        const unsigned roll = step();
        if (static_cast<int>(frac * 65536.0f) < static_cast<int>(roll % 0xffffu)) {
            flagA = 1;
        } else if (0.0f < frac) {
            const unsigned second = step();
            flagB = (0xbfffu < second % 0xffffu) ? 0 : 1;
        }
    }
    int classKind = -1;
    int slotKind = -1;
    if (mode == 0) {
        classKind = 0;
        const float u = static_cast<float>(step()) * single_from_bits(0x37800000);
        if (!(u < single_from_bits(0x3d4ccccc))) {
            classKind = 2;
            if (u < single_from_bits(0x3dcccccc))
                classKind = 1;
        }
    } else if (mode == 1) {
        slotKind = 0;
        const float u = static_cast<float>(step()) * single_from_bits(0x37800000);
        if (!(u < single_from_bits(0x3e23d70a))) {
            slotKind = 1;
            if (!(u < single_from_bits(0x3ea3d70a))) {
                slotKind = 2;
                if (!(u < single_from_bits(0x3ef5c28f))) {
                    slotKind = 3;
                    if (!(u < single_from_bits(0x3f23d70a))) {
                        slotKind = 4;
                        if (!(u < single_from_bits(0x3f4ccccc)))
                            slotKind = 5;
                    }
                }
            }
        }
    }
    for (int i = 0; itemTable[i].name; ++i)
        if (eligible(itemTable[i], mode, level, flagA, flagB, classKind, slotKind))
            ++eligibleCount;
    if (eligibleCount == 0)
        return 0;
    if (extra == -1) {
        if (mode == 2) {
            extra = 1;
        } else {
            const unsigned roll = step();
            if (roll < 6500u)
                extra = 1;
        }
    }
    int prefixCount = 0;
    for (int i = 0; prefixTable[i].head; ++i)
        if (!(level < prefixTable[i].level))
            ++prefixCount;
    int suffixCount = 0;
    for (int i = 0; suffixTable[i].head; ++i)
        ++suffixCount;
    int best = -1;
    const int attempts = mode == 0 ? 15 : 10;
    for (int attempt = 0; attempt < attempts; ++attempt) {
        Description candidate;
        clearCandidate(candidate);
        int pick = static_cast<int>(step() % static_cast<unsigned>(eligibleCount));
        int row = 0;
        for (;; ++row) {
            if (!eligible(itemTable[row], mode, level, flagA, flagB, classKind, slotKind))
                continue;
            if (--pick < 0)
                break;
        }
        const ItemRow& item = itemTable[row];
        int suffix = static_cast<int>(step() % static_cast<unsigned>(suffixCount));
        while (level < suffixTable[suffix].level)
            suffix = static_cast<int>(step() % static_cast<unsigned>(suffixCount));
        const int category = item.category;
        const unsigned suffixText = suffixTable[suffix].text[category];
        char text[256];
        if (mode == 2) {
            strcpy(text, textAt(item.name));
            unsigned chosen = suffixTable[0].text[category];
            if (extra > 0 && suffixText != 0 && !isPotion(item))
                chosen = suffixText;
            strcat(text, textAt(chosen));
            create__4itemPcPQ24item11descriptionii(text, &candidate, -1, 0);
            bytes(candidate)[32] = 0xff;
            bytes(candidate)[33] = (extra > 0 && suffixText != 0 && !isPotion(item)) ? suffix : 0;
            const int value = getValue__4itemPQ24item11description(&candidate);
            if (!isPotion(item))
                optionFromValue(candidate, value, single_from_bits(0x3f28f5c2));
            if (value < best || limit < value)
                continue;
            *out = candidate;
            best = value;
            continue;
        }
        for (int p = 0; p < prefixCount; ++p) {
            const PrefixRow& prefix = prefixTable[p];
            if (level < prefix.level)
                continue;
            strcpy(text, textAt(item.name));
            if (attempt != 0) {
                if (prefix.mask == 0 || (item.flags & prefix.mask) != 0) {
                    const unsigned word = prefix.text[category];
                    if (word != 0)
                        strcat(text, textAt(word));
                }
                if (extra > 0 && suffixText != 0 && !isPotion(item))
                    strcat(text, textAt(suffixText));
            }
            create__4itemPcPQ24item11descriptionii(text, &candidate, -1, 0);
            if (attempt != 0) {
                if (prefix.mask == 0 || (item.flags & prefix.mask) != 0) {
                    if (prefix.text[category] != 0)
                        bytes(candidate)[32] = static_cast<unsigned char>(p);
                }
                if (prefix.word32 != 0)
                    bytes(candidate)[32] = (item.word32 & prefix.word32) ? static_cast<unsigned char>(p) : 0xff;
                if (extra > 0 && suffixText != 0 && !isPotion(item))
                    bytes(candidate)[33] = static_cast<unsigned char>(suffix);
            }
            const int value = getValue__4itemPQ24item11description(&candidate);
            if (!isPotion(item))
                optionFromValue(candidate, value, single_from_bits(0x3f28f5c2));
            if (value < best || limit < value)
                continue;
            *out = candidate;
            best = value;
        }
    }
    if (best < 0)
        return 0;
    const unsigned countField = static_cast<unsigned>(reinterpret_cast<ItemRow*>(NORRATH_PTR(*reinterpret_cast<unsigned*>(out)))->flags >> 11) & 0x1f;
    int field = 0;
    if (static_cast<int>(countField) > 0)
        field = static_cast<int>(step() % countField);
    out->w[1] = (out->w[1] & 0xffffffffffff07ffull) | (static_cast<unsigned long long>(field) << 11);
    return 1;
}
