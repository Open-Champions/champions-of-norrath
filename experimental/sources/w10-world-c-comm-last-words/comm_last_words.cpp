// Observed: lastlastCE is the 4-byte word at 0x0048662c, zero in the file. The comm
// processing routine stores the previous lastCE into it and compares the two.
// Inferred: none. Unknown: any other writer.
extern "C" int lastlastCE = 0;
static_assert(sizeof(lastlastCE) == 4, "observed size");
