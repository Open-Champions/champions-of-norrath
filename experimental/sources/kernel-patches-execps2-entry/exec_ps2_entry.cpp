// Observed: SysExecPS2Entry at 0x004451e0, 24 bytes of .data: the words 0x5a,
// the address of the local kCopy at 0x002dd960, 0x5b, 0x80074000,
// 0xffffc402, and 0. InitExecPS2 passes the first two pairs to its setup
// service, then queries the entry address for the word at index 4.
// Inferred: pairs of a service number and a handler or buffer address.
// Unknown: what the setup service does with a pair, and why the last pair's
// second word is zero.
extern "C" {
void kernel_execps2_pattern_kcopy();
unsigned SysExecPS2Entry[6] = {
    0x5au, static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(kernel_execps2_pattern_kcopy)),
    0x5bu, 0x80074000u,
    0xffffc402u, 0u,
};
}
