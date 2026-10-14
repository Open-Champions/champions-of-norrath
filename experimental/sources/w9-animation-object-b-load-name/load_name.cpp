// Observed loadName at 0x8d14a0 in .bss. The next symbol, slotsCorrupted, begins at
// 0x8d14c0, so the extent is 32 bytes. The only reader in the authored sources
// compares a requested name with it using strcmp, so it holds a text string.
// Unknown: who writes it and its declared capacity (32 is the distance to the next
// symbol, not a type).
extern "C" {
char loadName[32];
}
