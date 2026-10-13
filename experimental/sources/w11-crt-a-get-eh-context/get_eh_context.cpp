// Observed __get_eh_context at 0x002fa0e0, 36 bytes, pinned SLUS_205.65.
// Calls the function whose address is stored in the four-byte data word
// get_eh_context (0x004480dc, a file-local symbol) with no arguments and returns
// the result unchanged. The word starts out holding the address of the local
// routine eh_context_initialize (0x002fa350), which suggests the pointer is
// replaced once the per-thread context is set up; that replacement is not part of
// this function. The pointer word itself is not defined here: it stays an
// unresolved import until its owner is reconstructed.
// Unknown: what the pointer is switched to after start-up.
extern "C" {
extern void* (*get_eh_context)();

void* __get_eh_context() {
    return get_eh_context();
}
}
