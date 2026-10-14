// Observed: Release__19VIStreamAudioSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide at 0x010192f8, 64 bytes. Arguments: the sprite (S), the
// scene, the dictionary (D), the raster, the sound device and the collide object. If the word at S+0 is nonzero it calls
// Remove__12VIDictionaryR10VIResource(D, S) and returns -1 when that result is negative; otherwise (or when the word is
// zero) it returns 0. Only the dictionary argument is used; no other member is read.
// Inferred: the word at S+0 marks a sprite that is registered in the dictionary. Unknown: its name and type.
extern "C" {
int Remove__12VIDictionaryR10VIResource(unsigned char* dictionary, unsigned char* resource);

int Release__19VIStreamAudioSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide(unsigned char* self, unsigned char*, unsigned char* dictionary, unsigned char*, unsigned char*, unsigned char*) {
    if (*reinterpret_cast<int*>(self) != 0 && Remove__12VIDictionaryR10VIResource(dictionary, self) < 0) return -1;
    return 0;
}
}
