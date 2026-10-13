// Observed norrath_player_select_view_486d88 at 0x00486d88, 1 byte, section .bss (local b, no ELF size; the extent is the gap to the next symbol). It is one of the
// file-local statics of the character selection unit; playerSelectionTask__Fv (authored in w14-hud-b-player-selection-task)
// reads and writes it. The first-call flag of the selection task; the image has two objects named firstTime (0x00483e40, in .data, and 0x00486d88) and playerSelectionTask__Fv uses the .bss one.
// Inferred: signed char, firstTime (in the original). Unknown: the original type name, and the other functions of the unit that use it.
extern "C" {
signed char norrath_player_select_view_486d88;
}
