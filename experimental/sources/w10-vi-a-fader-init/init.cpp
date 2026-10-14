// Observed: Init__7VIFaderP8VIRasterP12VIDictionaryf at 0x0117cc88 is 140
// bytes. Arguments: object, VIRaster pointer, VIDictionary pointer, float f
// (f12). It stores the raster pointer at offset 0, f at offset 8, zero at
// offsets 12 and 16, then calls CreateMaterial__8VIRasteri(raster, 1) and
// stores the result at offset 20. A negative result is returned as -1. Otherwise
// it fetches Material__8VIRasteri(raster, index), calls
// InitSolidFill__10VIMaterial on it, SetLayerBlendMode__10VIMateriali16VIAlphaBlendMode
// (material, 0, 3), stores the 64-bit result of GetTime__7VITimer() at offset 24
// and returns 0.
// Observed: the dictionary argument is not read or stored; offset 4 is never
// written here, although Free__7VIFader reads offset 4.
// Unknown: the VIAlphaBlendMode meaning of 3, the material layer meaning of 1
// and 0.
extern "C" {
int CreateMaterial__8VIRasteri(void* raster, int count);
void* Material__8VIRasteri(void* raster, int index);
void InitSolidFill__10VIMaterial(void* material);
void SetLayerBlendMode__10VIMateriali16VIAlphaBlendMode(void* material, int layer, int mode);
long long GetTime__7VITimer();

int Init__7VIFaderP8VIRasterP12VIDictionaryf(void* object, void* raster, void* dictionary, float level) {
    (void)dictionary;
    unsigned char* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<unsigned*>(bytes) = static_cast<unsigned>(reinterpret_cast<unsigned long>(raster));
    *reinterpret_cast<float*>(bytes + 8) = level;
    *reinterpret_cast<unsigned*>(bytes + 12) = 0;
    *reinterpret_cast<unsigned*>(bytes + 16) = 0;
    int index = CreateMaterial__8VIRasteri(raster, 1);
    *reinterpret_cast<int*>(bytes + 20) = index;
    if (index < 0) return -1;
    void* material = Material__8VIRasteri(raster, index);
    InitSolidFill__10VIMaterial(material);
    SetLayerBlendMode__10VIMateriali16VIAlphaBlendMode(material, 0, 3);
    *reinterpret_cast<long long*>(bytes + 24) = GetTime__7VITimer();
    return 0;
}
}
