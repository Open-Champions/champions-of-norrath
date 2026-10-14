// Observed next_stack_level at 0x002fac28, 172 bytes (a local routine of the C++ exception runtime), pinned
// SLUS_205.65. Arguments: a code address, the frame-state record of the level just unwound and a record for
// the next level. It calls __frame_state_for (0x002fd0f0) with the code address and the third argument; a null
// result is returned as null. With the result s and r = s's base-register halfword (offset 1016):
//   * if the first record's saved byte for r is nonzero, s's base word becomes the low 32 bits of the doubleword
//     found at get_reg_addr(r, first record, 0) (0x002fab30); otherwise it becomes the first record's base word;
//   * if s's indirect byte (offset 1143) is nonzero, the base word is replaced by the word stored at
//     base + the low 32 bits of s's doubleword at offset 16;
//   * the low 32 bits of s's doubleword at offset 8 are added to the base word, and s is returned.
// Record layout: ../w15-libs-eh-a-common/eh.hpp. Inferred: the routine computes the stack-pointer value the
// caller's frame had; the field roles come from the use, not from any name in the image.
#include "../w15-libs-eh-a-common/eh.hpp"

extern "C" {
w15eh::FrameState* __frame_state_for(unsigned pc, w15eh::FrameState* result);
unsigned get_reg_addr(unsigned reg, w15eh::FrameState* state, w15eh::FrameState* unused);

w15eh::FrameState* next_stack_level(unsigned pc, w15eh::FrameState* current, w15eh::FrameState* next) {
    w15eh::FrameState* const level = __frame_state_for(pc, next);
    if (level == nullptr) return nullptr;
    const unsigned reg = level->base_reg;
    if (current->saved[reg] != 0) {
        const unsigned address = get_reg_addr(reg, current, nullptr);
        level->base = static_cast<unsigned>(*reinterpret_cast<long long*>(w15eh::from_word(address)));
    } else {
        level->base = current->base;
    }
    if (level->indirect != 0) {
        const unsigned address = level->base + static_cast<unsigned>(level->indirect_offset);
        level->base = *reinterpret_cast<unsigned*>(w15eh::from_word(address));
    }
    level->base = level->base + static_cast<unsigned>(level->offset_from_base);
    return level;
}
}
