// Host check of the EE object for haloTexture7. Not an EE plan input.
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
        std::fprintf(stderr, "usage: halo_texture_7_contract <ee-object>\n");
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
    const unsigned char* sym_sh = nullptr;
    for (unsigned i = 0; i < shnum; ++i) {
        if (std::strcmp(name_of(section(i)), ".symtab") == 0) sym_sh = section(i);
    }
    expect(sym_sh != nullptr, "symbols");
    if (failures) return 1;
    const unsigned sym_off = read_u32(sym_sh + 16);
    const unsigned sym_size = read_u32(sym_sh + 20);
    const unsigned sym_entsize = read_u32(sym_sh + 36);
    const unsigned strtab_off = read_u32(section(read_u32(sym_sh + 24)) + 16);
    bool found = false;
    for (unsigned i = 0; i < sym_size / sym_entsize; ++i) {
        const unsigned char* sym = elf.data() + sym_off + i * sym_entsize;
        const char* name = reinterpret_cast<const char*>(elf.data() + strtab_off + read_u32(sym));
        if (std::strcmp(name, "haloTexture7") == 0) {
            found = true;
            const unsigned shndx = read_u16(sym + 14);
            expect(read_u32(sym + 8) == 4u, "four-byte pointer");
            expect((sym[12] & 0xf) == 1, "object");
            expect((sym[12] >> 4) == 1, "global");
            expect(shndx < shnum, "section");
            if (shndx < shnum) expect(read_u32(section(shndx) + 4) == 8u, "nobits zero");
        }
    }
    expect(found, "haloTexture7 symbol");
    if (failures) return 1;
    std::printf("haloTexture7 contract passed\n");
    return 0;
}
