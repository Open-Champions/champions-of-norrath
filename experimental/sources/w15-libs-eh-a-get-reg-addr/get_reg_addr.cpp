// Observed get_reg_addr at 0x002fab30, 144 bytes (a local routine of the C++ exception runtime), pinned
// SLUS_205.65. Arguments: a register number, a frame-state record (layout in ../w15-libs-eh-a-common/eh.hpp)
// and a third value that the code overwrites without reading. While the record's saved byte for the register is 2
// (saved in another register), the register number is replaced by the low 32 bits of that register's
// doubleword slot. A saved byte of 1 returns the base word plus the low 32 bits of the register's slot, as an
// address word. Any other value calls abort (0x002ff8c0) and does not return.
// Inferred: the routine yields where a register was saved; the third argument is unused in the image.
#include "../w15-libs-eh-a-common/eh.hpp"

extern "C" {
void abort();

unsigned get_reg_addr(unsigned reg, w15eh::FrameState* state, w15eh::FrameState* unused) {
    (void)unused;
    while (state->saved[reg] == w15eh::kSavedInRegister) reg = static_cast<unsigned>(static_cast<int>(state->reg_or_offset[reg]));
    if (state->saved[reg] != w15eh::kSavedAtOffset) abort();
    return state->base + static_cast<unsigned>(static_cast<int>(state->reg_or_offset[reg]));
}
}
