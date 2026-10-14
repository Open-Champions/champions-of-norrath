// Observed playerInit__FiiPcScScScScT2 at 0x001a1630, 1640 bytes. Only byte
// offsets of the 23984-byte player record at `player` (0x00408600) are
// recorded; none of the fields is named. Arguments are, in order: record index,
// a word stored at +84, a name copied to +0, four bytes stored at +32, +33,
// +34 and +35 in argument order, and an optional four-byte source for the words
// at +120..+132 ("tint" here is only a label for that source). The reconstruction follows the image
// in this order:
//  1. The record is cleared, bytes +168, +231 and +23982 are set to 0xaa, and
//     the 64 words from +23712 to +23968 are set to 0xffffffff.
//  2. +84 takes the second argument. +80 takes 0 for values 0 and 1, then by
//     unsigned test: 8 and 9 give 2, 4 and 5 give 1, 2 and 3 give 3, 6 and 7
//     give 4, and every other value gives 3.
//  3. The seven words of this record's row in fadeTimer and in fadeIconY
//     (0x0034b7b8 and 0x0034b828, 28 bytes per record) are cleared.
//  4. Three 64-entry lists of words at +328, +4944 and +9560, 72 bytes apart,
//     are cleared, as are the words at +4936, +9552 and +14168.
//  5. A fixed block of values is stored: the five bytes "None" at +36, floats
//     10.0 at +196 and +200, the float bits 0x3d999999 at +184 and +188, 125.0 at +312, the word
//     500 at +232, 1 at +242, and 64 zero bytes ending at +306.
//  6. A second fixed block of small values is stored (see the code), followed
//     by a loop of 16 zero words ending at +21956 and 2 ending at +21964, and
//     15 entries 104 bytes apart from +22008 with halfword -1 and a zero byte.
//  7. +172 takes the float at +176 and +196 takes the float at +200. With a
//     null tint source +120..+132 are the four words of the abilityTable row
//     (0x00422aa8, 16 bytes) chosen by the word at +80; otherwise they are the
//     four sign-extended bytes of the tint source.
//  8. playerRecomputeWeight(index) runs, the name is copied, +32..+35 are
//     stored, then +172 takes 53.0 and +196 takes 16.0, 35.0, 25.0, 21.0 or
//     40.0 for the word at +80 equal to 0, 1, 2, 3 or 4 (no store for others).
//  9. playerRecomputeAbilityModifiers() and setupDefaultEquipment(index) run.
// Observed for the image only; the meaning of the stored values is unknown.
typedef int __attribute__((may_alias)) Word;
typedef unsigned short __attribute__((may_alias)) Half;
typedef float __attribute__((may_alias)) Single;

extern "C" unsigned char player[];
extern "C" int fadeTimer[];
extern "C" int fadeIconY[];
extern "C" int abilityTable[];

extern "C" void* memset(void* destination, int value, __SIZE_TYPE__ count);
extern "C" char* strcpy(char* destination, const char* source);
extern "C" void playerRecomputeWeight__Fi(int index);
extern "C" void playerRecomputeAbilityModifiers__Fv();
extern "C" void setupDefaultEquipment__Fi(int index);

static inline void put_word(unsigned char* base, unsigned offset, int value) {
    *reinterpret_cast<Word*>(base + offset) = value;
}
static inline void put_half(unsigned char* base, unsigned offset, unsigned value) {
    *reinterpret_cast<Half*>(base + offset) = static_cast<unsigned short>(value);
}
static inline void put_single(unsigned char* base, unsigned offset, float value) {
    *reinterpret_cast<Single*>(base + offset) = value;
}
static inline int get_word(const unsigned char* base, unsigned offset) {
    return *reinterpret_cast<const Word*>(base + offset);
}
static inline float get_single(const unsigned char* base, unsigned offset) {
    return *reinterpret_cast<const Single*>(base + offset);
}

extern "C" void playerInit__FiiPcScScScScT2(int index, int second, char* name, signed char first_byte,
                                            signed char second_byte, signed char third_byte,
                                            signed char fourth_byte, char* tint) {
    unsigned char* const record = player + index * 23984;
    memset(record, 0, 23984);

    record[23982] = 0xaa;
    record[231] = 0xaa;
    record[168] = 0xaa;
    for (int i = 0; i < 64; ++i) {
        put_half(record, 23712u + i * 4u, 0xffffu);
        put_half(record, 23714u + i * 4u, 0xffffu);
    }

    put_word(record, 84, second);
    if (static_cast<unsigned>(second) < 2u) {
        put_word(record, 80, 0);
    } else {
        int kind;
        if (static_cast<unsigned>(second - 8) < 2u) {
            kind = 2;
        } else if (static_cast<unsigned>(second - 4) < 2u) {
            kind = 1;
        } else if (static_cast<unsigned>(second - 2) < 2u) {
            kind = 3;
        } else if (static_cast<unsigned>(second - 6) < 2u) {
            kind = 4;
        } else {
            kind = 3;
        }
        put_word(record, 80, kind);
    }

    for (int i = 6; i >= 0; --i) {
        fadeTimer[index * 7 + (6 - i)] = 0;
        fadeIconY[index * 7 + (6 - i)] = 0;
    }

    for (int entry = 0; entry < 64; ++entry) {
        put_word(record, 9560u + entry * 72u, 0);
        put_word(record, 4944u + entry * 72u, 0);
        put_word(record, 328u + entry * 72u, 0);
    }

    put_word(record, 4936, 0);
    put_word(record, 9552, 0);
    put_word(record, 14168, 0);
    __builtin_memcpy(record + 36, "None", 5);
    put_single(record, 200, 10.0f);
    put_word(record, 204, 0);
    put_single(record, 196, 10.0f);
    put_word(record, 232, 500);
    put_word(record, 188, 0x3d999999);  // the image float is one unit below 0.075f
    put_word(record, 184, 0x3d999999);
    put_single(record, 312, 125.0f);
    put_word(record, 308, 0);
    record[242] = 1;
    record[240] = 0;
    record[241] = 0;
    for (int i = 0; i < 64; ++i) record[306 - i] = 0;

    record[228] = 0;
    record[229] = 0;
    record[230] = 0;
    put_word(record, 224, 0);
    put_word(record, 208, 11);
    put_word(record, 216, 2);
    put_word(record, 212, 1);
    put_word(record, 220, 1);
    put_word(record, 180, 0);
    put_word(record, 92, 0);
    record[89] = 1;
    record[88] = 1;
    put_word(record, 17760, 0);
    put_word(record, 21872, 0);
    put_word(record, 21868, -1);
    put_word(record, 21864, 0);
    record[23582] = 5;
    for (int i = 0; i < 16; ++i) put_word(record, 21956u - i * 4u, 0);

    put_word(record, 23536, 0);
    record[23574] = 0;
    put_word(record, 23540, 0);
    put_word(record, 23576, 0);
    record[23572] = 0;
    record[23580] = 0;
    record[23581] = 1;
    put_word(record, 23564, 0);
    put_word(record, 23568, 0);
    for (int i = 0; i < 2; ++i) put_word(record, 21964u - i * 4u, 0);

    for (int i = 0; i < 15; ++i) {
        record[22012u + i * 104u] = 0;
        put_half(record, 22008u + i * 104u, 0xffffu);
    }

    put_word(record, 23556, 0);
    put_single(record, 172, get_single(record, 176));
    put_word(record, 23548, 0);
    put_word(record, 23560, 0);
    put_single(record, 196, get_single(record, 200));
    put_word(record, 23552, 0);
    put_word(record, 308, 0);

    if (tint != 0) {
        put_word(record, 120, tint[0]);
        put_word(record, 124, tint[1]);
        put_word(record, 128, tint[2]);
        put_word(record, 132, tint[3]);
    } else {
        put_word(record, 120, abilityTable[get_word(record, 80) * 4 + 0]);
        put_word(record, 124, abilityTable[get_word(record, 80) * 4 + 1]);
        put_word(record, 128, abilityTable[get_word(record, 80) * 4 + 2]);
        put_word(record, 132, abilityTable[get_word(record, 80) * 4 + 3]);
    }

    playerRecomputeWeight__Fi(index);
    strcpy(reinterpret_cast<char*>(record), name);
    record[35] = static_cast<unsigned char>(fourth_byte);
    record[32] = static_cast<unsigned char>(first_byte);
    record[33] = static_cast<unsigned char>(second_byte);
    record[34] = static_cast<unsigned char>(third_byte);

    const int kind = get_word(record, 80);
    if (kind == 0) {
        put_single(record, 172, 53.0f);
        put_single(record, 196, 16.0f);
    } else if (kind == 2) {
        put_single(record, 172, 53.0f);
        put_single(record, 196, 25.0f);
    } else if (kind == 1) {
        put_single(record, 172, 53.0f);
        put_single(record, 196, 35.0f);
    } else if (kind == 3) {
        put_single(record, 172, 53.0f);
        put_single(record, 196, 21.0f);
    } else if (kind == 4) {
        put_single(record, 172, 53.0f);
        put_single(record, 196, 40.0f);
    }

    playerRecomputeAbilityModifiers__Fv();
    setupDefaultEquipment__Fi(index);
}
