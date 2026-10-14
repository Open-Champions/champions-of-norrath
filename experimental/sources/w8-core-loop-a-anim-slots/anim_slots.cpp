// Observed anim at 0x007d0dc8 in zero-filled storage; the symbol table has two local symbols called anim
// (this one and a 16-byte one at 0x00790f60). This is the 32-byte one, between dlgName/optTbl and deferedCall,
// which the dialog message code indexes as pointer slots 0 and 4 (see the pre-logo-dialog package). Eight
// pointer-sized slots fill it.
// Inferred: that the import named anim refers to this one, from the dialog code's use of slots 0 and 4.
// Unknown: the purpose of slots 1-3 and 5-7, and what the 16-byte anim at 0x00790f60 is.
extern "C" {
void* anim[8];
}
