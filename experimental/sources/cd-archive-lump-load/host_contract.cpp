#include <cstdio>
#include <cstdint>
#include <cstring>
extern "C" unsigned char norrath_constructor_lump_cache_view[128 * 40] = {};
extern "C" unsigned char norrath_constructor_lump_count_view[4] = {};
extern "C" unsigned noNewVags = 0;
extern "C" unsigned char lumpLoadLump[4] = {};
extern "C" unsigned char lumpLoadSize[4] = {};
extern "C" unsigned char lumpLoadCompletionFlag = 0;
extern "C" unsigned char lumpLoadName[192] = {};
extern "C" char engineCurrentLevelName[80] = {};
static int lookups = 0;
static int lookup_result = 0;
static int lookup_size = 0;
static int alloc_bytes = 0;
static char* alloc_label = nullptr;
static void* alloc_buffer = nullptr;
static int load_requested = -1;
static int load_offset = -1;
static unsigned vags_at_load = 99;
static int processes = 0;
static int prints = 0;
static const char* printed = nullptr;
static unsigned stored_address = 99;
static unsigned stored_value = 99;
static void put_word(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}
static unsigned word_at(const unsigned char* slot) {
    return slot[0] | (slot[1] << 8) | (slot[2] << 16) | (slot[3] << 24);
}
extern "C" void norrath_host_store_absolute(unsigned address, unsigned value) {
    stored_address = address;
    stored_value = value;
}
static void copy_text(char* destination, const char* source) {
    while ((*destination++ = *source++) != 0) {}
}
extern "C" int strcmp(const char* left, const char* right) {
    while (*left != 0 && *left == *right) { ++left; ++right; }
    return static_cast<int>(static_cast<unsigned char>(*left)) - static_cast<int>(static_cast<unsigned char>(*right));
}
extern "C" char* strcpy(char* destination, const char* source) {
    copy_text(destination, source);
    return destination;
}
extern "C" int cdDirectoryLookup__FPcPiT1i(char*, int* sector, int* bytes, int) {
    lookups += 1;
    *sector = 7;
    *bytes = lookup_size;
    return lookup_result;
}
extern "C" void* memAlloc__FiPc(int bytes, char* label) {
    alloc_bytes = bytes;
    alloc_label = label;
    return alloc_buffer;
}
extern "C" void cdLoad__FPcPvii(char*, void*, int requested, int byte_offset) {
    load_requested = requested;
    load_offset = byte_offset;
    vags_at_load = noNewVags;
}
extern "C" void cdProcess__Fv() {
    processes += 1;
    const unsigned count = word_at(norrath_constructor_lump_count_view);
    unsigned char* record = norrath_constructor_lump_cache_view + count * 40u;
    copy_text(reinterpret_cast<char*>(record), reinterpret_cast<char*>(lumpLoadName));
    put_word(record + 16, word_at(lumpLoadLump));
    put_word(record + 20, 0u);
    put_word(norrath_constructor_lump_count_view, count + 1u);
}
extern "C" int scePrintf(const char* format, ...) {
    prints += 1;
    printed = format;
    return 0;
}
extern "C" void* lumpLoad__FPc(char*);
int main() {
    unsigned char* cache = norrath_constructor_lump_cache_view;
    copy_text(reinterpret_cast<char*>(cache), "fx.lmp");
    put_word(cache + 16, 0x1000u);
    put_word(cache + 20, 0u);
    put_word(norrath_constructor_lump_count_view, 1u);
    if (lumpLoad__FPc(const_cast<char*>("fx.lmp")) != reinterpret_cast<void*>(static_cast<uintptr_t>(0x1000u))) return 1;
    if (lookups != 0) return 2;
    put_word(norrath_constructor_lump_count_view, 0u);
    lookup_result = 0;
    if (lumpLoad__FPc(const_cast<char*>("gone.lmp")) != nullptr) return 3;
    if (prints != 1 || strcmp(printed, "!!!!!!lump not found\n") != 0) return 4;
    if (stored_address != 0u || stored_value != 0u) return 5;
    lookup_result = 1;
    lookup_size = 100;
    alloc_buffer = reinterpret_cast<void*>(static_cast<uintptr_t>(0x2000u));
    copy_text(engineCurrentLevelName, "faydark2");
    noNewVags = 0u;
    void* loaded = lumpLoad__FPc(const_cast<char*>("vamplord.lmp"));
    if (loaded != alloc_buffer) return 6;
    if (vags_at_load != 1u || noNewVags != 0u) return 7;
    if (alloc_bytes != 2304 || alloc_label != reinterpret_cast<char*>(cache)) return 8;
    if (load_requested != 0 || load_offset != 0) return 9;
    if (word_at(lumpLoadSize) != 100u || lumpLoadCompletionFlag != 1) return 10;
    if (strcmp(reinterpret_cast<char*>(lumpLoadName), "vamplord.lmp") != 0) return 11;
    if (processes != 1) return 12;
    noNewVags = 0u;
    copy_text(engineCurrentLevelName, "gothic2");
    put_word(norrath_constructor_lump_count_view, 0u);
    lumpLoad__FPc(const_cast<char*>("vamplord.lmp"));
    if (vags_at_load != 0u) return 13;
    copy_text(engineCurrentLevelName, "ruinint1");
    put_word(norrath_constructor_lump_count_view, 0u);
    lumpLoad__FPc(const_cast<char*>("superorc.lmp"));
    if (vags_at_load != 0u) return 14;
    copy_text(engineCurrentLevelName, "other");
    put_word(norrath_constructor_lump_count_view, 0u);
    lumpLoad__FPc(const_cast<char*>("superorc.lmp"));
    if (vags_at_load != 1u) return 15;
    prints = 0;
    lookups = 0;
    lookup_result = 0;
    put_word(norrath_constructor_lump_count_view, 1u);
    copy_text(reinterpret_cast<char*>(cache), "fx.lmp");
    put_word(cache + 16, 0x1000u);
    put_word(cache + 20, 1u);
    if (lumpLoad__FPc(const_cast<char*>("fx.lmp")) != nullptr || lookups != 1) return 16;
    put_word(cache + 16, 0u);
    put_word(cache + 20, 0u);
    lookups = 0;
    if (lumpLoad__FPc(const_cast<char*>("fx.lmp")) != nullptr || lookups != 1) return 17;
    std::puts("lump load contract passed");
    return 0;
}
