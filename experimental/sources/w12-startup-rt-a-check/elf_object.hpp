// Host-only helper for the w12-startup-rt-a checks. Not a build-plan input.
// Reads one symbol out of a little-endian ELF32 relocatable object written by the reviewed EE
// compiler: its bytes, size, section name and the REL relocations that land inside it.
#pragma once
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

struct ElfReloc {
    std::uint32_t offset;   // offset inside the symbol
    std::uint32_t type;
    std::string symbol;
};

struct ElfSymbol {
    bool found = false;
    std::uint32_t size = 0;
    std::string section;
    std::vector<unsigned char> bytes;   // empty for NOBITS
    std::vector<ElfReloc> relocs;
    std::vector<std::string> undefined; // every undefined symbol of the object
};

class ElfObject {
public:
    explicit ElfObject(const char* path) {
        std::ifstream in(path, std::ios::binary);
        d.assign(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
        ok = d.size() > 52 && d[0] == 0x7f && d[1] == 'E' && d[4] == 1 && d[5] == 1;
        if (ok) { shoff = w(32); shent = h(46); shnum = h(48); shstr = h(50); }
    }
    bool good() const { return ok; }

    ElfSymbol symbol(const char* name) const {
        ElfSymbol r;
        for (unsigned s = 0; s < shnum; ++s) {
            if (f(s, 1) == 2) {
                const unsigned strsec = f(s, 6);
                for (std::uint32_t c = 0; c + 16 <= f(s, 5); c += 16) {
                    const std::uint32_t b = f(s, 4) + c;
                    const unsigned shndx = h(b + 14);
                    const char* n = str(strsec, w(b));
                    if (shndx == 0) { r.undefined.push_back(n); continue; }
                    if (r.found || std::strcmp(n, name) != 0 || shndx >= shnum) continue;
                    r.found = true;
                    r.size = w(b + 8);
                    r.section = str(shstr, f(shndx, 0));
                    const std::uint32_t value = w(b + 4);
                    if (f(shndx, 1) != 8) r.bytes.assign(d.begin() + f(shndx, 4) + value, d.begin() + f(shndx, 4) + value + r.size);
                    collect(shndx, value, r);
                }
            }
        }
        return r;
    }

private:
    std::vector<unsigned char> d;
    bool ok = false;
    std::uint32_t shoff = 0, shent = 0, shnum = 0, shstr = 0;
    std::uint32_t w(std::size_t o) const { return d[o] | (d[o + 1] << 8) | (d[o + 2] << 16) | (std::uint32_t(d[o + 3]) << 24); }
    std::uint32_t h(std::size_t o) const { return d[o] | (d[o + 1] << 8); }
    std::uint32_t f(unsigned sec, int idx) const { return w(shoff + sec * shent + idx * 4); }
    const char* str(unsigned strsec, std::uint32_t off) const { return reinterpret_cast<const char*>(d.data() + f(strsec, 4) + off); }
    void collect(unsigned target, std::uint32_t value, ElfSymbol& r) const {
        for (unsigned s = 0; s < shnum; ++s) {
            if (f(s, 1) != 9 || f(s, 7) != target) continue;
            const unsigned symtab = f(s, 6);
            for (std::uint32_t c = 0; c + 8 <= f(s, 5); c += 8) {
                const std::uint32_t off = w(f(s, 4) + c), info = w(f(s, 4) + c + 4);
                if (off < value || off >= value + r.size) continue;
                const std::uint32_t sb = f(symtab, 4) + (info >> 8) * 16;
                const unsigned secidx = h(sb + 14);
                std::string nm = str(f(symtab, 6), w(sb));
                if (nm.empty() && secidx < shnum) nm = std::string("[") + str(shstr, f(secidx, 0)) + "]";
                r.relocs.push_back({off - value, info & 255, nm});
            }
        }
    }
};

inline std::uint32_t word_at(const std::vector<unsigned char>& b, std::size_t i) {
    return b[i] | (b[i + 1] << 8) | (b[i + 2] << 16) | (std::uint32_t(b[i + 3]) << 24);
}
