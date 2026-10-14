// Observed throbR at 0x00486d70, 4 bytes, section .bss (local b, no ELF size; the extent is the gap to the next symbol). It is one of the
// file-local statics of the character selection unit; playerSelectionTask__Fv (authored in w14-hud-b-player-selection-task)
// reads and writes it.
// Inferred: int, red of the pulsing highlight colour. Unknown: the original type name, and the other functions of the unit that use it.
extern "C" {
int throbR;
}
