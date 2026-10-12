/* Host comparison, not an EE plan input. Compares one compiled function body
 * with the bytes at an address of a local executable image. Instruction words
 * that carry a relocation in the compiled object are compared outside the
 * relocated field only (low 16 bits for HI16/LO16/PC16, low 26 bits for 26).
 * Each relocated word is listed with its symbol so the caller can confirm the
 * targets by hand. The comparison says nothing about what the targets are.
 *
 * usage: reloc_check <executable> <address> <object> <symbol>
 */
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>

typedef std::vector<unsigned char> Bytes;

static bool load(const char* path, Bytes& out) {
    std::ifstream in(path, std::ios::binary);
    if (!in) return false;
    out.assign(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
    return !out.empty();
}

static std::uint32_t half(const Bytes& e, std::size_t o) {
    return static_cast<std::uint32_t>(e[o] | (e[o + 1] << 8));
}
static std::uint32_t word(const Bytes& e, std::size_t o) {
    return static_cast<std::uint32_t>(e[o] | (e[o + 1] << 8) | (e[o + 2] << 16) |
                                      (static_cast<std::uint32_t>(e[o + 3]) << 24));
}

struct Reloc {
    std::uint32_t offset;
    unsigned type;
    std::string name;
};

static bool find_function(const Bytes& elf, const char* symbol, Bytes& body, std::vector<Reloc>& relocs) {
    if (elf.size() < 52 || elf[0] != 0x7f || elf[1] != 'E' || elf[4] != 1) return false;
    const std::uint32_t shoff = word(elf, 32), shentsize = half(elf, 46), shnum = half(elf, 48);
    if (shentsize < 40 || shoff + static_cast<std::uint64_t>(shnum) * shentsize > elf.size()) return false;
    auto field = [&](std::uint32_t s, int i) { return word(elf, shoff + s * shentsize + i * 4); };
    std::uint32_t symtab = 0, strtab = 0, textsec = 0, value = 0, span = 0;
    for (std::uint32_t s = 0; s < shnum; ++s) {
        if (field(s, 1) != 2) continue;
        symtab = s;
        strtab = field(s, 6);
        for (std::uint32_t c = 0; c + 16 <= field(s, 5); c += 16) {
            const std::uint32_t b = field(s, 4) + c;
            const char* n = reinterpret_cast<const char*>(elf.data() + field(strtab, 4) + word(elf, b));
            if ((elf[b + 12] & 15) == 2 && std::strcmp(n, symbol) == 0) {
                textsec = half(elf, b + 14);
                value = word(elf, b + 4);
                span = word(elf, b + 8);
            }
        }
    }
    if (!span) return false;
    body.assign(elf.begin() + field(textsec, 4) + value, elf.begin() + field(textsec, 4) + value + span);
    for (std::uint32_t s = 0; s < shnum; ++s) {
        if (field(s, 1) != 9 || field(s, 7) != textsec) continue;
        for (std::uint32_t c = 0; c + 8 <= field(s, 5); c += 8) {
            const std::uint32_t off = word(elf, field(s, 4) + c), info = word(elf, field(s, 4) + c + 4);
            if (off < value || off >= value + span) continue;
            const std::uint32_t sb = field(symtab, 4) + (info >> 8) * 16;
            const char* n = reinterpret_cast<const char*>(elf.data() + field(strtab, 4) + word(elf, sb));
            relocs.push_back({off - value, info & 0xff, n});
        }
    }
    return true;
}

static bool image_bytes(const Bytes& elf, std::uint32_t address, std::size_t size, Bytes& out) {
    const std::uint32_t phoff = word(elf, 28), phentsize = half(elf, 42), phnum = half(elf, 44);
    for (std::uint32_t i = 0; i < phnum; ++i) {
        const std::uint32_t b = phoff + i * phentsize;
        if (word(elf, b) != 1) continue;
        const std::uint32_t off = word(elf, b + 4), vaddr = word(elf, b + 8), filesz = word(elf, b + 16);
        if (address < vaddr || address - vaddr + size > filesz) continue;
        out.assign(elf.begin() + off + (address - vaddr), elf.begin() + off + (address - vaddr) + size);
        return true;
    }
    return false;
}

int main(int argc, char** argv) {
    if (argc != 5) {
        std::fputs("usage: reloc_check <executable> <address> <object> <symbol>\n", stderr);
        return 2;
    }
    const std::uint32_t address = static_cast<std::uint32_t>(std::strtoul(argv[2], nullptr, 0));
    Bytes exe, obj, mine, orig;
    std::vector<Reloc> relocs;
    if (!load(argv[1], exe) || !load(argv[3], obj)) return 3;
    if (!find_function(obj, argv[4], mine, relocs) || !image_bytes(exe, address, mine.size(), orig)) return 4;
    std::vector<std::uint32_t> mask(mine.size() / 4, 0xffffffffu);
    for (const Reloc& r : relocs) {
        if (r.offset % 4) return 5;
        std::uint32_t keep = 0;
        if (r.type == 4) keep = 0xfc000000u;                   /* R_MIPS_26 */
        else if (r.type == 5 || r.type == 6 || r.type == 10) keep = 0xffff0000u; /* HI16 LO16 PC16 */
        else return 6;
        mask[r.offset / 4] = keep;
        const std::uint32_t o = word(orig, r.offset);
        if (r.type == 4)
            std::printf("  +%u %s <- original target %#x\n", r.offset, r.name.c_str(), (o & 0x03ffffffu) << 2);
        else
            std::printf("  +%u %s type %u original field %#x\n", r.offset, r.name.c_str(), r.type, o & 0xffffu);
    }
    for (std::size_t i = 0; i < mask.size(); ++i) {
        if ((word(mine, i * 4) & mask[i]) != (word(orig, i * 4) & mask[i])) {
            std::fprintf(stderr, "word %zu differs: %08x vs %08x\n", i, word(mine, i * 4), word(orig, i * 4));
            return 1;
        }
    }
    std::printf("body match (relocated fields excluded): %s %zu bytes at %#x\n", argv[4], mine.size(), address);
    return 0;
}
