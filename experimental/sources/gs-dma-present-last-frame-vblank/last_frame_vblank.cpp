// Observed lastFrameVBlankCount at 0x00483f24. Four .sdata bytes, file
// value 1. A second local symbol with the same name is BSS at 0x00486a38
// and is the address engineFrameEnd stores. This definition is the global
// object. The local word is not a second external symbol.
extern "C" unsigned lastFrameVBlankCount = 1u;
static_assert(sizeof(lastFrameVBlankCount) == 4, "one word");
