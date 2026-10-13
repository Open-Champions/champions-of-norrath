// Host check for createRandomCombineableItem__4itemPQ24item11descriptionsPc. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -fno-builtin host_check.cpp
// Table rows and their text live in one byte array standing for the image: a 32-bit "address" is an offset
// from kBase, and the host maps it back through norrath_ccri_pointer. The libc strcmp and strcpy are used as is.
#define NORRATH_HOST_CONTRACT
#include "create_random_combineable_item.cpp"

#include <cstdio>
#include <cstring>
#include <string>

static const unsigned kBase = 0x32b320;
extern "C" {
unsigned char norrath_item_table_view[0x4000];
int gameDifficulty;
unsigned syncRandState;
unsigned char* norrath_ccri_pointer(unsigned word) { return norrath_item_table_view + (word - kBase); }
}

static std::string trace;
static int createCalls;
static unsigned createRow;      // address placed in word 0 of the produced description
static unsigned long long createWord8;

extern "C" void create__4itemPcPQ24item11descriptionii(char* name, void* description, int a, int b) {
    ++createCalls;
    trace += std::string("C") + name + "," + std::to_string(a) + "," + std::to_string(b) + ";";
    unsigned char* item = static_cast<unsigned char*>(description);
    // the caller must have prepared the template before the call
    for (int i = 0; i < 4; ++i)
        if (item[36 + 8 * i] != 0xff || item[37 + 8 * i] != 0xff) trace += "T?";
    if (item[32] != 0xff || item[33] != 0xff) trace += "B?";
    for (int i = 0; i < 4; ++i)
        if (item[16 + 2 * i] != 0 || item[17 + 2 * i] != 0 || item[24 + 2 * i] != 0 || item[25 + 2 * i] != 0) trace += "H?";
    std::memset(item, 0x5a, 72);
    std::memcpy(item, &createRow, 4);
    std::memcpy(item + 8, &createWord8, 8);
}

static unsigned char* row_at(int index) { return norrath_item_table_view + 64 * index; }
static unsigned text_cursor = 0x2000;

static unsigned put_text(const char* text) {
    unsigned address = kBase + text_cursor;
    std::strcpy(reinterpret_cast<char*>(norrath_item_table_view + text_cursor), text);
    text_cursor += static_cast<unsigned>(std::strlen(text)) + 1;
    return address;
}

static void put_u32(unsigned char* p, unsigned v) { std::memcpy(p, &v, 4); }
static void put_u64(unsigned char* p, unsigned long long v) { std::memcpy(p, &v, 8); }

static void set_row(int index, const char* name, int minimum, int category, unsigned long long flags, const char* second) {
    unsigned char* row = row_at(index);
    std::memset(row, 0, 64);
    put_u32(row, put_text(name));
    row[4] = static_cast<unsigned char>(minimum);
    row[5] = static_cast<unsigned char>(minimum >> 8);
    row[29] = static_cast<unsigned char>(category);
    put_u64(row + 40, flags);
    put_u32(row + 48, put_text(second));
}

static unsigned reference_draw(unsigned& state) {
    state = state * 1664525u + 1013904223u;
    return state >> 16;
}

static const unsigned long long kFlag35 = 1ull << 35;

static void reset(int rows) {
    std::memset(norrath_item_table_view, 0, sizeof norrath_item_table_view);
    text_cursor = 0x2000;
    (void)rows;
    trace.clear();
    createCalls = 0;
    gameDifficulty = 0;
    createRow = kBase + 64 * 4;    // row 4 is used as the row named by the produced description
    createWord8 = 0xfedcba9876543210ull;
}

int main() {
    unsigned char out[72];
    // Rows: 0 usable, 1 flag 0x10000, 2 wrong category, 3 minimum too high for level 20, 4 usable and carrying
    // the roll count 5 (flags bits 11..15), 5 usable but missing flag bit 35, 6 usable (minimum 17).
    // no difficulty floor: rows 0, 4, 6 qualify, so three candidates
    for (unsigned seed : {1u, 2u, 3u, 0xdeadbeefu}) {
        reset(7);
        set_row(0, "water", 5, 2, kFlag35, "a");
        set_row(1, "wine", 5, 2, kFlag35 | 0x10000, "b");
        set_row(2, "beer", 5, 1, kFlag35, "a");
        set_row(3, "ale", 25, 2, kFlag35, "a");
        set_row(4, "mead", 16, 2, kFlag35 | (5ull << 11), "b");
        set_row(5, "milk", 5, 2, 0, "a");
        set_row(6, "tea", 17, 2, kFlag35, "a");
        syncRandState = seed;
        unsigned state = seed;
        unsigned pick = reference_draw(state) % 3;
        static const char* names[3] = {"water", "mead", "tea"};
        std::memset(out, 0xee, sizeof out);
        int result = createRandomCombineableItem__4itemPQ24item11descriptionsPc(out, 20, nullptr);
        if (result != 1) return 1;
        if (trace != std::string("C") + names[pick] + ",-1,0;") { std::printf("%s\n", trace.c_str()); return 2; }
        // the output is the 72 bytes produced by create, with bits 11..15 of the word at +8 replaced
        unsigned long long expected = createWord8 & ~(0x1full << 11);
        unsigned long long second = 0;
        (void)second;
        // the row named by word 0 is row 4, whose flags carry 5 in bits 11..15: one more draw modulo 5
        expected |= static_cast<unsigned long long>(reference_draw(state) % 5) << 11;
        unsigned long long got;
        std::memcpy(&got, out + 8, 8);
        if (got != expected) return 3;
        if (out[20] != 0x5a || out[71] != 0x5a || out[4] != 0x5a) return 4;
        if (syncRandState != state) return 5;
    }
    // a roll count of zero draws nothing more and clears the field
    reset(7);
    set_row(0, "water", 5, 2, kFlag35, "a");
    set_row(1, "mead", 16, 2, kFlag35, "b");
    createRow = kBase + 64 * 1;
    syncRandState = 77;
    {
        unsigned state = 77;
        reference_draw(state);
        createWord8 = 0xffffffffffffffffull;
        if (createRandomCombineableItem__4itemPQ24item11descriptionsPc(out, 20, nullptr) != 1) return 6;
        unsigned long long got;
        std::memcpy(&got, out + 8, 8);
        if (got != 0xffffffffffff07ffull || syncRandState != state) return 7;
    }
    // difficulty 3 raises the floor to 27, difficulty 2 to 17: row minimums below it are not eligible
    reset(7);
    set_row(0, "water", 16, 2, kFlag35, "a");
    set_row(1, "wine", 26, 2, kFlag35, "a");
    set_row(2, "beer", 27, 2, kFlag35, "a");
    set_row(3, "mead", 28, 2, kFlag35, "b");
    gameDifficulty = 3; syncRandState = 5;
    if (createRandomCombineableItem__4itemPQ24item11descriptionsPc(out, 40, nullptr) != 1) return 8;
    {
        unsigned state = 5;
        const char* names[2] = {"beer", "mead"};
        if (trace != std::string("C") + names[reference_draw(state) % 2] + ",-1,0;") { std::printf("[%s]\n", trace.c_str()); return 9; }
    }
    reset(7);
    set_row(0, "water", 16, 2, kFlag35, "a");
    set_row(1, "wine", 17, 2, kFlag35, "a");
    set_row(2, "mead", 28, 2, kFlag35, "b");
    gameDifficulty = 2; syncRandState = 7;
    if (createRandomCombineableItem__4itemPQ24item11descriptionsPc(out, 40, nullptr) != 1) return 10;
    {
        unsigned state = 7;
        const char* names[2] = {"wine", "mead"};
        if (trace != std::string("C") + names[reference_draw(state) % 2] + ",-1,0;") return 11;
    }
    // a level below every minimum, or an empty table: result 0, no draw, nothing created
    reset(7);
    set_row(0, "water", 10, 2, kFlag35, "a");
    syncRandState = 9;
    if (createRandomCombineableItem__4itemPQ24item11descriptionsPc(out, 9, nullptr) != 0 || createCalls != 0 || syncRandState != 9) return 12;
    if (createRandomCombineableItem__4itemPQ24item11descriptionsPc(out, -3, nullptr) != 0) return 13;
    // a named request considers only rows whose second text equals the name; flag 0x10000 does not exclude here
    reset(7);
    set_row(0, "water", 5, 2, kFlag35, "other");
    set_row(1, "wine", 5, 2, kFlag35 | 0x10000, "wanted");
    set_row(2, "beer", 5, 2, kFlag35, "wanted");
    set_row(3, "ale", 5, 1, kFlag35, "wanted");
    set_row(4, "mead", 5, 2, 0, "wanted");
    syncRandState = 11;
    {
        unsigned state = 11;
        const char* names[2] = {"wine", "beer"};
        char request[] = "wanted";
        if (createRandomCombineableItem__4itemPQ24item11descriptionsPc(out, 5, request) != 1) return 14;
        if (trace != std::string("C") + names[reference_draw(state) % 2] + ",-1,0;") return 15;
    }
    reset(7);
    set_row(0, "water", 5, 2, kFlag35, "other");
    syncRandState = 12;
    {
        char request[] = "wanted";
        if (createRandomCombineableItem__4itemPQ24item11descriptionsPc(out, 5, request) != 0 || createCalls != 0) return 16;
    }
    // the level argument is a signed halfword: 0xffff-style values from the caller mean -1
    reset(7);
    set_row(0, "water", -1, 2, kFlag35, "a");
    syncRandState = 13;
    if (createRandomCombineableItem__4itemPQ24item11descriptionsPc(out, -1, nullptr) != 1) return 17;
    std::puts("create random combineable item passed");
    return 0;
}
