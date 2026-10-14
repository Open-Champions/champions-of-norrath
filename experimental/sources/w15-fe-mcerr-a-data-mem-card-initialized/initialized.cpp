// Observed: sMemCardInitialized at 0x01192c98, 4 bytes, zero in the image. The VIMemCard functions (LazyInit,
// Pad, Unpad, PadAsync, Status, ReadFile, WriteFile, GetDir, DeleteFile and their async variants) address it
// together with sListIcon and sProductTitle.
// Inferred: a word flag that records whether the memory-card layer has set itself up, from the name and the
// zero initial value; the symbol is local in the image.
// Unknown: its exact values beyond the initial zero and which routines set it.
extern "C" {
int sMemCardInitialized;
}
