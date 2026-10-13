// Observed: SetMaterial__8VIRasteri at 0x01113888 is 68 bytes. It calls
// AddMaterial__8VIRasteriPP10_sceDmaTag(raster, material, raster+19492). If that result is
// negative it returns -1 without anything else; otherwise it calls
// FlushDMABufferIfFull__8VIRaster(raster) and returns zero.
// Inferred: the integer is a material handle and the raster+19492 slot is the DMA write cursor,
// as in the other callers of AddMaterial.
// Unknown: what AddMaterial's non-negative results mean.
extern "C" {
struct VIRaster;

int AddMaterial__8VIRasteriPP10_sceDmaTag(VIRaster* raster, int material, void* tagPointer);
int FlushDMABufferIfFull__8VIRaster(VIRaster* raster);

int SetMaterial__8VIRasteri(VIRaster* raster, int material) {
    if (AddMaterial__8VIRasteriPP10_sceDmaTag(raster, material, reinterpret_cast<char*>(raster) + 19492) < 0)
        return -1;
    FlushDMABufferIfFull__8VIRaster(raster);
    return 0;
}
}
