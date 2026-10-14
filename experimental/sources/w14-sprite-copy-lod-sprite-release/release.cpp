// Observed: Release__11VILODSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide at 0x01076a40, 160 bytes. Arguments: the sprite (S), the scene (SC), the
// dictionary (D) and three more that are not used. If the word at S+0 is nonzero it calls
// Remove__12VIDictionaryR10VIResource(D, S) and returns -1 when the result is negative. Then, for i from 0 while i is less than
// the word at S+68 (reread after every call), it calls ReleaseSprite__7VIScenei(SC, word at S+76+8*i). It returns 0.
// Inferred: the word at S+68 is a child count and the words are child sprite ids, spaced 8 bytes apart. Unknown: their names
// and the other bytes of each 8-byte child record.
extern "C" {
int Remove__12VIDictionaryR10VIResource(unsigned char* dictionary, unsigned char* resource);
void ReleaseSprite__7VIScenei(unsigned char* scene, int sprite);

int Release__11VILODSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide(unsigned char* self, unsigned char* scene, unsigned char* dictionary, unsigned char*, unsigned char*, unsigned char*) {
    if (*reinterpret_cast<int*>(self) != 0 && Remove__12VIDictionaryR10VIResource(dictionary, self) < 0) return -1;
    for (int i = 0; i < *reinterpret_cast<int*>(self + 68); ++i)
        ReleaseSprite__7VIScenei(scene, *reinterpret_cast<int*>(self + 76 + 8 * i));
    return 0;
}
}
