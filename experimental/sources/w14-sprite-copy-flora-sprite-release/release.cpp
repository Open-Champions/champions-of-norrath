// Observed: Release__13VIFloraSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide at 0x010062e8, 132 bytes. Arguments: the sprite (S), the scene, the
// dictionary (D), the raster (RS), the sound device and the collide object. If the word at S+0 is nonzero it calls
// Remove__12VIDictionaryR10VIResource(D, S) and returns -1 when the result is negative. Then, if RS is nonnull, it calls
// ReleasePrimBuffer__8VIRasteriR12VIDictionary(RS, word at S+64, D) and ReleaseMaterialPal__8VIRasteriR12VIDictionary(RS,
// word at S+68, D); neither result is used. It returns 0.
// Inferred: the two words are a primitive-buffer handle and a material-palette handle. Unknown: their names.
extern "C" {
int Remove__12VIDictionaryR10VIResource(unsigned char* dictionary, unsigned char* resource);
int ReleasePrimBuffer__8VIRasteriR12VIDictionary(unsigned char* raster, int buffer, unsigned char* dictionary);
int ReleaseMaterialPal__8VIRasteriR12VIDictionary(unsigned char* raster, int palette, unsigned char* dictionary);

int Release__13VIFloraSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide(unsigned char* self, unsigned char*, unsigned char* dictionary, unsigned char* raster, unsigned char*, unsigned char*) {
    if (*reinterpret_cast<int*>(self) != 0 && Remove__12VIDictionaryR10VIResource(dictionary, self) < 0) return -1;
    if (raster != 0) {
        ReleasePrimBuffer__8VIRasteriR12VIDictionary(raster, *reinterpret_cast<int*>(self + 64), dictionary);
        ReleaseMaterialPal__8VIRasteriR12VIDictionary(raster, *reinterpret_cast<int*>(self + 68), dictionary);
    }
    return 0;
}
}
