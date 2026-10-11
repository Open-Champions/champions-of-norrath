#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <sys/mman.h>
extern "C" unsigned char norrath_constructor_lump_cache_view[128 * 40] = {};
extern "C" unsigned char norrath_constructor_lump_count_view[4] = {};
static int loads = 0;
static uintptr_t pointer_high = 0;
static unsigned char* low_page() {
    static unsigned char* page = nullptr;
    if (page != nullptr) return page;
    page = static_cast<unsigned char*>(mmap(nullptr, 0x10000, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0));
    if (page == MAP_FAILED) std::_Exit(90);
    const uintptr_t start = reinterpret_cast<uintptr_t>(page);
    const uintptr_t end = start + 0x10000u;
    if ((start >> 32) != ((end - 1) >> 32)) std::_Exit(91);
    pointer_high = start & ~uintptr_t{0xffffffffu};
    return page;
}
static unsigned low_address(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<uintptr_t>(pointer));
}
extern "C" void* norrath_host_pointer(unsigned address) {
    low_page();
    return reinterpret_cast<void*>(pointer_high | address);
}
static void put_word(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}
extern "C" int strcmp(const char* left, const char* right) {
    while (*left != 0 && *left == *right) { ++left; ++right; }
    return static_cast<int>(static_cast<unsigned char>(*left)) - static_cast<int>(static_cast<unsigned char>(*right));
}
extern "C" void* lumpLoad__FPc(char* name) {
    loads += 1;
    if (strcmp(name, "font.lmp") != 0) return nullptr;
    unsigned char* lump = low_page() + 0x200;
    lump[0] = 3;
    lump[1] = 0;
    const char* names[] = {"a.fnt", "m.fnt", "z.fnt"};
    unsigned data[] = {0x10u, 0x20u, 0x30u};
    char* text = reinterpret_cast<char*>(low_page() + 0x300);
    for (int index = 0; index < 3; ++index) {
        char* cursor = text;
        const char* source = names[index];
        while ((*cursor++ = *source++) != 0) {}
        unsigned char* record = lump + 4 + index * 12;
        put_word(record, low_address(text));
        put_word(record + 4, data[index]);
        text = cursor;
    }
    return lump;
}
extern "C" char* lumpFindResource__FPcT0(char*, char*);
int main() {
    unsigned char* cache = norrath_constructor_lump_cache_view;
    char* resident_name = reinterpret_cast<char*>(low_page());
    const char* source = "gamefonta.fnt";
    char* cursor = resident_name;
    while ((*cursor++ = *source++) != 0) {}
    const char* lump_name = "font.lmp";
    cursor = reinterpret_cast<char*>(cache);
    while ((*cursor++ = *lump_name++) != 0) {}
    put_word(cache + 16, low_address(resident_name));
    put_word(cache + 20, 1u);
    put_word(norrath_constructor_lump_count_view, 1u);
    char* direct = lumpFindResource__FPcT0(const_cast<char*>("font.lmp"), const_cast<char*>("gamefonta.fnt"));
    if (direct != resident_name + 256 || loads != 0) return 1;
    put_word(cache + 20, 0u);
    unsigned char* lump = low_page() + 0x100;
    lump[0] = 3;
    lump[1] = 0;
    const char* names[] = {"a.fnt", "m.fnt", "z.fnt"};
    unsigned data[] = {0x10u, 0x20u, 0x30u};
    char* text = reinterpret_cast<char*>(low_page() + 0x180);
    for (int index = 0; index < 3; ++index) {
        cursor = text;
        source = names[index];
        while ((*cursor++ = *source++) != 0) {}
        unsigned char* record = lump + 4 + index * 12;
        put_word(record, low_address(text));
        put_word(record + 4, data[index]);
        text = cursor;
    }
    put_word(cache + 16, low_address(lump));
    char* found = lumpFindResource__FPcT0(const_cast<char*>("font.lmp"), const_cast<char*>("m.fnt"));
    if (found != static_cast<char*>(norrath_host_pointer(0x20u)) || loads != 0) return 2;
    if (lumpFindResource__FPcT0(const_cast<char*>("font.lmp"), const_cast<char*>("missing.fnt")) != nullptr) return 3;
    put_word(norrath_constructor_lump_count_view, 0u);
    char* loaded = lumpFindResource__FPcT0(const_cast<char*>("font.lmp"), const_cast<char*>("z.fnt"));
    if (loads != 1 || loaded != static_cast<char*>(norrath_host_pointer(0x30u))) return 4;
    if (lumpFindResource__FPcT0(const_cast<char*>("other.lmp"), const_cast<char*>("z.fnt")) != nullptr) return 5;
    std::puts("lump find resource contract passed");
    return 0;
}
