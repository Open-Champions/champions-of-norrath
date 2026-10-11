// Host check for the EE relocatable bin image. It fails if the 1032-byte
// object, its zero head words, or the 256 self-relative MIPS addresses change.
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
    std::fprintf(stderr, "bins object: %s\n", message);
    return false;
}
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::fprintf(stderr, "usage: check_bins_object <malloc_bins.o>\n");
        return 2;
    }
    std::ifstream input(argv[1], std::ios::binary);
    std::vector<unsigned char> elf((std::istreambuf_iterator<char>(input)), {});
    if (elf.size() < 52 || std::memcmp(elf.data(), "\x7f""ELF\x01\x01\x01", 7) != 0)
        return fail("not a little-endian ELF32 object");
    const unsigned shoff = ru32(elf.data() + 32);
    const unsigned shentsize = ru16(elf.data() + 46);
    const unsigned shnum = ru16(elf.data() + 48);
    const unsigned shstrndx = ru16(elf.data() + 50);
    if (shentsize < 40 || shoff + shnum * shentsize > elf.size()) return fail("bad section table");
    auto section = [&](unsigned index) { return elf.data() + shoff + index * shentsize; };
    const unsigned char* shstr = section(shstrndx);
    auto bytes = [&](const unsigned char* s) {
        return elf.data() + ru32(s + 16);
    };
    auto size = [&](const unsigned char* s) { return ru32(s + 20); };
    auto name_of = [&](const unsigned char* s) {
        return reinterpret_cast<const char*>(bytes(shstr) + ru32(s));
    };
    const unsigned char* symtab = nullptr;
    const unsigned char* data_sec = nullptr;
    const unsigned char* rel = nullptr;
    for (unsigned i = 0; i < shnum; ++i) {
        const unsigned char* s = section(i);
        const char* name = name_of(s);
        if (ru32(s + 4) == 2) symtab = s;
        if (std::strcmp(name, ".data") == 0) data_sec = s;
        if (std::strcmp(name, ".rel.data") == 0) rel = s;
    }
    if (symtab == nullptr || data_sec == nullptr || rel == nullptr) return fail("missing data or symbols");
    if (size(data_sec) != 1032) return fail("data section is not 1032 bytes");
    const unsigned char* symbols = bytes(symtab);
    const unsigned symbol_count = size(symtab) / 16;
    const unsigned char* strtab = bytes(section(ru32(symtab + 24)));
    int symbol_index = -1;
    unsigned functions = 0;
    for (unsigned i = 0; i < symbol_count; ++i) {
        const unsigned char* s = symbols + i * 16;
        const char* name = reinterpret_cast<const char*>(strtab + ru32(s));
        const unsigned info = s[12];
        if ((info & 15) == 2 && s[14] != 0) ++functions;
        if (std::strcmp(name, "norrath_player_malloc_bins_view") == 0) {
            if (ru32(s + 4) != 0 || ru32(s + 8) != 1032) return fail("bin symbol size or value");
            symbol_index = static_cast<int>(i);
        }
    }
    if (symbol_index < 0) return fail("bin symbol missing");
    if (functions != 0) return fail("bin image emitted a function");
    const unsigned char* image = bytes(data_sec);
    if (ru32(image) != 0 || ru32(image + 4) != 0) return fail("leading words are not zero");
    if (size(rel) != 256 * 8) return fail("expected 256 relocations");
    const unsigned char* entries = bytes(rel);
    for (unsigned i = 0; i < 256; ++i) {
        const unsigned offset = ru32(entries + i * 8);
        const unsigned info = ru32(entries + i * 8 + 4);
        const unsigned type = info & 0xff;
        const unsigned sym = info >> 8;
        const unsigned addend = ru32(image + offset);
        if (offset != 8 + i * 4 || type != 2 || sym != static_cast<unsigned>(symbol_index))
            return fail("relocation target");
        if (addend != (i / 2) * 8) return fail("circular bin addend");
    }
    std::printf("malloc bin object checks passed\n");
    return 0;
}
