// Observed: Release__14VISimpleSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide at 0x01167198, 156 bytes. Arguments: the sprite (S), the scene, the
// dictionary (D), the raster (RS), the sound device and the collide object (C). If the word at S+0 is nonzero it calls
// Remove__12VIDictionaryR10VIResource(D, S) and returns -1 when the result is negative. Then, if RS is nonnull, it calls
// ReleasePrimBuffer__8VIRasteriR12VIDictionary(RS, word at S+64, D) and ReleaseMaterialPal__8VIRasteriR12VIDictionary(RS,
// word at S+72, D). C is not tested: ReleaseCollBuffer__9VICollidei(C, word at S+68) is always called. Results other than
// Remove's are not used. It returns 0.
// Inferred: the three words are a primitive-buffer handle, a collision-buffer handle and a material-palette handle.
// Unknown: their names.
extern "C" {
int Remove__12VIDictionaryR10VIResource(unsigned char* dictionary, unsigned char* resource);
int ReleasePrimBuffer__8VIRasteriR12VIDictionary(unsigned char* raster, int buffer, unsigned char* dictionary);
int ReleaseMaterialPal__8VIRasteriR12VIDictionary(unsigned char* raster, int palette, unsigned char* dictionary);
int ReleaseCollBuffer__9VICollidei(unsigned char* collide, int buffer);

int Release__14VISimpleSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide(unsigned char* self, unsigned char*, unsigned char* dictionary, unsigned char* raster, unsigned char*, unsigned char* collide) {
    if (*reinterpret_cast<int*>(self) != 0 && Remove__12VIDictionaryR10VIResource(dictionary, self) < 0) return -1;
    if (raster != 0) {
        ReleasePrimBuffer__8VIRasteriR12VIDictionary(raster, *reinterpret_cast<int*>(self + 64), dictionary);
        ReleaseMaterialPal__8VIRasteriR12VIDictionary(raster, *reinterpret_cast<int*>(self + 72), dictionary);
    }
    ReleaseCollBuffer__9VICollidei(collide, *reinterpret_cast<int*>(self + 68));
    return 0;
}
}
