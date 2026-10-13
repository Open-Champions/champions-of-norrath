// Observed eh_context_initialize at 0x002fa350, 100 bytes, pinned SLUS_205.65 (a local routine of the
// C++ exception runtime). It is the first value of the function pointer get_eh_context (0x004480dc).
// Read from the code:
//   1. when get_eh_context still holds this routine's own address, it is replaced by eh_context_static
//      (another local routine, imported by name);
//   2. when the word dwarf_reg_size_table_initialized (0x004480e0) is zero, init_reg_size_table() runs;
//   3. the routine returns what the (possibly replaced) get_eh_context function returns.
// The pointer is read again after init_reg_size_table. Inferred: the first exception lookup switches to
// the single static context and later calls go straight there. Unknown: why the table setup sits here.
extern "C" {
extern void* (*get_eh_context)();
extern int dwarf_reg_size_table_initialized;
void* eh_context_static();
void init_reg_size_table();

void* eh_context_initialize() {
    if (get_eh_context == eh_context_initialize) get_eh_context = eh_context_static;
    if (dwarf_reg_size_table_initialized == 0) init_reg_size_table();
    return get_eh_context();
}
}
