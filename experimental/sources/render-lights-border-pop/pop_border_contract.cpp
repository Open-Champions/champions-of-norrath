// Host contract for norrath_archive_engine_pop_border. Not an EE plan input.
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <vector>

extern "C" void norrath_archive_engine_pop_border();

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
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
    const unsigned char* shstr = section(shstrndx);
    const unsigned str_off = read_u32(shstr + 16);
    auto name_of = [&](const unsigned char* sh) {
        return reinterpret_cast<const char*>(elf.data() + str_off + read_u32(sh));
    };
    const unsigned char* text = nullptr;
    const unsigned char* sym_sh = nullptr;
    unsigned rel_text = 0;
    for (unsigned i = 0; i < shnum; ++i) {
        const char* name = name_of(section(i));
        if (std::strcmp(name, ".text") == 0) text = section(i);
        if (std::strcmp(name, ".symtab") == 0) sym_sh = section(i);
        if (std::strcmp(name, ".rel.text") == 0) rel_text = read_u32(section(i) + 20);
    }
    expect(text && sym_sh, "text and symbols");
    expect(rel_text == 0u, "pop has no text relocation");
    if (failures) return;
    const unsigned text_off = read_u32(text + 16);
    const unsigned text_size = read_u32(text + 20);
    expect(text_size == 8u, "eight-byte return");
    expect(read_u32(elf.data() + text_off) == 0x03e00008u, "jr ra");
    expect(read_u32(elf.data() + text_off + 4) == 0u, "delay nop");
    const unsigned sym_off = read_u32(sym_sh + 16);
    const unsigned sym_size = read_u32(sym_sh + 20);
    const unsigned sym_entsize = read_u32(sym_sh + 36);
    const unsigned strtab_off = read_u32(section(read_u32(sym_sh + 24)) + 16);
    bool found = false;
    for (unsigned i = 0; i < sym_size / sym_entsize; ++i) {
        const unsigned char* sym = elf.data() + sym_off + i * sym_entsize;
        const char* name = reinterpret_cast<const char*>(elf.data() + strtab_off + read_u32(sym));
        if (std::strcmp(name, "norrath_archive_engine_pop_border") == 0) {
            found = true;
            expect(read_u32(sym + 8) == 8u, "symbol size");
            expect((sym[12] & 0xf) == 2, "function");
            expect((sym[12] >> 4) == 1, "global");
        }
    }
    expect(found, "pop symbol");
}

int main(int argc, char** argv) {
    unsigned canary = 0x22222222u;
    norrath_archive_engine_pop_border();
    expect(canary == 0x22222222u, "pop leaves the caller state alone");
    if (argc == 2) check_object(argv[1]);
    if (failures != 0) return 1;
    std::printf("border-pop contract passed\n");
    return 0;
}
