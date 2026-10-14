// Observed: mt_ebx (0x004341a8) and mt_eby (0x004345a8) are two 1024-byte initialized-data
// objects in the pinned SLUS_205.65 symbol table, adjacent, between mt_tiledata (320 bytes)
// and levelSeeds. Every byte of both reads as zero in the file image. The megatile entity
// routines read them as float arrays: the walk tests whether the first float of mt_ebx is
// -2.0, treats -1.0 as a group end and -2.0 as the list end, and pairs mt_ebx[i] with
// mt_eby[i].
// Inferred: 256 floats each, filled at run time (nothing in the file image is nonzero) by
// code outside this unit.
// Unknown: the writers, and whether the length is exactly 256 floats (it is the symbol size).
extern "C" {
alignas(4) float mt_ebx[256];
alignas(4) float mt_eby[256];
}
