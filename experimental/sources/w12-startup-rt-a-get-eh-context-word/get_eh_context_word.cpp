// Observed get_eh_context at 0x004480dc, pinned SLUS_205.65: a four-byte function pointer in the
// initialised data section (a file-local symbol of the C++ exception runtime). The image holds
// 0x002fa350, the address of the local routine eh_context_initialize, which replaces the pointer with
// eh_context_static on its first call. __get_eh_context calls through it; __throw reads it too.
// The initial value is a relocation to eh_context_initialize (authored in
// w12-startup-rt-a-eh-context-initialize).
// Unknown: whether anything other than eh_context_initialize ever stores to it.
extern "C" {
void* eh_context_initialize();
void* (*get_eh_context)() = eh_context_initialize;
}
