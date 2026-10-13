// Storage for wLumpHeader: one word at 0x004845a4, initialised data in the original (zero, so BSS in the reviewed profile) with the
// file value 0. Observed use: a pointer value that the armor-cache release code compares with
// zero, passes to memFree__FPv, and sets back to zero (w8-net-play-c-player-deallocate-armor-caches).
// Inferred: it holds the address of a heap block while a lump header is loaded.
// Unknown: who allocates it and what the lump header holds.
extern "C" {
unsigned wLumpHeader = 0;
}
