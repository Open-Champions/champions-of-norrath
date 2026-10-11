// Host contract for engineSetScreenColor__Fiiii. Not an EE plan input.
#include <cstdio>
#include <cstring>
#include <fstream>
#include <vector>

extern "C" int compositeR;
extern "C" int compositeG;
extern "C" int compositeB;
extern "C" void engineSetScreenColor__Fiiii(int red, int green, int blue, int alpha);

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
        const unsigned value = read_u32(sym + 4);
        const unsigned size = read_u32(sym + 8);
        const unsigned shndx = read_u16(sym + 14);
        if (std::strcmp(name, "engineSetScreenColor__Fiiii") == 0) {
            found_function = 1;
            expect((sym[12] & 0xf) == 2, "function");
            expect((sym[12] >> 4) == 1, "global function");
            expect(size > 0u, "function body");
        }
        if (std::strcmp(name, "compositeR") == 0 ||
            std::strcmp(name, "compositeG") == 0 ||
            std::strcmp(name, "compositeB") == 0) {
            ++found_words;
            expect(size == 4u, "color word size");
            expect((sym[12] & 0xf) == 1, "color object");
            expect((sym[12] >> 4) == 1, "global color");
            expect(shndx < shnum, "color section");
            if (shndx < shnum) {
                const unsigned char* sh = section(shndx);
                expect(read_u32(sh + 4) == 1u, "color word is file data");
                const unsigned off = read_u32(sh + 16);
                const unsigned char* bytes = elf.data() + off + value;
                expect(bytes[0] == 0x80 && bytes[1] == 0 && bytes[2] == 0 && bytes[3] == 0,
                       "initial channel 128");
            }
        }
    }
    expect(found_function == 1, "screen-color symbol");
    expect(found_words == 3, "three channel words");
}

int main(int argc, char** argv) {
    expect(compositeR == 128 && compositeG == 128 && compositeB == 128, "initial channels");
    engineSetScreenColor__Fiiii(-128, -128, -128, 0x55);
    expect(compositeR == 0 && compositeG == 0 && compositeB == 0, "startup black");
    engineSetScreenColor__Fiiii(0, 1, -1, 999);
    expect(compositeR == 128 && compositeG == 129 && compositeB == 127, "offset channels");
    engineSetScreenColor__Fiiii(2147483520, -129, 127, -1);
    expect(compositeR == static_cast<int>(2147483520u + 128u), "red wraps");
    expect(compositeG == -1 && compositeB == 255, "green and blue offsets");
    if (argc == 2) check_object(argv[1]);
    if (failures != 0) return 1;
    std::printf("screen-color contract passed\n");
    return 0;
}
