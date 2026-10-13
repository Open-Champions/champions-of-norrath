// Observed: SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary at 0x010cf850 is 156
// bytes. Arguments: material (a0), layer (a1), texture id (a2), raster (a3), dictionary (t0).
// With slot = material + 48 + 144 * layer: reads the old id at slot[0]. If it is not -1 it
// calls ReleaseSurface__8VIRasteriR12VIDictionary(raster, old, dictionary). It then stores
// the new id at slot[0]; if the new id is not -1 it calls ShareSurface__8VIRasteri(raster,
// new). Finally it stores 1 at material+32 and returns 0.
// Inferred: a material has an array of 144-byte layers starting at +48 whose first word is a
// surface/texture id (-1 = none) with reference counting through the raster; +32 is a
// "changed" flag. Release happens before the new surface is shared, even when both ids are
// equal.
// Unknown: the layer layout beyond word 0 and the meaning of the +32 flag.
extern "C" {
void ReleaseSurface__8VIRasteriR12VIDictionary(void* raster, int surface, void* dictionary);
void ShareSurface__8VIRasteri(void* raster, int surface);

int SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary(unsigned char* material, int layer, int texture, void* raster, void* dictionary) {
    int* slot = reinterpret_cast<int*>(material + 48 + 144 * layer);
    int old = *slot;
    if (old != -1) {
        ReleaseSurface__8VIRasteriR12VIDictionary(raster, old, dictionary);
    }
    *slot = texture;
    if (texture != -1) {
        ShareSurface__8VIRasteri(raster, texture);
    }
    *reinterpret_cast<int*>(material + 32) = 1;
    return 0;
}
}
