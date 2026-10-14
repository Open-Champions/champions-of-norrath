// Object check for LocalData. Not a build-plan input.
//   c++ -std=c++17 -o object_check object_check.cpp && ./object_check <unit.o compiled with authored_cpp flags>
// Requires a zero-initialised object (.bss) of 0x1d00 bytes, no data bytes and no relocations.
#include "../w12-startup-rt-a-check/elf_object.hpp"
int main(int argc, char** argv) {
    if (argc < 2) return 9;
    ElfObject o(argv[1]);
    if (!o.good()) return 8;
    ElfSymbol s = o.symbol("LocalData");
    if (!s.found || s.section != ".bss" || s.size != 0x1d00 || !s.bytes.empty() || !s.relocs.empty()) return 1;
    std::puts("LocalData object ok");
    return 0;
}
