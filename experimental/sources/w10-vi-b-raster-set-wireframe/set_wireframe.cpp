// Observed SetWireframe__8VIRasteri at 0x01113a88, 32 bytes. It stores the
// argument at offset 17808 (0x4590) of the raster, then calls
// SetOverrideMaterial__8VIRaster with the raster only, and returns zero.
// The mangled callee name lists no argument; the original leaves the flag
// in the second argument register, which this source does not reproduce.
extern "C" {
struct VIRaster;
void SetOverrideMaterial__8VIRaster(VIRaster* raster);

int SetWireframe__8VIRasteri(VIRaster* raster, int wireframe) {
    *reinterpret_cast<int*>(reinterpret_cast<char*>(raster) + 17808) = wireframe;
    SetOverrideMaterial__8VIRaster(raster);
    return 0;
}
}
