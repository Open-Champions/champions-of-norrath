// Observed: Free__7VIFader at 0x0117cd18 is 76 bytes and returns nothing. The
// word at offset 0 is a VIRaster pointer and the word at offset 20 a material
// index. When the raster pointer is nonzero and the index is not negative it
// calls ReleaseMaterial__8VIRasteri R12VIDictionary with (raster, index, the
// dictionary pointer stored at offset 4). It then clears the raster pointer and
// sets the index to -1; the index is -1 afterwards on every path.
// Observed: Init__7VIFader receives a dictionary pointer but never stores it, so
// offset 4 is written by some other code. Inferred: offset 4 holds a VIDictionary
// pointer, from the parameter type of ReleaseMaterial.
// Unknown: the rest of the VIFader layout is established per field by the
// other w10-vi-a fader units.
extern "C" {
void ReleaseMaterial__8VIRasteriR12VIDictionary(void* raster, int material, void* dictionary);

void Free__7VIFader(void* object) {
    int* words = static_cast<int*>(object);
    void* raster = reinterpret_cast<void*>(static_cast<unsigned long>(static_cast<unsigned>(words[0])));
    if (raster != nullptr) {
        int material = words[5];
        if (material >= 0) {
            ReleaseMaterial__8VIRasteriR12VIDictionary(
                raster, material,
                reinterpret_cast<void*>(static_cast<unsigned long>(static_cast<unsigned>(words[1]))));
        }
    }
    words[0] = 0;
    words[5] = -1;
}
}
