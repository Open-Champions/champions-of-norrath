// Observed in the pinned image: _8VIRaster$PrimBuffer1 at 0x011b0194, 4 bytes, with initial value 0x15 (21) in the file. The address lies in the
// file-backed data of the VI module, which the section table lists under .text. Routines in the raster family address this
// group of 4-byte statics by absolute address (for example UploadPerMaterial__8VIRasterPP10_sceDmaTag reads two of its neighbours).
// Inferred: the name ends in Offset or Buffer and the neighbouring words are small integers, so the value is probably an index
// into VU1 data memory; this is a reading of the name and value, not of any consumer of this symbol.
// Unknown: every reader and writer of this word, its unit, and whether it changes at run time (it is writable data here).
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
unsigned norrath_prim_buffer1 __asm__("_8VIRaster$PrimBuffer1") NORRATH_LOADED(".data") = 0x15;
}
