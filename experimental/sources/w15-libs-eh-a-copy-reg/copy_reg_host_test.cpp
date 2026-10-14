// Host check only; not an EE build input. Build the source with -DW15_EH_HOST -Dcopy_reg=w15_copy_reg
// -Dget_reg_addr=w15_t_get_reg_addr -Dmemcpy=w15_t_memcpy; this file is compiled with -DW15_EH_HOST only.
// Address words are offsets into one arena; the stubs record the call order and the copy length.
#include <cstdio>
#include <cstring>
#include "../w15-libs-eh-a-common/eh.hpp"
using w15eh::FrameState;
static unsigned char arena[0x1000];
unsigned char* w15_eh_host_base = arena;
extern "C" signed char dwarf_reg_size_table[123];
signed char dwarf_reg_size_table[123];
static int order[4], calls;
static FrameState* seen_state[4];
extern "C" unsigned w15_t_get_reg_addr(unsigned reg, FrameState* state, FrameState* unused) {
    if (calls < 4) { order[calls] = static_cast<int>(reg); seen_state[calls] = state; }
    ++calls;
    return (state->base + reg * 16u) & 0x7ff;   // distinct location per state
}
static unsigned copied_len; static void* copied_to; static const void* copied_from;
extern "C" void* w15_t_memcpy(void* d, const void* s, unsigned long n) { copied_to = d; copied_from = s; copied_len = static_cast<unsigned>(n); return d; }
extern "C" void w15_copy_reg(unsigned reg, FrameState* from, FrameState* to);
int main() {
    static FrameState a, b;
    a.base = 0x100; b.base = 0x400;
    dwarf_reg_size_table[3] = 8; dwarf_reg_size_table[40] = 4; dwarf_reg_size_table[66] = 16; dwarf_reg_size_table[100] = 0;
    w15_copy_reg(3, &a, &b);
    if (calls != 2 || seen_state[0] != &a || seen_state[1] != &b || order[0] != 3 || order[1] != 3) return 1;
    if (copied_len != 8 || copied_from != arena + ((0x100 + 48) & 0x7ff) || copied_to != arena + ((0x400 + 48) & 0x7ff)) return 2;
    w15_copy_reg(40, &b, &a);
    if (copied_len != 4 || copied_from != arena + ((0x400 + 640) & 0x7ff) || copied_to != arena + ((0x100 + 640) & 0x7ff)) return 3;
    w15_copy_reg(66, &a, &b); if (copied_len != 16) return 4;
    w15_copy_reg(100, &a, &b); if (copied_len != 0) return 5;
    std::puts("copy_reg ok");
    return 0;
}
