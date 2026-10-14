// Observed: Release__20VIEffectVolumeSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide at 0x0114e878, 96 bytes. Arguments: the sprite (S), the scene, the
// dictionary (D) and three more that are not used. It first calls Free__14VIEffectVolume(S + 88), then, if the word at S+0
// is nonzero, calls Remove__12VIDictionaryR10VIResource(D, S). It returns -1 when that result is negative, otherwise 0.
// Inferred: the object at S+88 is an embedded effect-volume record. Unknown: the layout of the record.
extern "C" {
void Free__14VIEffectVolume(unsigned char* volume);
int Remove__12VIDictionaryR10VIResource(unsigned char* dictionary, unsigned char* resource);

int Release__20VIEffectVolumeSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide(unsigned char* self, unsigned char*, unsigned char* dictionary, unsigned char*, unsigned char*, unsigned char*) {
    Free__14VIEffectVolume(self + 88);
    if (*reinterpret_cast<int*>(self) != 0 && Remove__12VIDictionaryR10VIResource(dictionary, self) < 0) return -1;
    return 0;
}
}
