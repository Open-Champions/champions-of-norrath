// Host check of the EE object for norrath_factory_clear_color_word.
// Not an EE plan input. Pass the reviewed-toolchain object as argv[1].
#include <cstdio>
#include <cstring>
#include <fstream>
#include <vector>

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

int main(int argc, char** argv) {
    if (argc != 2) {
        std::fprintf(stderr, "usage: clear_color_contract <ee-object>\n");
        return 2;
    }
    std::ifstream in(argv[1], std::ios::binary);
    std::vector<unsigned char> elf((std::istreambuf_iterator<char>(in)),
                                   std::istreambuf_iterator<char>());
    expect(elf.size() > 52 && elf[0] == 0x7f && elf[1] == 'E', "elf");
    if (failures) return 1;
    const unsigned shoff = read_u32(elf.data() + 32);
    const unsigned shentsize = read_u16(elf.data() + 46);
    const unsigned shnum = read_u16(elf.data() + 48);
    const unsigned shstrndx = read_u16(elf.data() + 50);
    auto section = [&](unsigned index) { return elf.data() + shoff + index * shentsize; };
    const unsigned str_off = read_u32(section(shstrndx) + 16);
    auto name_of = [&](const unsigned char* sh) {
        return reinterpret_cast<const char*>(elf.data() + str_off + read_u32(sh));
    };
    const unsigned char* data_sh = nullptr;
    const unsigned char* sym_sh = nullptr;
    unsigned rel_data = 0;
    for (unsigned i = 0; i < shnum; ++i) {
        const char* name = name_of(section(i));
        if (std::strcmp(name, ".data") == 0) data_sh = section(i);
        if (std::strcmp(name, ".symtab") == 0) sym_sh = section(i);
        if (std::strcmp(name, ".rel.data") == 0) rel_data = read_u32(section(i) + 20);
    }
    expect(data_sh && sym_sh, "data and symbols");
    expect(rel_data == 0u, "clear color has no relocation");
    if (failures) return 1;
    const unsigned data_off = read_u32(data_sh + 16);
    const unsigned data_size = read_u32(data_sh + 20);
    expect(data_size == 4u, "four data bytes");
    expect(elf[data_off] == 0x00 && elf[data_off + 1] == 0x00 &&
           elf[data_off + 2] == 0x00 && elf[data_off + 3] == 0x80, "initial word");
    const unsigned sym_off = read_u32(sym_sh + 16);
    const unsigned sym_size = read_u32(sym_sh + 20);
    const unsigned sym_entsize = read_u32(sym_sh + 36);
    const unsigned strtab_off = read_u32(section(read_u32(sym_sh + 24)) + 16);
    bool found = false;
    for (unsigned i = 0; i < sym_size / sym_entsize; ++i) {
        const unsigned char* sym = elf.data() + sym_off + i * sym_entsize;
        const char* name = reinterpret_cast<const char*>(elf.data() + strtab_off + read_u32(sym));
        if (std::strcmp(name, "norrath_factory_clear_color_word") == 0) {
            found = true;
            expect(read_u32(sym + 4) == 0u, "symbol offset");
            expect(read_u32(sym + 8) == 4u, "symbol size");
            expect((sym[12] & 0xf) == 1, "object");
            expect((sym[12] >> 4) == 1, "global");
        }
    }
    expect(found, "clear-color symbol");
    if (failures) return 1;
    std::printf("clear-color contract passed\n");
    return 0;
}
