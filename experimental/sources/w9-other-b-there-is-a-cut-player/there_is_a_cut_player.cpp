// Observed: thereIsACutPlayer is one byte at 0x0048459c in initialized data; the symbol table size is 1
// and the file byte is 0 (the next symbol, eqLumpHeader, is 4 bytes on). cutClear__Fv and the Player
// constructors/destructor store a byte to it; msg_draw__6Player loads a signed byte.
// Inferred: a boolean flag.
// Unknown: which condition sets it besides the stores seen.

extern "C" unsigned char thereIsACutPlayer = 0;
static_assert(sizeof(thereIsACutPlayer) == 1, "thereIsACutPlayer");
