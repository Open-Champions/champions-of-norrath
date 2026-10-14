// Observed: the local SysEntry at 0x004451f8, 8 bytes of .data: the word 0x5a
// and the address of the local kCopy at 0x002ddad0. SetArg passes the two
// words to its setup service (service 116) before it copies any text.
// Several local objects in the executable are named SysEntry; this one is
// published as kernel_setarg_sysentry, an added name.
// Inferred: a service number and a handler address.
// Unknown: what the setup service does with the pair.
extern "C" {
void kernel_setarg_pattern_kcopy();
unsigned kernel_setarg_sysentry[2] = {
    0x5au, static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(kernel_setarg_pattern_kcopy)),
};
}
