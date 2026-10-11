// Host comparison for one pinned object published under two names.
// Both symbols must be the same global extent. A second body fails.
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>

namespace {
unsigned ru32(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
unsigned ru16(const unsigned char* p) { return unsigned(p[0]) | (unsigned(p[1]) << 8); }
bool fail(const char* message) {
    std::fprintf(stderr, "same extent: %s\n", message);
    return false;
}
struct Symbol {
    bool found = false;
    unsigned value = 0;
    unsigned size = 0;
    unsigned info = 0;
    unsigned section = 0;
};
Symbol find_symbol(const std::vector<unsigned char>& elf, const char* wanted) {
    Symbol result;
    const unsigned shoff = ru32(elf.data() + 32);
    const unsigned shentsize = ru16(elf.data() + 46);
    const unsigned shnum = ru16(elf.data() + 48);
    auto section = [&](unsigned index) { return elf.data() + shoff + index * shentsize; };
    const unsigned char* symtab = nullptr;
    for (unsigned i = 0; i < shnum; ++i) {
        if (ru32(section(i) + 4) == 2) symtab = section(i);
    }
    if (symtab == nullptr) return result;
    const unsigned char* symbols = elf.data() + ru32(symtab + 16);
    const unsigned count = ru32(symtab + 20) / 16;
    const unsigned char* strings = elf.data() + ru32(section(ru32(symtab + 24)) + 16);
    for (unsigned i = 0; i < count; ++i) {
        const unsigned char* symbol = symbols + i * 16;
        const char* name = reinterpret_cast<const char*>(strings + ru32(symbol));
        if (std::strcmp(name, wanted) != 0) continue;
        result.found = true;
        result.value = ru32(symbol + 4);
        result.size = ru32(symbol + 8);
        result.info = symbol[12];
        result.section = ru16(symbol + 14);
    }
    return result;
}
}

int main(int argc, char** argv) {
    if (argc != 4) {
        std::fprintf(stderr, "usage: check_same_extent <object> <name> <name>\n");
        return 2;
    }
    std::ifstream input(argv[1], std::ios::binary);
    std::vector<unsigned char> elf((std::istreambuf_iterator<char>(input)), {});
    if (elf.size() < 52 || std::memcmp(elf.data(), "\x7f""ELF\x01\x01\x01", 7) != 0)
        return fail("not a little-endian ELF32 object");
    const Symbol first = find_symbol(elf, argv[2]);
    const Symbol second = find_symbol(elf, argv[3]);
    if (!first.found || !second.found) return fail("missing symbol");
    if (first.section == 0 || second.section == 0) return fail("symbol is undefined");
    if (first.section != second.section || first.value != second.value || first.size != second.size)
        return fail("names do not share one extent");
    if (first.size == 0 || first.info != second.info) return fail("extent is empty or typed differently");
    if ((first.info >> 4) != 1) return fail("symbol is not global");
    std::printf("same extent %s %s size %u\n", argv[2], argv[3], first.size);
    return 0;
}
