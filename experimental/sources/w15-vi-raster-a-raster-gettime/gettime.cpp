// Observed: GetTime__8VIRaster at 0x01113e80 is 8 bytes in the pinned SLUS-20565 image. It returns the 64-bit value stored at offset
// 17872 (0x45d0) of the raster object.
// Inferred: the value is a time or tick counter kept by the raster (the name says time; the unit is not established).
// Unknown: who writes the field, its unit, and whether it is signed. Nothing here ran on the EE.
extern "C" {
unsigned long long GetTime__8VIRaster(unsigned char* self) {
    return *reinterpret_cast<unsigned long long*>(self + 17872);
}
}
