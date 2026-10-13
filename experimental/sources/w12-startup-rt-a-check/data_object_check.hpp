// Generic data-object check for the w12-startup-rt-a units. Not a build-plan input. A unit's
// host_check.cpp calls data_object_check(object, symbol, section, size, hex bytes or "-" for NOBITS,
// {"symbol@offset", ...}) with its fixed expectation. Fails when the symbol is missing, in another section, of another size, holds other bytes, or the
// relocation list (symbol@offset inside the object, in order) differs.
#pragma once
#include "elf_object.hpp"
#include <cstdlib>
inline int data_object_check(const char* object, const char* symbol, const char* section, unsigned size,
                             const std::string& hex, const std::vector<std::string>& relocs = {}) {
    ElfObject o(object);
    if (!o.good()) return 8;
    ElfSymbol s = o.symbol(symbol);
    if (!s.found || s.section != section || s.size != size) { std::puts("shape"); return 1; }
    if (hex == "-") {
        if (!s.bytes.empty()) { std::puts("expected NOBITS"); return 2; }
    } else {
        if (hex.size() != s.bytes.size() * 2) { std::puts("length"); return 2; }
        for (std::size_t i = 0; i < s.bytes.size(); ++i)
            if (std::strtoul(hex.substr(2 * i, 2).c_str(), nullptr, 16) != s.bytes[i]) { std::printf("byte %zu\n", i); return 3; }
    }
    if (s.relocs.size() != relocs.size()) { std::puts("reloc count"); return 4; }
    for (std::size_t i = 0; i < s.relocs.size(); ++i)
        if (relocs[i] != s.relocs[i].symbol + "@" + std::to_string(s.relocs[i].offset)) { std::puts("reloc"); return 5; }
    std::printf("%s object ok\n", symbol);
    return 0;
}
