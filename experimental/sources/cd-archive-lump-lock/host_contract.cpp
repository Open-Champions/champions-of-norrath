#include <cstdio>
extern "C" unsigned char norrath_constructor_lump_cache_view[128 * 40] = {};
extern "C" unsigned char norrath_constructor_lump_count_view[4] = {};
static int locks = 0;
static int locked_value = 0;
static void put_word(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}
static unsigned word_at(const unsigned char* slot) {
    return slot[0] | (slot[1] << 8) | (slot[2] << 16) | (slot[3] << 24);
}
extern "C" void soundLock__Fi(int value) {
    locks += 1;
    locked_value = value;
}
extern "C" void lumpLock__Fi(int value);
int main() {
    unsigned char* cache = norrath_constructor_lump_cache_view;
    put_word(cache + 32, 10u);
    put_word(cache + 40 + 32, 80u);
    put_word(norrath_constructor_lump_count_view, 2u);
    lumpLock__Fi(50);
    if (word_at(cache + 32) != 50u || word_at(cache + 72) != 80u) return 1;
    if (locks != 1 || locked_value != 50) return 2;
    put_word(norrath_constructor_lump_count_view, 0u);
    put_word(cache + 32, 1u);
    lumpLock__Fi(9);
    if (word_at(cache + 32) != 1u || locks != 2 || locked_value != 9) return 3;
    put_word(cache + 32, 0xffffffffu);
    put_word(norrath_constructor_lump_count_view, 1u);
    lumpLock__Fi(1);
    if (word_at(cache + 32) != 1u) return 4;
    std::puts("lump lock contract passed");
    return 0;
}
