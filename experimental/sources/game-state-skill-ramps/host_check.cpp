// Host contract for gameInitSkillRamps. Not an EE plan input.
#define NORRATH_HOST_CONTRACT
#include "skill_ramps.cpp"

#include <cstdio>
#include <cstring>

static int failures = 0;
static unsigned char item_one[128];
static unsigned char item_two_storage[160];
static unsigned char* item_two = item_two_storage + 16;
static unsigned char table_one[96];
static unsigned char table_two[96];
static unsigned char* calls[4];
static unsigned called_words[4];
static int call_count = 0;

static void expect(int ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

static void store_u32(unsigned char* address, unsigned value) {
    address[0] = static_cast<unsigned char>(value);
    address[1] = static_cast<unsigned char>(value >> 8);
    address[2] = static_cast<unsigned char>(value >> 16);
    address[3] = static_cast<unsigned char>(value >> 24);
}

static void store_u16(unsigned char* address, unsigned value) {
    address[0] = static_cast<unsigned char>(value);
    address[1] = static_cast<unsigned char>(value >> 8);
}

extern "C" unsigned skills[8] = {};

extern "C" unsigned char* norrath_host_word_pointer(unsigned word) {
    if (word == 0x1000u) return item_one;
    if (word == 0x2000u) return item_two;
    if (word == 0x5000u) return table_one;
    if (word == 0x6000u) return table_two;
    std::fprintf(stderr, "unexpected pointer word %u\n", word);
    ++failures;
    return item_one;
}

extern "C" void norrath_host_call_word(unsigned word, unsigned char* argument) {
    if (call_count < 4) {
        called_words[call_count] = word;
        calls[call_count] = argument;
    }
    ++call_count;
}

int main() {
    std::memset(item_one, 0, sizeof item_one);
    std::memset(item_two_storage, 0, sizeof item_two_storage);
    std::memset(table_one, 0, sizeof table_one);
    std::memset(table_two, 0, sizeof table_two);
    store_u32(item_one + 96, 0x5000u);
    store_u32(item_two + 96, 0x6000u);
    store_u16(table_one + 80, 4u);
    store_u32(table_one + 84, 0x10u);
    store_u16(table_two + 80, 0xfffeu);
    store_u32(table_two + 84, 0x20u);

    skills[0] = 0x11111111u;
    skills[1] = 0u;
    skills[2] = 0x33333333u;
    call_count = 0;
    gameInitSkillRamps__Fv();
    expect(call_count == 0 && failures == 0, "a zero word at skills+4 stops before the first word");

    skills[1] = 0x1000u;
    skills[2] = 0x2000u;
    skills[3] = 0u;
    skills[4] = 0x33333333u;
    call_count = 0;
    gameInitSkillRamps__Fv();
    expect(call_count == 2, "two entries are called and the zero word stops the walk");
    expect(called_words[0] == 0x10u && calls[0] == item_one + 4, "first call uses adjust 4");
    expect(called_words[1] == 0x20u && calls[1] == item_two - 2, "second call uses adjust -2");
    expect(failures == 0, "the skipped first word and the word after zero are not resolved");

    if (failures != 0) return 1;
    std::puts("gameInitSkillRamps contract passed");
    return 0;
}
