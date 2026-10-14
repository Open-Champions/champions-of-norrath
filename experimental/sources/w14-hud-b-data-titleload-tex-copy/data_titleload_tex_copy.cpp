// Observed titleloadTexCopy at 0x008dd280, 64 bytes, section .bss (local b, no ELF size). The next symbol,
// scrollmapTexCopy, starts 0x40 bytes later. coreStartLoadingScreen__Fv (authored in w13-frontend-game-a-core-start-loading-screen) copies one
// 64-byte texture record into it.
// Inferred: one 64-byte texture record, taken from the gap. Unknown: the record fields, and whether the true extent is 64.
extern "C" {
alignas(8) unsigned char titleloadTexCopy[64];
}
