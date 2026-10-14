// Host check only; not an EE build input. Build the source with -Dget_reg_addr=w15_get_reg_addr -Dabort=w15_abort;
// this file is compiled without them. abort is stubbed with a longjmp so the no-return branch is observable.
#include <csetjmp>
#include <initializer_list>
#include <cstdio>
#include <cstring>
#include "../w15-libs-eh-a-common/eh.hpp"
using w15eh::FrameState;
static std::jmp_buf trap;
extern "C" void w15_abort() { std::longjmp(trap, 1); }
extern "C" unsigned w15_get_reg_addr(unsigned reg, FrameState* state, FrameState* unused);
int main() {
    static FrameState s;
    std::memset(&s, 0, sizeof s);
    s.base = 0x1000;
    s.saved[5] = w15eh::kSavedAtOffset; s.reg_or_offset[5] = -24;
    s.saved[6] = w15eh::kSavedInRegister; s.reg_or_offset[6] = 5;
    s.saved[7] = w15eh::kSavedInRegister; s.reg_or_offset[7] = 6;
    s.saved[8] = w15eh::kSavedAtOffset; s.reg_or_offset[8] = 0x100000008ll;   // only the low 32 bits count
    s.saved[9] = w15eh::kSavedInRegister; s.reg_or_offset[9] = 0x100000005ll; // likewise for a register number
    s.saved[10] = w15eh::kSavedAtOffset; s.reg_or_offset[10] = 4;
    if (w15_get_reg_addr(5, &s, nullptr) != 0x1000u - 24) return 1;
    if (w15_get_reg_addr(6, &s, nullptr) != 0x1000u - 24) return 2;   // one hop
    if (w15_get_reg_addr(7, &s, nullptr) != 0x1000u - 24) return 3;   // two hops
    if (w15_get_reg_addr(8, &s, nullptr) != 0x1008u) return 4;
    if (w15_get_reg_addr(9, &s, nullptr) != 0x1000u - 24) return 5;
    if (w15_get_reg_addr(10, &s, &s) != 0x1004u) return 6;            // third argument ignored
    for (unsigned reg : {0u, 1u, 11u, 122u}) {                         // not saved: abort
        if (setjmp(trap) == 0) { w15_get_reg_addr(reg, &s, nullptr); return 8; }
    }
    s.saved[11] = 3;                                                   // unknown state: abort
    if (setjmp(trap) == 0) { w15_get_reg_addr(11, &s, nullptr); return 9; }
    s.saved[12] = w15eh::kSavedInRegister; s.reg_or_offset[12] = 13;   // chain ends in an unsaved register
    if (setjmp(trap) == 0) { w15_get_reg_addr(12, &s, nullptr); return 10; }
    std::puts("get_reg_addr ok");
    return 0;
}
