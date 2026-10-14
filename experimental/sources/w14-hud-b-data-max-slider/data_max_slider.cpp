// Observed maxSlider at 0x00486d32, 1 byte, section .bss (local b, no ELF size; the extent is the gap to the next symbol). It is one of the
// file-local statics of the character selection unit; playerSelectionTask__Fv (authored in w14-hud-b-player-selection-task)
// reads and writes it.
// Inferred: signed char, how many look sliders the chosen character has (3 or 4). Unknown: the original type name, and the other functions of the unit that use it.
extern "C" {
signed char maxSlider;
}
