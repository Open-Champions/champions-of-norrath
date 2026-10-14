// Observed copy_reg at 0x002fabc0, 100 bytes (a local routine of the C++ exception runtime), pinned
// SLUS_205.65. Arguments: a register number and two frame-state records. It asks get_reg_addr (0x002fab30,
// third argument zero) for where the register lives in the first record, then in the second, and calls
// memcpy (0x00301ffc) with destination = the second record's location, source = the first record's, and a length
// of the signed byte dwarf_reg_size_table[reg] (0x008eaa98). The first lookup is made before the second.
// Inferred: the routine copies one saved register between two unwinder states.
#include "../w15-libs-eh-a-common/eh.hpp"

extern "C" {
extern signed char dwarf_reg_size_table[];
unsigned get_reg_addr(unsigned reg, w15eh::FrameState* state, w15eh::FrameState* unused);
void* memcpy(void* destination, const void* source, unsigned long count);

void copy_reg(unsigned reg, w15eh::FrameState* from, w15eh::FrameState* to) {
    const unsigned source = get_reg_addr(reg, from, nullptr);
    const unsigned destination = get_reg_addr(reg, to, nullptr);
    memcpy(w15eh::from_word(destination), w15eh::from_word(source), static_cast<unsigned long>(dwarf_reg_size_table[reg]));
}
}
