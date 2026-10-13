// Observed: BeginTriStrip__8VIRaster at 0x01113138 is 128 bytes. If the word at offset 19532 of
// the raster is not 1 it calls UploadRasterMicro__8VIRasterPP10_sceDmaTag(raster, raster+19492).
// If the word at 17888 is nonzero it then calls CalcMatrices__8VIRaster(raster) and
// UploadMatrices__8VIRasterPP10_sceDmaTag(raster, raster+19492). It reads the pointer stored at
// offset 19492 only after those calls, stores 1 at offset 17884, stores pointer+48 at offset 52,
// pointer+16 at offset 56 and zero at offset 48, and returns zero.
// Inferred: 19492 holds the DMA packet write cursor; the strip cursor fields at 48 to 56 point
// into the packet just reserved. 17888 is a matrices-dirty flag (cleared by code not in this unit).
// Unknown: what the words at 48, 52 and 56 are called, and whether they are a count and two
// pointers.
extern "C" {
struct VIRaster;

int UploadRasterMicro__8VIRasterPP10_sceDmaTag(VIRaster* raster, void* tagPointer);
int CalcMatrices__8VIRaster(VIRaster* raster);
int UploadMatrices__8VIRasterPP10_sceDmaTag(VIRaster* raster, void* tagPointer);

int BeginTriStrip__8VIRaster(VIRaster* raster) {
    char* base = reinterpret_cast<char*>(raster);
    if (*reinterpret_cast<int*>(base + 19532) != 1)
        UploadRasterMicro__8VIRasterPP10_sceDmaTag(raster, base + 19492);
    if (*reinterpret_cast<int*>(base + 17888) != 0) {
        CalcMatrices__8VIRaster(raster);
        UploadMatrices__8VIRasterPP10_sceDmaTag(raster, base + 19492);
    }
    unsigned cursor = *reinterpret_cast<unsigned*>(base + 19492);
    *reinterpret_cast<int*>(base + 17884) = 1;
    *reinterpret_cast<unsigned*>(base + 52) = cursor + 48;
    *reinterpret_cast<unsigned*>(base + 56) = cursor + 16;
    *reinterpret_cast<unsigned*>(base + 48) = 0;
    return 0;
}
}
