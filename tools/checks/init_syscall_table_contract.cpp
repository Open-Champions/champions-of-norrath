#include <cstdio>
static int setups, finds;
extern "C" unsigned kernel_syscall_sysentry[4] = {1, 2, 3, 4};
extern "C" unsigned char kFindAddress[4] = {}, kernel_syscall_pattern_kcopy[4] = {};
extern "C" unsigned _SyscallEntry = 0;
extern "C" void kernel_syscall_table_setup(unsigned a, unsigned b) {
    if (setups == 0 && (a != 1 || b != 2)) __builtin_trap();
    if (setups == 1 && (a != 3 || b != 4)) __builtin_trap();
    ++setups;
}
static int mismatch;
extern "C" unsigned FindAddress(unsigned start, unsigned end, const void* pattern) {
    if (end != 0x80080000u) return 0;
    ++finds;
    if (!mismatch) {
        if (start != 0x80000000u) return 0;
        return pattern == kFindAddress ? 1524u : 1360u;
    }
    if (pattern == kFindAddress) {
        if (start == 0x80000000u) return 1000u;
        if (start == 1004u) return 1524u;
    } else if (start == 0x80000000u) {
        return 1360u;
    }
    return 0;
}
extern "C" void InitSystemCallTableAddress();
int main() {
    InitSystemCallTableAddress();
    if (setups != 2 || finds != 2 || _SyscallEntry != 1000u) return 1;
    mismatch = 1;
    finds = 0;
    _SyscallEntry = 0;
    InitSystemCallTableAddress();
    if (finds != 3 || _SyscallEntry != 1000u) return 2;
    std::puts("syscall table search passed");
}
