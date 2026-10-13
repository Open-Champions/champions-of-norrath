// Observed: loadSlot at 0x00486980 is one zero-initialised byte in BSS, followed directly by
// loadCompleteFlag (0x00486981) and loadPage (0x00486982). The texture page loader reads and
// writes it as a signed byte and uses it to index the per-slot texture page tables.
// Inferred: a signed byte slot number, from the signed byte accesses.
// Unknown: its value range, and other users.
extern "C" signed char loadSlot;
signed char loadSlot;
