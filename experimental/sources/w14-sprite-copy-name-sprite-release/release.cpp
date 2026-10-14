// Observed: Release__12VINameSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide at 0x010a1220, 132 bytes. Arguments: the sprite (S), the scene, the
// dictionary (D), the raster (RS), the sound device and the collide object. If the word at S+0 is nonzero it calls
// Remove__12VIDictionaryR10VIResource(D, S) and returns -1 when the result is negative. Then, if RS is nonnull, it calls
// ReleaseFont__8VIRasteriR12VIDictionary(RS, word at S+64, D) and returns -1 when that result is negative. Otherwise it returns 0.
// Inferred: the word at S+64 is a font handle obtained from the raster. Unknown: names of both words.
extern "C" {
int Remove__12VIDictionaryR10VIResource(unsigned char* dictionary, unsigned char* resource);
int ReleaseFont__8VIRasteriR12VIDictionary(unsigned char* raster, int font, unsigned char* dictionary);

int Release__12VINameSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide(unsigned char* self, unsigned char*, unsigned char* dictionary, unsigned char* raster, unsigned char*, unsigned char*) {
    if (*reinterpret_cast<int*>(self) != 0 && Remove__12VIDictionaryR10VIResource(dictionary, self) < 0) return -1;
    if (raster != 0 && ReleaseFont__8VIRasteriR12VIDictionary(raster, *reinterpret_cast<int*>(self + 64), dictionary) < 0) return -1;
    return 0;
}
}
