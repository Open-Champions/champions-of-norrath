// Object check for ModifierCodes. Not a build-plan input.
//   c++ -std=c++17 -o object_check object_check.cpp && ./object_check <unit.o compiled with authored_cpp flags>
// Requires the symbol in .data, size 8, bytes e0..e7, and no relocations.
#include "../w12-startup-rt-a-check/elf_object.hpp"
int main(int argc, char** argv) {
    if (argc < 2) return 9;
    ElfObject o(argv[1]);
    if (!o.good()) return 8;
    ElfSymbol s = o.symbol("ModifierCodes");
    if (!s.found || s.section != ".data" || s.size != 8 || s.bytes.size() != 8 || !s.relocs.empty()) return 1;
    for (int i = 0; i < 8; ++i)
        if (s.bytes[i] != 0xe0 + i) return 2;
    std::puts("ModifierCodes object ok");
    return 0;
}
