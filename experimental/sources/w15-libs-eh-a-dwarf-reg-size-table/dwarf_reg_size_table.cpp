// Observed dwarf_reg_size_table at 0x008eaa98: 124 bytes of uninitialised data (the extent runs to the next
// object, marker.0 at 0x008eab14). init_reg_size_table (0x002fa130) writes entries 0..30 and 32..121 once; copy_reg
// (0x002fabc0) reads one signed byte per register number as a copy length. Entry 31 and entries 122..123 are never
// written by anything seen. The image has no initial value (BSS).
// Inferred: one byte of size per unwinder register number; the declared extent of 124 is the gap to the next symbol.
extern "C" {
signed char dwarf_reg_size_table[124];
}
