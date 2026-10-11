/* Host comparison for one complete function body. Not an EE plan input. */
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

static bool name_at(const std::vector<unsigned char>& elf, std::uint32_t table, std::uint32_t size,
                    std::uint32_t offset, const char* expected) {
    if (offset >= size) return false;
    const unsigned char* text = elf.data() + table + offset;
    return std::strcmp(reinterpret_cast<const char*>(text), expected) == 0;
}

static bool object_body(const std::vector<unsigned char>& elf, const char* symbol,
                        std::vector<unsigned char>& body) {
    if (elf.size() < 52 || elf[0] != 0x7f || elf[1] != 'E' || elf[4] != 1) return false;
    const auto half = [&](std::size_t o) {
        return static_cast<std::uint32_t>(elf[o] | (elf[o + 1] << 8));
    };
    const auto word = [&](std::size_t o) {
        return static_cast<std::uint32_t>(elf[o] | (elf[o + 1] << 8) | (elf[o + 2] << 16) | (elf[o + 3] << 24));
    };
    const std::uint32_t shoff = word(32);
    const std::uint32_t shentsize = half(46);
    const std::uint32_t shnum = half(48);
    if (shentsize < 40 || shoff + static_cast<std::uint64_t>(shnum) * shentsize > elf.size()) return false;
    auto field = [&](std::uint32_t section, int index) {
        return word(shoff + section * shentsize + static_cast<std::uint32_t>(index) * 4);
    };
    for (std::uint32_t section = 0; section < shnum; ++section) {
        if (field(section, 1) != 2) continue;
        const std::uint32_t off = field(section, 4);
        const std::uint32_t size = field(section, 5);
        const std::uint32_t link = field(section, 6);
        const std::uint32_t entsize = field(section, 9);
        if (entsize != 16 || link >= shnum || off + size > elf.size()) return false;
        const std::uint32_t strings = field(link, 4);
        const std::uint32_t string_size = field(link, 5);
        if (strings + string_size > elf.size()) return false;
        for (std::uint32_t cursor = 0; cursor + 16 <= size; cursor += 16) {
            const std::uint32_t base = off + cursor;
            const std::uint32_t name = word(base);
            const std::uint32_t value = word(base + 4);
            const std::uint32_t span = word(base + 8);
            const unsigned info = elf[base + 12];
            const std::uint32_t shndx = half(base + 14);
            if ((info & 15) != 2 || shndx == 0 || shndx >= shnum) continue;
            if (!name_at(elf, strings, string_size, name, symbol)) continue;
            const std::uint32_t raw = field(shndx, 4);
            if (raw + static_cast<std::uint64_t>(value) + span > elf.size() || span == 0) return false;
            body.assign(elf.begin() + raw + value, elf.begin() + raw + value + span);
            return true;
        }
    }
    return false;
}

static bool image_body(const std::vector<unsigned char>& elf, std::uint32_t address, std::size_t size,
                       std::vector<unsigned char>& body) {
    if (elf.size() < 52) return false;
    const auto half = [&](std::size_t o) {
        return static_cast<std::uint32_t>(elf[o] | (elf[o + 1] << 8));
    };
    const auto word = [&](std::size_t o) {
        return static_cast<std::uint32_t>(elf[o] | (elf[o + 1] << 8) | (elf[o + 2] << 16) | (elf[o + 3] << 24));
    };
    const std::uint32_t phoff = word(28);
    const std::uint32_t phentsize = half(42);
    const std::uint32_t phnum = half(44);
    if (phentsize < 32 || phoff + static_cast<std::uint64_t>(phnum) * phentsize > elf.size()) return false;
    for (std::uint32_t i = 0; i < phnum; ++i) {
        const std::uint32_t base = phoff + i * phentsize;
        if (word(base) != 1) continue;
        const std::uint32_t offset = word(base + 4);
        const std::uint32_t vaddr = word(base + 8);
        const std::uint32_t filesz = word(base + 16);
        if (address < vaddr || address - vaddr >= filesz) continue;
        if (static_cast<std::uint64_t>(address - vaddr) + size > filesz) return false;
        const std::uint32_t file = offset + (address - vaddr);
        if (file + size > elf.size()) return false;
        body.assign(elf.begin() + file, elf.begin() + file + size);
        return true;
    }
    return false;
}

int main(int argc, char** argv) {
    if (argc != 5) {
        std::fputs("usage: host_check <executable> <address> <object> <symbol>\n", stderr);
        return 2;
    }
    const auto address = static_cast<std::uint32_t>(std::strtoul(argv[2], nullptr, 0));
    std::vector<unsigned char> executable, object, compiled, original;
    if (!load(argv[1], executable) || !load(argv[3], object)) return 3;
    if (!object_body(object, argv[4], compiled) || !image_body(executable, address, compiled.size(), original)) return 4;
    if (compiled != original) {
        std::fprintf(stderr, "body mismatch: %zu bytes at %#x\n", compiled.size(), address);
        return 1;
    }
    std::printf("body match: %s %zu bytes at %#x\n", argv[4], compiled.size(), address);
    return 0;
}
