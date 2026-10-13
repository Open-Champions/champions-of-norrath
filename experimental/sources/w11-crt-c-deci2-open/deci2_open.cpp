// Observed sceDeci2Open at 0x002df858, 68 bytes. It builds four words on its
// stack: the first argument masked to 16 bits, the second argument, the third
// argument, and the address of the 4152-byte area at 0x008e4748 (the symbol
// table calls it userarea) with bit 29 set (0x20000000 added to the cached
// address). It returns Deci2Call(1, words).
// Inferred: Deci2Call is the kernel entry (syscall 124 in its body) with 1 as
// the open request; the third argument is the receive handler (sceTtyInit
// passes sceTtyHandler) and the second the option pointer given back to it.
// Unknown: what the fourth word is used for by the kernel.
extern "C" {
extern unsigned char norrath_deci2_user_area_view[];
int Deci2Call(int request, void* words);

int sceDeci2Open(int protocol, void* option, void (*handler)()) {
    unsigned words[4];
    words[0] = static_cast<unsigned>(protocol) & 0xffffu;
    words[1] = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(option));
    words[2] = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(handler));
    words[3] = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(norrath_deci2_user_area_view)) | 0x20000000u;
    return Deci2Call(1, words);
}
}
