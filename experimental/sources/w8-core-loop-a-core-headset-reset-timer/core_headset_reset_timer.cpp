// Observed coreHeadsetResetTimer at 0x00486690: 4 bytes of initialized data, value 0 in the file. gameLoop reads and writes it as a word and startNewWorld
// stores to it.
// Inferred: nothing beyond those accesses. No field meaning is claimed.
// Unknown: what the value represents and its full range.
extern "C" {
int coreHeadsetResetTimer = 0;
}
