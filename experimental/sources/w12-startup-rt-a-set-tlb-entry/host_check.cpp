// Host check for _SetTLBEntry. Not a build-plan input.
//   c++ -std=c++17 -o check host_check.cpp && ./check <assembled-unit.o>
// The object is assembled by the caller with the reviewed authored_entry_asm flags (see
// docs/agents/w12-startup-rt-a.md). Expected words are the four instructions the pinned image holds at
// 0x002dfcc8: li v1,86 / syscall / jr ra / nop. A wrong service number, a missing delay-slot nop,
// a changed size or an extra relocation fails.
#include "../w12-startup-rt-a-check/elf_object.hpp"
int main(int argc, char** argv) {
    if (argc < 2) return 9;
    ElfObject o(argv[1]);
    if (!o.good()) return 8;
    ElfSymbol s = o.symbol("_SetTLBEntry");
    const std::uint32_t want[4] = {0x24030056, 0x0000000c, 0x03e00008, 0x00000000};
    if (!s.found || s.size != 16 || s.bytes.size() != 16 || !s.relocs.empty() || s.section != ".text") { std::puts("shape"); return 1; }
    for (int i = 0; i < 4; ++i)
        if (word_at(s.bytes, 4 * i) != want[i]) { std::printf("word %d\n", i); return 2; }
    std::puts("_SetTLBEntry ok");
    return 0;
}
