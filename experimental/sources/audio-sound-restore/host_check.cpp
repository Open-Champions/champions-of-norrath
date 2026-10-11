/* Host check that the original name is the existing restore body.
 * Not an EE plan input.
 */
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <vector>

static bool load(const char* path, std::vector<unsigned char>& out) {
    std::ifstream in(path, std::ios::binary);
    if (!in) return false;
    out.assign(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
    return !out.empty();
}

struct Symbol {
    std::uint32_t value;
    std::uint32_t size;
    std::uint32_t section;
    bool found;
};

static Symbol find_symbol(const std::vector<unsigned char>& elf, const char* expected) {
    Symbol missing{0, 0, 0, false};
    if (elf.size() < 52 || elf[0] != 0x7f || elf[1] != 'E' || elf[4] != 1) return missing;
    const auto half = [&](std::size_t offset) {
        return static_cast<std::uint32_t>(elf[offset] | (elf[offset + 1] << 8));
    };
    const auto word = [&](std::size_t offset) {
        return static_cast<std::uint32_t>(elf[offset] | (elf[offset + 1] << 8) |
                                           (elf[offset + 2] << 16) | (elf[offset + 3] << 24));
    };
    const std::uint32_t shoff = word(32);
    const std::uint32_t shentsize = half(46);
    const std::uint32_t shnum = half(48);
    if (shentsize < 40 || shoff + static_cast<std::uint64_t>(shnum) * shentsize > elf.size()) return missing;
    auto field = [&](std::uint32_t section, int index) {
        return word(shoff + section * shentsize + static_cast<std::uint32_t>(index) * 4);
    };
    for (std::uint32_t section = 0; section < shnum; ++section) {
        if (field(section, 1) != 2) continue;
        const std::uint32_t off = field(section, 4);
        const std::uint32_t size = field(section, 5);
        const std::uint32_t link = field(section, 6);
        const std::uint32_t entsize = field(section, 9);
        if (entsize != 16 || link >= shnum || off + size > elf.size()) return missing;
        const std::uint32_t strings = field(link, 4);
        const std::uint32_t string_size = field(link, 5);
        if (strings + string_size > elf.size()) return missing;
        for (std::uint32_t cursor = 0; cursor + 16 <= size; cursor += 16) {
            const std::uint32_t base = off + cursor;
            const std::uint32_t name = word(base);
            if (name >= string_size) continue;
            if (std::strcmp(reinterpret_cast<const char*>(elf.data() + strings + name), expected) != 0) continue;
            const unsigned info = elf[base + 12];
            if ((info & 15) != 2) continue;
            return Symbol{word(base + 4), word(base + 8), half(base + 14), true};
        }
    }
    return missing;
}

int main(int argc, char** argv) {
    if (argc != 2) return 2;
    std::vector<unsigned char> elf;
    if (!load(argv[1], elf)) return 3;
    const Symbol published = find_symbol(elf, "norrath_sound_restore");
    const Symbol original = find_symbol(elf, "soundRestore__Fb");
    if (!published.found || !original.found) return 4;
    if (published.size == 0 || published.section == 0) return 5;
    if (published.value != original.value || published.size != original.size ||
        published.section != original.section) return 6;
    std::puts("sound restore alias passed");
    return 0;
}
