// Observed: the local SysEntry at 0x00445d50, 64 bytes of .data: the words
// 0x5a, the address of the local kCopy at 0x002dfb88, 0x5b, 0x80075000,
// 0x54, the address of _kExitTLBHandler (0x002e0100), then the pairs
// (0x55, 0), (0x56, 0), (0x57, 0), (0x58, 0), (0x59, 0). InitTLBFunctions
// passes the first three pairs to its setup service, then walks the
// remaining five: for each, it queries the entry address for the first word
// and passes both back to setup.
// Inferred: pairs of a service number and a handler or buffer address.
// Unknown: what the setup service does with a pair.
// _kExitTLBHandler is not defined here.
extern "C" {
void kernel_tlb_pattern_kcopy();
void _kExitTLBHandler();
unsigned kernel_tlb_sysentry[16] = {
    0x5au, static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(kernel_tlb_pattern_kcopy)),
    0x5bu, 0x80075000u,
    0x54u, static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_kExitTLBHandler)),
    0x55u, 0u,
    0x56u, 0u,
    0x57u, 0u,
    0x58u, 0u,
    0x59u, 0u,
};
}
