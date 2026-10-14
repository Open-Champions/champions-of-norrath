/* Observed data object, pinned SLUS_205.65: gRomName at 0x00444858, 15 bytes, in the initialised data section
 * (its symbol is local to the image). All 15 bytes are zero at load, so the compiler places this definition in
 * zero-initialised storage; whether the original object sat in .data for a reason or by link order is unknown.
 * GetRomName fills the first 14 bytes once from a file and IsT10K tests the byte at offset 4; the 15th byte is
 * never written by the routines seen. Inferred: a ROM version string of the console. Unknown: its exact layout.
 */
extern "C" {
char gRomName[15];
}
