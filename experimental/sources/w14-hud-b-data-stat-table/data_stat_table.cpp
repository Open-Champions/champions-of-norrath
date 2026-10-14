// Observed statTable at 0x00437028, 16 bytes, section .data (local d, no ELF size; the next symbol, targets, starts 16 bytes later). Four
// 32-bit pointers, in the file, to the text keys "selectStatStrength" (0x004760b0), "selectStatIntel" (0x004760c8),
// "selectStatDexterity" (0x004760d8) and "selectStatStamina" (0x004760f0). playerSelectionTask__Fv (authored in
// w14-hud-b-player-selection-task) prints the four allocation rows from it, passing each pointer to textGetString__FPc.
// Inferred: the key of the label of each of the four stat rows (strength, intelligence, dexterity, stamina), in that order.
// The strings are authored here as literals, so the pointers differ from the image's. Unknown: the exact string extents
// (the keys are read as NUL-terminated text) and who else reads the table.
extern "C" {
const char* statTable[4] = {"selectStatStrength", "selectStatIntel", "selectStatDexterity", "selectStatStamina"};
}
