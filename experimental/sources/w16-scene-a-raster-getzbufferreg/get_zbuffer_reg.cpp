// Observed: GetZBufferReg__C8VIRaster at 0x01114930 is 12 bytes in the pinned SLUS-20565 image. It ignores `this` and returns the 64-bit
// doubleword at offset 480 (0x1e0) of the static object `_8VIRaster$DBuffer` (0x01227f80, 816 bytes) in one register.
// Inferred: the doubleword is a GS register image kept in the double-buffer record (the name says Z buffer); it is a plain read.
// Unknown: which GS register the value is, and who writes it. The data object is an import here. Nothing here ran on the EE.
extern unsigned char norrath_raster_dbuffer[0x330] __asm__("_8VIRaster$DBuffer");
extern "C" {
unsigned long long GetZBufferReg__C8VIRaster(const unsigned char*) {
    return *reinterpret_cast<unsigned long long*>(norrath_raster_dbuffer + 480);
}
}
