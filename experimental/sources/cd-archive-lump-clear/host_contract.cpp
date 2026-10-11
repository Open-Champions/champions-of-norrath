#include <cstdio>
#include <cstdint>
extern "C" unsigned char discardable[32] = {};
extern "C" unsigned char norrath_constructor_lump_cache_view[128 * 40] = {};
extern "C" unsigned char norrath_constructor_lump_count_view[4] = {};
extern "C" unsigned char norrath_constructor_resource_frame_view[4] = {1, 0, 0, 0};
static int clears = 0;
static int cleared_value = 0;
static int cleared_while_dirty = 0;
static int unloads = 0;
static unsigned unloaded = 0;
static void put_word(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}
static unsigned word_at(const unsigned char* slot) {
    return slot[0] | (slot[1] << 8) | (slot[2] << 16) | (slot[3] << 24);
}
extern "C" void soundClear__Fi(int value) {
    clears += 1;
    cleared_value = value;
    if (discardable[0] != 0) cleared_while_dirty = 1;
}
extern "C" void lumpUnload__FPv(void* lump) {
    unloads += 1;
    unloaded = static_cast<unsigned>(reinterpret_cast<uintptr_t>(lump));
    const unsigned count = word_at(norrath_constructor_lump_count_view);
    if (count != 0u) put_word(norrath_constructor_lump_count_view, count - 1u);
}
extern "C" void lumpClear__Fi(int value);
int main() {
    discardable[0] = 7;
    unsigned char* cache = norrath_constructor_lump_cache_view;
    put_word(cache + 16, 0x111u);
    put_word(cache + 32, 10u);
    put_word(cache + 40 + 16, 0x222u);
    put_word(cache + 40 + 32, 40u);
    put_word(norrath_constructor_lump_count_view, 2u);
    lumpClear__Fi(50);
    if (cleared_while_dirty != 0 || clears != 1 || cleared_value != 50) return 1;
    if (unloads != 2 || unloaded != 0x111u) return 2;
    if (word_at(norrath_constructor_lump_count_view) != 0u) return 3;
    if (word_at(norrath_constructor_resource_frame_view) != 2u) return 4;
    put_word(norrath_constructor_lump_count_view, 1u);
    put_word(cache + 16, 0x333u);
    put_word(cache + 32, 80u);
    unloads = 0;
    lumpClear__Fi(20);
    if (unloads != 0 || word_at(norrath_constructor_resource_frame_view) != 3u) return 5;
    put_word(norrath_constructor_lump_count_view, 0u);
    lumpClear__Fi(5);
    if (unloads != 0 || clears != 3 || word_at(norrath_constructor_resource_frame_view) != 4u) return 6;
    std::puts("lump clear contract passed");
    return 0;
}
