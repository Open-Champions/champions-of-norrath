// Observed statAllocation at 0x00486d20, 4 bytes, section .bss (local b, no ELF size; the extent is the gap to the next symbol). It is one of the
// file-local statics of the character selection unit; playerSelectionTask__Fv (authored in w14-hud-b-player-selection-task)
// reads and writes it.
// Inferred: int, the stat points still to spend on the allocation page (20 at the start). Unknown: the original type name, and the other functions of the unit that use it.
extern "C" {
int statAllocation;
}
