// Observed: SetTime__8VIRasterl at 0x01113e70 is 12 bytes. It stores its 64-bit argument at
// offset 17872 of the raster and returns zero.
// Unknown: the unit of the time value and which code reads it.
extern "C" {
struct VIRaster;

int SetTime__8VIRasterl(VIRaster* raster, long long time) {
    *reinterpret_cast<long long*>(reinterpret_cast<char*>(raster) + 17872) = time;
    return 0;
}
}
