// Observed: Begin2D__4VIUI at 0x0102fa98, 28 bytes. It loads the raster pointer stored at offset 72 of the
// VIUI object (the same slot Init__4VIUI fills): it calls Begin2D__8VIRaster with that
// pointer and returns without using the result.
// Inferred: a thin forwarder that starts 2D drawing on the UI's raster.
// Unknown: the return type of Begin2D__8VIRaster (treated as void here, result ignored by the original).
extern "C" {
struct VIUI;
struct VIRaster;

void Begin2D__8VIRaster(VIRaster* raster);

void Begin2D__4VIUI(VIUI* ui) {
    VIRaster* raster = *reinterpret_cast<VIRaster**>(reinterpret_cast<char*>(ui) + 72);
    Begin2D__8VIRaster(raster);
}
}
