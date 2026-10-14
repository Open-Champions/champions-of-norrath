// Observed in the pinned image: _8VIRaster$ColorMatrix at 0x011b0190, 4 bytes, with initial value 0x0f (15) in the file. Like its
// neighbours _8VIRaster$PrimBuffer1 (0x011b0194) and _8VIRaster$RInitOffset (0x011b01a8) it lies in the file-backed data of the VI
// module, which the section table lists under .text, and the raster routines reach it by absolute address.
// Inferred: a small integer of the same family as the neighbouring offset words (probably an index into VU1 data memory); this is a
// reading of the name and value, not of any consumer of this symbol.
// Unknown: every reader and writer of this word, its unit, and whether it changes at run time (it is writable data here).
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
unsigned norrath_color_matrix __asm__("_8VIRaster$ColorMatrix") NORRATH_LOADED(".data") = 0x0f;
}
