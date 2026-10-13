// Observed dwarf_reg_size_table_initialized at 0x004480e0, pinned SLUS_205.65: a four-byte word in the
// initialised data section, zero in the image, right after the get_eh_context pointer. eh_context_initialize
// calls init_reg_size_table when it is zero.
// Placed in .data explicitly because a zero initialiser alone would go to .bss.
// Unknown: who sets it (init_reg_size_table is expected to; it is not authored here).
extern "C" {
#if defined(__mips__)
__attribute__((section(".data")))
#endif
int dwarf_reg_size_table_initialized = 0;
}
