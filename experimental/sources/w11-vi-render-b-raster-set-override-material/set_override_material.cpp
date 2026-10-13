// Observed: SetOverrideMaterial__8VIRaster at 0x0110e008 is 104 bytes. It always stores zero at
// offset 17804 of the raster first. If the word at 17808 is nonzero it picks the value at offset
// 17840; otherwise, if the word at 17812 is nonzero, it picks the value at offset 17828;
// otherwise it returns zero with nothing else done. With a picked value it calls
// AddMaterial__8VIRasteriPP10_sceDmaTag(raster, value, raster+19492); if that result is not
// negative it also calls FlushDMABufferIfFull__8VIRaster(raster). In both cases it then stores 1
// at offset 17804. The result is zero.
// Inferred: 17804 marks "an override material is being applied" for the duration of the call;
// 17808 and 17812 are two override-slot-present words with their values at 17840 and 17828.
// Unknown: what the two slots are and why they are checked in this order.
extern "C" {
struct VIRaster;

int AddMaterial__8VIRasteriPP10_sceDmaTag(VIRaster* raster, int material, void* tagPointer);
int FlushDMABufferIfFull__8VIRaster(VIRaster* raster);

int SetOverrideMaterial__8VIRaster(VIRaster* raster) {
    char* base = reinterpret_cast<char*>(raster);
    int* applying = reinterpret_cast<int*>(base + 17804);
    int material;
    *applying = 0;
    if (*reinterpret_cast<int*>(base + 17808) != 0) {
        material = *reinterpret_cast<int*>(base + 17840);
    } else {
        if (*reinterpret_cast<int*>(base + 17812) == 0) return 0;
        material = *reinterpret_cast<int*>(base + 17828);
    }
    if (AddMaterial__8VIRasteriPP10_sceDmaTag(raster, material, base + 19492) >= 0)
        FlushDMABufferIfFull__8VIRaster(raster);
    *applying = 1;
    return 0;
}
}
