// Observed norrath_player_select_view_486d3c at 0x00486d3c, 4 bytes, section .bss (local b, no ELF size; the extent is the gap to the next symbol). It is one of the
// file-local statics of the character selection unit; playerSelectionTask__Fv (authored in w14-hud-b-player-selection-task)
// reads and writes it. The fade alpha of the selection prompts; the image has three objects named flareAlpha (0x00486cb0, 0x00486d3c and 0x00486d90) and playerSelectionTask__Fv uses the middle one.
// Inferred: float, flareAlpha (in the original). Unknown: the original type name, and the other functions of the unit that use it.
extern "C" {
float norrath_player_select_view_486d3c;
}
