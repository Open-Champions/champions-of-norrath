// Host contract for textureFreeCacheBuffer__Fv. Not an EE plan input.
#include <cstdio>
#include <cstring>
#include <fstream>
#include <vector>

extern "C" char* uncompCacheAlloc;
extern "C" char* uncompCacheBuffer;
extern "C" char* uncompCache;
extern "C" void textureFreeCacheBuffer__Fv();

static int failures = 0;
static int frees = 0;
static char* freed = nullptr;
static char* alloc_during_free = nullptr;
static char* buffer_during_free = nullptr;
static char* cache_during_free = nullptr;

static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

extern "C" void memFree__FPv(char* block) {
    ++frees;
    freed = block;
    alloc_during_free = uncompCacheAlloc;
    buffer_during_free = uncompCacheBuffer;
    cache_during_free = uncompCache;
}

static unsigned read_u32(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) |
           (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static unsigned read_u16(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8);
}

static void check_object(const char* path) {
    std::ifstream in(path, std::ios::binary);
    std::vector<unsigned char> elf((std::istreambuf_iterator<char>(in)),
                                   std::istreambuf_iterator<char>());
    expect(elf.size() > 52 && elf[0] == 0x7f && elf[1] == 'E', "elf");
    if (failures) return;
    const unsigned shoff = read_u32(elf.data() + 32);
    const unsigned shentsize = read_u16(elf.data() + 46);
    const unsigned shnum = read_u16(elf.data() + 48);
    const unsigned shstrndx = read_u16(elf.data() + 50);
    auto section = [&](unsigned index) { return elf.data() + shoff + index * shentsize; };
    const unsigned str_off = read_u32(section(shstrndx) + 16);
    auto name_of = [&](const unsigned char* sh) {
        return reinterpret_cast<const char*>(elf.data() + str_off + read_u32(sh));
    };
    const unsigned char* sym_sh = nullptr;
    for (unsigned i = 0; i < shnum; ++i) {
        if (std::strcmp(name_of(section(i)), ".symtab") == 0) sym_sh = section(i);
    }
    expect(sym_sh != nullptr, "symbols");
    if (failures) return;
    const unsigned sym_off = read_u32(sym_sh + 16);
    const unsigned sym_size = read_u32(sym_sh + 20);
    const unsigned sym_entsize = read_u32(sym_sh + 36);
    const unsigned strtab_off = read_u32(section(read_u32(sym_sh + 24)) + 16);
    int found_function = 0;
    int found_words = 0;
    for (unsigned i = 0; i < sym_size / sym_entsize; ++i) {
        const unsigned char* sym = elf.data() + sym_off + i * sym_entsize;
        const char* name = reinterpret_cast<const char*>(elf.data() + strtab_off + read_u32(sym));
        const unsigned size = read_u32(sym + 8);
        const unsigned shndx = read_u16(sym + 14);
        if (std::strcmp(name, "textureFreeCacheBuffer__Fv") == 0) {
            found_function = 1;
            expect((sym[12] & 0xf) == 2, "function");
            expect((sym[12] >> 4) == 1, "global function");
            expect(size > 0u, "function body");
        }
        if (std::strcmp(name, "uncompCacheAlloc") == 0 ||
            std::strcmp(name, "uncompCacheBuffer") == 0 ||
            std::strcmp(name, "uncompCache") == 0) {
            ++found_words;
            expect(size == 4u, "cache pointer size");
            expect(shndx < shnum, "cache section");
            if (shndx < shnum) expect(read_u32(section(shndx) + 4) == 8u, "cache pointer is nobits");
        }
    }
    expect(found_function == 1, "free symbol");
    expect(found_words == 3, "three cache pointers");
}

int main(int argc, char** argv) {
    char kept = 0;
    char block = 0;
    uncompCache = &kept;
    textureFreeCacheBuffer__Fv();
    expect(frees == 0, "null allocation does not free");
    expect(uncompCache == &kept, "null free leaves the cache word");
    uncompCacheAlloc = &block;
    uncompCacheBuffer = &block + 1;
    textureFreeCacheBuffer__Fv();
    expect(frees == 1 && freed == &block, "frees the allocation pointer");
    expect(alloc_during_free == &block && buffer_during_free == &block + 1, "words still set during free");
    expect(cache_during_free == &kept, "cache word unchanged during free");
    expect(uncompCacheAlloc == nullptr && uncompCacheBuffer == nullptr, "clears both words after free");
    expect(uncompCache == &kept, "does not clear the cache word");
    textureFreeCacheBuffer__Fv();
    expect(frees == 1, "second call sees a null allocation");
    if (argc == 2) check_object(argv[1]);
    if (failures != 0) return 1;
    std::printf("texture-cache-free contract passed\n");
    return 0;
}
