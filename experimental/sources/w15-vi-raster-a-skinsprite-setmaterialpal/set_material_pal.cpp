// Observed: SetMaterialPal__12VISkinSpriteiR8VIRasterR12VIDictionary at 0x0116d600 is 120 bytes in the pinned SLUS-20565 image.
// Arguments: the sprite (this), a palette index, the raster, the dictionary. It calls
// ReleaseMaterialPal__8VIRasteriR12VIDictionary(raster, word at this+72, dictionary); a negative result returns -1 and leaves the
// word unchanged. Otherwise it stores the new index at this+72, calls ShareMaterialPal__8VIRasteri(raster, index), and returns -1 if
// that result is negative, else 0.
// Inferred: this+72 holds the sprite's current material-palette index; the sprite drops its old palette and takes a share of the new one.
// Unknown: ReleaseMaterialPal's behavior and the sprite layout beyond this word. Nothing here ran on the EE.
extern "C" {
int ReleaseMaterialPal__8VIRasteriR12VIDictionary(unsigned char* raster, int index, unsigned char* dictionary);
int ShareMaterialPal__8VIRasteri(unsigned char* raster, int index);
int SetMaterialPal__12VISkinSpriteiR8VIRasterR12VIDictionary(unsigned char* self, int index, unsigned char* raster, unsigned char* dictionary) {
    if (ReleaseMaterialPal__8VIRasteriR12VIDictionary(raster, *reinterpret_cast<int*>(self + 72), dictionary) < 0) return -1;
    *reinterpret_cast<int*>(self + 72) = index;
    return ShareMaterialPal__8VIRasteri(raster, index) < 0 ? -1 : 0;
}
}
