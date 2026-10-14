// Host check only; not an EE build input. Build the source with -DW15_EH_HOST -Dnext_stack_level=w15_next_stack_level
// -D__frame_state_for=w15_t_fsf -Dget_reg_addr=w15_t_get_reg_addr; this file is compiled with -DW15_EH_HOST only.
// Address words are offsets into one arena.
#include <cstdio>
#include <cstring>
#include "../w15-libs-eh-a-common/eh.hpp"
using w15eh::FrameState;
static unsigned char arena[0x1000];
unsigned char* w15_eh_host_base = arena;
static FrameState level, current, next_record;
static unsigned seen_pc; static FrameState* seen_next; static bool null_result;
extern "C" FrameState* w15_t_fsf(unsigned pc, FrameState* result) { seen_pc = pc; seen_next = result; return null_result ? nullptr : &level; }
static unsigned lookup_reg = 0xffff;
extern "C" unsigned w15_t_get_reg_addr(unsigned reg, FrameState* state, FrameState* unused) { lookup_reg = reg; return 0x200; }
extern "C" FrameState* w15_next_stack_level(unsigned pc, FrameState* current, FrameState* next);
static void reset() {
    std::memset(&level, 0, sizeof level); std::memset(&current, 0, sizeof current); std::memset(arena, 0, sizeof arena);
    current.base = 0x100; lookup_reg = 0xffff; null_result = false;
}
int main() {
    reset(); null_result = true;
    if (w15_next_stack_level(0x4242, &current, &next_record) != nullptr || seen_pc != 0x4242 || seen_next != &next_record) return 1;
    reset();   // not saved: base comes from the first record, then the offset is added
    level.base_reg = 7; level.offset_from_base = 0x30;
    if (w15_next_stack_level(1, &current, &next_record) != &level || level.base != 0x130 || lookup_reg != 0xffff) return 2;
    reset();   // saved: base is the low 32 bits of the doubleword at the looked-up address
    level.base_reg = 7; level.offset_from_base = -4; current.saved[7] = 1;
    const unsigned long long wide = 0xdeadbeef00000500ull; std::memcpy(arena + 0x200, &wide, 8);
    if (w15_next_stack_level(1, &current, &next_record) != &level || level.base != 0x4fc || lookup_reg != 7) return 3;
    reset();   // indirect: load a word at base + indirect_offset, then add the offset
    level.base_reg = 3; level.indirect = 1; level.indirect_offset = 0x20; level.offset_from_base = 8;
    const unsigned pointed = 0x700; std::memcpy(arena + 0x120, &pointed, 4);
    if (w15_next_stack_level(1, &current, &next_record) != &level || level.base != 0x708) return 4;
    reset();   // indirect with a negative offset and a saved base register
    level.base_reg = 9; level.indirect = 0x40; level.indirect_offset = -0x10; level.offset_from_base = 0; current.saved[9] = 2;
    const unsigned wide_low = 0x300; std::memcpy(arena + 0x200, &wide_low, 4);
    const unsigned word_at = 0x555; std::memcpy(arena + 0x2f0, &word_at, 4);
    if (w15_next_stack_level(1, &current, &next_record) != &level || level.base != 0x555 || lookup_reg != 9) return 5;
    std::puts("next_stack_level ok");
    return 0;
}
