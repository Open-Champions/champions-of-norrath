// Observed: itemToDestroy is the 4-byte bss word at 0x00486abc. Item counting
// stores a game-object pointer in it and the shop grid run code reads it back.
// Unknown: how the pointer is used after being read.
extern "C" void* itemToDestroy = nullptr;
