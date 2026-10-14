// Observed norrath_player_select_view_486d44 at 0x00486d44, 4 bytes, section .bss (local b, no ELF size; the extent is the gap to the next symbol). It is one of the
// file-local statics of the character selection unit; playerSelectionTask__Fv (authored in w14-hud-b-player-selection-task)
// reads and writes it. The per-frame step of that fade alpha; the image has two objects named flareDelta (0x00486cb4 and 0x00486d44) and playerSelectionTask__Fv uses the second.
// Inferred: float, flareDelta (in the original). Unknown: the original type name, and the other functions of the unit that use it.
extern "C" {
float norrath_player_select_view_486d44;
}
