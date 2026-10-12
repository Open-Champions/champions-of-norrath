// Observed: the local SysEntry at 0x00444a20, 16 bytes of .data: the words
// 0x83, the address of kFindAddress, 0x5a, and the address of
// kernel_syscall_pattern_kcopy. InitSystemCallTableAddress passes the two
// pairs to its setup service and the two addresses to FindAddress as
// search patterns.
// Inferred: each pair is a service number and a handler address.
// Unknown: what the setup service does with a pair.
extern "C" {
void kFindAddress();
void kernel_syscall_pattern_kcopy();
unsigned kernel_syscall_sysentry[4] = {
    0x83u, static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(kFindAddress)),
    0x5au, static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(kernel_syscall_pattern_kcopy)),
};
}
