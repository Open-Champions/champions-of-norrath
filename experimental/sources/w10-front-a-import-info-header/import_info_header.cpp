// Observed: importInfoHeader at 0x008d1068 is a zero-initialised BSS buffer. The next object,
// iconHeader, begins 0x70 bytes later, and the memory card load of a player's information reads
// exactly 112 bytes into it (command 3 with size 112), the size of one slot header as
// the loaded slot image keeps it.
// Inferred: a 112-byte header buffer, from the spacing and the read size.
// Unknown: the layout of the header beyond what the callers read.
extern "C" char importInfoHeader[112];
char importInfoHeader[112];
