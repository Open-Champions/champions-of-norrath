// Observed _8VIRaster$DBuffer at 0x01227f80 in the pinned image: 816 (0x330) bytes, all zero in
// the file, inside the file-backed data of the VI module segment. The raster routines pass its
// address to sceGsSetDefDBuffDc and sceGsSwapDBuffDc, and read words at offsets 384 and 752
// (see the BeginScene, Init and SetClearColor units). The size is the observed symbol extent
// and matches the 0x330 used by those units' extern declarations.
// Inferred: this is the SDK double-buffer descriptor that those two calls fill in; no field of it
// is named here. Unknown: the internal layout beyond the two offsets above.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
unsigned char norrath_raster_dbuffer[0x330] __asm__("_8VIRaster$DBuffer") NORRATH_LOADED(".data") = {0};
}
