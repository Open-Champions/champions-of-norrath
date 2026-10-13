// Observed init_reg_size_table at 0x002fa130, 520 bytes (a local routine of the C++ exception runtime),
// pinned SLUS_205.65. Called once by eh_context_initialize while dwarf_reg_size_table_initialized is zero.
// Fills the byte table dwarf_reg_size_table (0x008eaa98, uninitialised data) and sets the flag word
// dwarf_reg_size_table_initialized (0x004480e0) to 1. Entry values by index, as written by the image:
//   0..30   8        31      not written      32..63  4        64, 65  16       66  16
//   67..75  4        76..78  8                79..89  0        90..121 8
// Indices 0..121 are the only ones touched; the table's declared size is not visible from the image.
// Inferred: the index is a DWARF register number and the byte its size in bytes (4, 8, 16, or 0 for
// registers that are skipped) in the unwinder's frame description; this is read from the pattern, not confirmed.
// Unknown: why index 31 is left alone, and the real extent of the table.
extern "C" {
extern unsigned char dwarf_reg_size_table[];
extern int dwarf_reg_size_table_initialized;

static void fill(int first, int last, unsigned char size) {
    for (int i = first; i <= last; ++i) dwarf_reg_size_table[i] = size;
}

void init_reg_size_table() {
    fill(0, 30, 8);
    fill(32, 63, 4);
    fill(64, 66, 16);
    fill(67, 75, 4);
    fill(76, 78, 8);
    fill(79, 89, 0);
    fill(90, 121, 8);
    dwarf_reg_size_table_initialized = 1;
}
}
