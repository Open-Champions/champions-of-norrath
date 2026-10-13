// Observed mapScissorX (0x00486b10) and mapScissorY (0x00486b14): two adjacent
// 4-byte objects in the zero-filled data region (the symbol table gives them no
// size; each is four bytes up to the next symbol). Both are read and written as
// 32-bit words by gp-relative loads and stores.
// Inferred: the pair is a scissor origin of the map view; both are word-sized.
// Unknown: signedness, units and the writers' ranges. Both are defined here as
// plain int zero.
extern "C" {
int mapScissorX = 0;
int mapScissorY = 0;
}
