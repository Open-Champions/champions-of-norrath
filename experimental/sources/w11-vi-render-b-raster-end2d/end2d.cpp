// Observed: End2D__8VIRaster at 0x01113248 is 32 bytes. It stores zero at offset 17884 of the
// raster, calls Restore2D__8VIRaster(raster) (the store executes before the call), and returns
// zero.
// Inferred: the word at 17884 is an in-2D-or-strip mode flag; BeginTriStrip__8VIRaster stores 1
// there.
// Unknown: what Restore2D restores; it is not part of this unit.
extern "C" {
struct VIRaster;

int Restore2D__8VIRaster(VIRaster* raster);

int End2D__8VIRaster(VIRaster* raster) {
    *reinterpret_cast<int*>(reinterpret_cast<char*>(raster) + 17884) = 0;
    Restore2D__8VIRaster(raster);
    return 0;
}
}
