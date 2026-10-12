// Observed: recallEnter is one byte at 0x00483f3e in initialized data; the symbol table size is 1 and the
// file byte is 0. gameLoop__Fv, gameLoadWorld__FPc and the Player constructor load it as a signed byte
// (the constructor also loads it unsigned); playerRecallSpell__Fv and startNewWorld__Fv store it.
// Inferred: a flag or small index, since both signed and unsigned loads and byte stores appear.
// Unknown: the meaning of values other than zero.

extern "C" signed char recallEnter = 0;
static_assert(sizeof(recallEnter) == 1, "recallEnter");
