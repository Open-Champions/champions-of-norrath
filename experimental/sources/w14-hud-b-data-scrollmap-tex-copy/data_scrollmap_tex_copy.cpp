// Observed scrollmapTexCopy at 0x008dd2c0, 128 bytes, section .bss (local b, no ELF size). The next symbol,
// blackoutTexCopy, starts 0x80 bytes later. coreStartLoadingScreen__Fv (authored in w13-frontend-game-a-core-start-loading-screen) copies two
// consecutive 64-byte texture records into it (elements 0 and 1).
// Inferred: two 64-byte texture records, taken from the gap. Unknown: the record fields, and whether the true extent is 128.
extern "C" {
alignas(8) unsigned char scrollmapTexCopy[128];
}
