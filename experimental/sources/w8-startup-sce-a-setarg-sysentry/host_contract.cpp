#include <cstdint>
#include <cstdio>
#include "setarg_sysentry.cpp"

// 32-bit addresses are only meaningful on the target; on the host the cast
// truncates, so the check compares the truncated value and the layout.
extern "C" void kernel_setarg_pattern_kcopy() {}

int main() {
    if (sizeof(kernel_setarg_sysentry) != 8) return 1;
    if (kernel_setarg_sysentry[0] != 0x5au) return 2;
    if (kernel_setarg_sysentry[1] != static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(kernel_setarg_pattern_kcopy))) return 3;
    std::puts("setarg sysentry contract passed");
    return 0;
}
