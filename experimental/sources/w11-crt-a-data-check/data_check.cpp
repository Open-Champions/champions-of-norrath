/* Host comparison for one complete data object. Not an EE plan input.
 * usage: data_check <executable> <address> <object> <symbol> [skip-offset ...]
 * Looks the symbol up in the compiled relocatable object (any symbol type, defined in a
 * PROGBITS or NOBITS section), then compares its bytes with the bytes at <address> of the
 * executable's loadable segments. NOBITS objects are taken as zero; an executable address
 * beyond the file bytes of its segment but inside its memory size is zero as well.
 * Each skip offset is a byte offset inside the object whose four-byte word is not compared
 * (a word that carries a relocation, which is zero in the unlinked object). Reading is
 * done without any dependency on the executable's section names.
 */
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>

static bool load(const char* path, std::vector<unsigned char>& out) {
    std::ifstream in(path, std::ios::binary);
    if (!in) return false;
    out.assign(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
    return !out.empty();
}

struct Elf {
    const std::vector<unsigned char>& d;
    std::uint32_t half(std::size_t o) const { return static_cast<std::uint32_t>(d[o] | (d[o + 1] << 8)); }
    std::uint32_t word(std::size_t o) const {
        return static_cast<std::uint32_t>(d[o] | (d[o + 1] << 8) | (d[o + 2] << 16) | (d[o + 3] << 24));
    }
};

static bool object_data(const std::vector<unsigned char>& elf, const char* symbol, std::vector<unsigned char>& body) {
    if (elf.size() < 52 || elf[0] != 0x7f || elf[1] != 'E' || elf[4] != 1) return false;
    const Elf e{elf};
    const std::uint32_t shoff = e.word(32), shentsize = e.half(46), shnum = e.half(48);
    if (shentsize < 40 || shoff + static_cast<std::uint64_t>(shnum) * shentsize > elf.size()) return false;
    auto field = [&](std::uint32_t section, int index) { return e.word(shoff + section * shentsize + static_cast<std::uint32_t>(index) * 4); };
    for (std::uint32_t section = 0; section < shnum; ++section) {
        if (field(section, 1) != 2) continue;
        const std::uint32_t off = field(section, 4), size = field(section, 5), link = field(section, 6), entsize = field(section, 9);
        if (entsize != 16 || link >= shnum || off + size > elf.size()) return false;
        const std::uint32_t strings = field(link, 4), string_size = field(link, 5);
        if (strings + string_size > elf.size()) return false;
        for (std::uint32_t cursor = 0; cursor + 16 <= size; cursor += 16) {
            const std::uint32_t base = off + cursor, name = e.word(base), value = e.word(base + 4), span = e.word(base + 8);
            const std::uint32_t shndx = e.half(base + 14);
            if (shndx == 0 || shndx >= shnum || name >= string_size) continue;
            if (std::strcmp(reinterpret_cast<const char*>(elf.data() + strings + name), symbol) != 0) continue;
            if (span == 0) return false;
            const std::uint32_t type = field(shndx, 1), raw = field(shndx, 4);
            if (type == 8) { body.assign(span, 0); return true; }
            if (raw + static_cast<std::uint64_t>(value) + span > elf.size()) return false;
            body.assign(elf.begin() + raw + value, elf.begin() + raw + value + span);
            return true;
        }
    }
    return false;
}

static bool image_data(const std::vector<unsigned char>& elf, std::uint32_t address, std::size_t size, std::vector<unsigned char>& body) {
    if (elf.size() < 52) return false;
    const Elf e{elf};
    const std::uint32_t phoff = e.word(28), phentsize = e.half(42), phnum = e.half(44);
    if (phentsize < 32 || phoff + static_cast<std::uint64_t>(phnum) * phentsize > elf.size()) return false;
    for (std::uint32_t i = 0; i < phnum; ++i) {
        const std::uint32_t base = phoff + i * phentsize;
        if (e.word(base) != 1) continue;
        const std::uint32_t offset = e.word(base + 4), vaddr = e.word(base + 8), filesz = e.word(base + 16), memsz = e.word(base + 20);
        if (address < vaddr || address - vaddr >= memsz) continue;
        if (static_cast<std::uint64_t>(address - vaddr) + size > memsz) return false;
        body.assign(size, 0);
        for (std::size_t k = 0; k < size; ++k) {
            const std::uint32_t rel = address - vaddr + static_cast<std::uint32_t>(k);
            if (rel < filesz) {
                if (offset + rel >= elf.size()) return false;
                body[k] = elf[offset + rel];
            }
        }
        return true;
    }
    return false;
}

int main(int argc, char** argv) {
    if (argc < 5) {
        std::fputs("usage: data_check <executable> <address> <object> <symbol> [skip-offset ...]\n", stderr);
        return 2;
    }
    const auto address = static_cast<std::uint32_t>(std::strtoul(argv[2], nullptr, 0));
    std::vector<unsigned char> executable, object, compiled, original;
    if (!load(argv[1], executable) || !load(argv[3], object)) return 3;
    if (!object_data(object, argv[4], compiled) || !image_data(executable, address, compiled.size(), original)) return 4;
    for (int i = 5; i < argc; ++i) {
        const std::size_t skip = std::strtoul(argv[i], nullptr, 0);
        for (std::size_t k = 0; k < 4 && skip + k < compiled.size(); ++k) compiled[skip + k] = original[skip + k];
    }
    if (compiled != original) {
        std::fprintf(stderr, "data mismatch: %zu bytes at %#x\n", compiled.size(), address);
        return 1;
    }
    std::printf("data match: %s %zu bytes at %#x\n", argv[4], compiled.size(), address);
    return 0;
}
