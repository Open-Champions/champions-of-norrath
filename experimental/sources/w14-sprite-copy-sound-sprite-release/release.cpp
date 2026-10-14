// Observed: Release__13VISoundSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide at 0x0117c3c8, 176 bytes. Arguments: the sprite (S), the scene, the
// dictionary (D), the raster, the sound device (SD) and the collide object. If the word at S+0 is nonzero it calls
// Remove__12VIDictionaryR10VIResource(D, S) and returns -1 when the result is negative. Then, if the word at S+68 is not -1
// and IsPlaying__13VISoundDevicei(SD, word at S+68) is nonzero, it calls Stop__13VISoundDevicei(SD, word at S+68) and stores -1
// at S+68. If SD is nonnull it calls ReleaseSound__13VISoundDeviceiR12VIDictionary(SD, word at S+64, D). It returns 0. SD is
// not tested before the first two calls.
// Inferred: S+68 is a playing-voice id with -1 meaning none; S+64 is a sound handle. Unknown: their names and result types.
extern "C" {
int Remove__12VIDictionaryR10VIResource(unsigned char* dictionary, unsigned char* resource);
int IsPlaying__13VISoundDevicei(unsigned char* device, int voice);
void Stop__13VISoundDevicei(unsigned char* device, int voice);
void ReleaseSound__13VISoundDeviceiR12VIDictionary(unsigned char* device, int sound, unsigned char* dictionary);

int Release__13VISoundSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide(unsigned char* self, unsigned char*, unsigned char* dictionary, unsigned char*, unsigned char* device, unsigned char*) {
    if (*reinterpret_cast<int*>(self) != 0 && Remove__12VIDictionaryR10VIResource(dictionary, self) < 0) return -1;
    if (*reinterpret_cast<int*>(self + 68) != -1 && IsPlaying__13VISoundDevicei(device, *reinterpret_cast<int*>(self + 68)) != 0) {
        Stop__13VISoundDevicei(device, *reinterpret_cast<int*>(self + 68));
        *reinterpret_cast<int*>(self + 68) = -1;
    }
    if (device != 0) ReleaseSound__13VISoundDeviceiR12VIDictionary(device, *reinterpret_cast<int*>(self + 64), dictionary);
    return 0;
}
}
