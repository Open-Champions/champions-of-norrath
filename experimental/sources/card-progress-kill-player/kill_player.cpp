// Observed: killPlayer is one byte. MC_Init stores zero with a byte store.
// The image byte is zero.
// Inferred: none.
// Unknown: which routine stores a nonzero value.
extern "C" unsigned char killPlayer = 0;
