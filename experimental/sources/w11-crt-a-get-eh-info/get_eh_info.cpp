// Observed __get_eh_info at 0x002fa108, 40 bytes, pinned SLUS_205.65.
// Calls the function whose address is stored in the data word get_eh_context
// (0x004480dc; the same pointer __get_eh_context calls) and returns that result
// plus 8 bytes. A null result is not special-cased, so the caller sees 8.
// Inferred: word 2 of the per-thread exception context is the "info" record that the
// C++ exception code takes the address of (the __throw routine tests the context's
// word at +8 for zero). Unknown: the record's declared type.
extern "C" {
extern void* (*get_eh_context)();

void* __get_eh_info() {
    return static_cast<unsigned char*>(get_eh_context()) + 8;
}
}
