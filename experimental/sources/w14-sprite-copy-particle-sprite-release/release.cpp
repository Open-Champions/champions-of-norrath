// Observed: Release__16VIParticleSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide at 0x010aa740, 132 bytes. Arguments: the sprite (S), the scene (SC), the
// dictionary (D) and three more that are not used. If the word at S+0 is nonzero it calls
// Remove__12VIDictionaryR10VIResource(D, S) and returns -1 when the result is negative. Then, if the address SC + 1352 is
// nonzero and the word at S+64 is not -1, it calls ReleaseParticleDefinition__16VIParticleSystemiR12VIDictionary(SC + 1352,
// word at S+64, D). It returns 0.
// Inferred: SC + 1352 is an embedded particle system in the scene; the word at S+64 is a particle-definition id with -1 meaning
// none. Unknown: the layout of the scene and the names of both words.
extern "C" {
int Remove__12VIDictionaryR10VIResource(unsigned char* dictionary, unsigned char* resource);
void ReleaseParticleDefinition__16VIParticleSystemiR12VIDictionary(unsigned char* system, int definition, unsigned char* dictionary);

int Release__16VIParticleSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide(unsigned char* self, unsigned char* scene, unsigned char* dictionary, unsigned char*, unsigned char*, unsigned char*) {
    if (*reinterpret_cast<int*>(self) != 0 && Remove__12VIDictionaryR10VIResource(dictionary, self) < 0) return -1;
    unsigned char* system = scene + 1352;
    if (system != 0 && *reinterpret_cast<int*>(self + 64) != -1)
        ReleaseParticleDefinition__16VIParticleSystemiR12VIDictionary(system, *reinterpret_cast<int*>(self + 64), dictionary);
    return 0;
}
}
