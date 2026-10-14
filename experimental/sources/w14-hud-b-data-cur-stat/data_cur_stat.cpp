// Observed curStat at 0x00486d2c, 1 byte, section .bss (local b, no ELF size; the extent is the gap to the next symbol). It is one of the
// file-local statics of the character selection unit; playerSelectionTask__Fv (authored in w14-hud-b-player-selection-task)
// reads and writes it.
// Inferred: unsigned char, the stat row selected on the allocation page, 0..3. Unknown: the original type name, and the other functions of the unit that use it.
extern "C" {
unsigned char curStat;
}
