// Observed InitSystemCallTableAddress at 0x002dd7d8. The search window
// 0x80000000..0x80080000 and the adjustments 524 and 360 are observed.
// kCopy is a duplicated local name; this import is the copy at 0x002dd718.
extern "C" {
void kernel_syscall_table_setup(unsigned first, unsigned second);
unsigned FindAddress(unsigned start, unsigned end, const void* pattern);
extern unsigned kernel_syscall_sysentry[];
extern unsigned char kFindAddress[];
extern unsigned char kernel_syscall_pattern_kcopy[];
extern unsigned _SyscallEntry;
void InitSystemCallTableAddress() {
    kernel_syscall_table_setup(kernel_syscall_sysentry[0], kernel_syscall_sysentry[1]);
    kernel_syscall_table_setup(kernel_syscall_sysentry[2], kernel_syscall_sysentry[3]);
    unsigned found_pattern = FindAddress(0x80000000u, 0x80080000u, kFindAddress);
    unsigned found_copy = FindAddress(0x80000000u, 0x80080000u, kernel_syscall_pattern_kcopy);
    unsigned adjusted_pattern = found_pattern - 524u;
    unsigned adjusted_copy = found_copy - 360u;
    while (adjusted_pattern != adjusted_copy) {
        if (adjusted_pattern < adjusted_copy) {
            found_pattern = FindAddress(found_pattern + 4u, 0x80080000u, kFindAddress);
            adjusted_pattern = found_pattern - 524u;
        } else {
            found_copy = FindAddress(found_copy + 4u, 0x80080000u, kernel_syscall_pattern_kcopy);
            adjusted_copy = found_copy - 360u;
        }
    }
    _SyscallEntry = adjusted_pattern;
}
}
