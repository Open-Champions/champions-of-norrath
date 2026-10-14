// Observed __3Webii at 0x0020fac8, 24 bytes (the two-argument constructor of
// the class named Web in the symbol table). Stores the first int at self+48 and
// the second at self+52, and zero words at self+8, self+28 and self+44. Nothing
// is returned and no other byte is touched. The name norrath_web_construct is
// a second name for the same function, used by authored callers. What the
// fields mean is not established. Nothing here was executed on the EE.
typedef int __attribute__((may_alias)) AliasedInt;

extern "C" void __3Webii(unsigned char* self, int first, int second) {
    *reinterpret_cast<AliasedInt*>(self + 48) = first;
    *reinterpret_cast<AliasedInt*>(self + 52) = second;
    *reinterpret_cast<AliasedInt*>(self + 8) = 0;
    *reinterpret_cast<AliasedInt*>(self + 28) = 0;
    *reinterpret_cast<AliasedInt*>(self + 44) = 0;
}

// One function, two authored names. The EE assembler and the host assembler
// both accept .set; Mach-O adds the leading underscore.
#if defined(__APPLE__)
asm(".globl _norrath_web_construct\n.set _norrath_web_construct, ___3Webii\n");
#else
asm(".globl norrath_web_construct\n.set norrath_web_construct, __3Webii\n");
#endif
