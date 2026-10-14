// Release__9VICSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide at 0x010b13a0, 748 bytes.
// Observed: it first calls Release__9VIHSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide(S, scene, dictionary, raster, sound, collide) with its own arguments and returns -1 if that is negative. Then, for i from 0 to 65: for the two
// words at S+424+52*i and S+428+52*i, a word that is not negative is passed to ReleaseAnimation__7VIScenei(scene, word), and a negative result returns -1; then, if the sound device is
// nonnull, for the two words at S+448+52*i and S+452+52*i, a word that is not negative is passed to ReleaseSound__13VISoundDeviceiR12VIDictionary(sound, word, dictionary), and a
// negative result returns -1. If the word at S+3856 is not -1 it calls ReleaseSprite__7VIScenei(scene, that word), returning -1 on a negative result. If the raster is nonnull it
// calls ReleaseMaterialPal__8VIRasteriR12VIDictionary(raster, word at S+416, dictionary) and then ReleaseRefMap__7VIScenei(scene, word at S+420), returning -1 on a negative result
// of either, and then for k from 0 to 18 passes the word at S+3908+24*k, when it is not -1, to ReleaseSurface__8VIRasteriR12VIDictionary(raster, word, dictionary) (results unused).
// Then for k from 0 to 2 the word at S+4372+12*k, when not -1, goes to ReleaseSprite__7VIScenei(scene, word) (results unused). If the pointer word at S+4576 is nonzero it calls
// VITrailFxEnd__FP9VITrailFx on it and stores 0 there. If the sound device is nonnull and the word at S+4592 is not -1, it calls Stop__13VISoundDevicei(sound, word at S+4600) when that word
// is not -1, and then ReleaseSound__13VISoundDeviceiR12VIDictionary(sound, word at S+4592, dictionary). Finally, for i below the word at S+4616 (reread each time), with the
// pointer word at S+4620 as the base of 1056-byte entries: when the entry's first word is not -1 it calls Destroy__17VIParticleEmitter(entry + 16) and then
// ReleaseParticleDefinition__16VIParticleSystemiR12VIDictionary(scene + 1352, that first word, dictionary). It returns 0. The collide argument is only forwarded to the base release.
// Inferred: the 66 repeated records hold animation and sound ids of the sprite's parts, scene+1352 is the embedded particle system (the null test on that address is folded away by
// the compiler), and S+4620 points to the sprite's particle emitters. Unknown: the names of every field, the layout of the 52-byte records and the 1056-byte entries beyond the
// words used, and why only some results are tested.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int Release__9VIHSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide(unsigned char* self, unsigned char* scene, unsigned char* dictionary, unsigned char* raster, unsigned char* sound, unsigned char* collide);
int ReleaseAnimation__7VIScenei(unsigned char* scene, int animation);
int ReleaseSound__13VISoundDeviceiR12VIDictionary(unsigned char* device, int sound, unsigned char* dictionary);
int ReleaseSprite__7VIScenei(unsigned char* scene, int sprite);
int ReleaseMaterialPal__8VIRasteriR12VIDictionary(unsigned char* raster, int palette, unsigned char* dictionary);
int ReleaseRefMap__7VIScenei(unsigned char* scene, int refMap);
void ReleaseSurface__8VIRasteriR12VIDictionary(unsigned char* raster, int surface, unsigned char* dictionary);
void VITrailFxEnd__FP9VITrailFx(unsigned trail);
void Stop__13VISoundDevicei(unsigned char* device, int voice);
void Destroy__17VIParticleEmitter(unsigned char* emitter);
void ReleaseParticleDefinition__16VIParticleSystemiR12VIDictionary(unsigned char* system, int definition, unsigned char* dictionary);

int Release__9VICSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide(unsigned char* self, unsigned char* scene, unsigned char* dictionary, unsigned char* raster, unsigned char* sound, unsigned char* collide) {
    if (Release__9VIHSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide(self, scene, dictionary, raster, sound, collide) < 0) return -1;
    unsigned char* system = scene + 1352;
    for (int i = 0; i < 66; ++i) {
        for (int j = 0; j < 2; ++j) {
            int animation = *reinterpret_cast<int*>(self + 424 + 52 * i + 4 * j);
            if (animation >= 0 && ReleaseAnimation__7VIScenei(scene, animation) < 0) return -1;
        }
        for (int j = 0; j < 2; ++j) {
            if (sound != 0) {
                int voice = *reinterpret_cast<int*>(self + 448 + 52 * i + 4 * j);
                if (voice >= 0 && ReleaseSound__13VISoundDeviceiR12VIDictionary(sound, voice, dictionary) < 0) return -1;
            }
        }
    }
    if (*reinterpret_cast<int*>(self + 3856) != -1 && ReleaseSprite__7VIScenei(scene, *reinterpret_cast<int*>(self + 3856)) < 0) return -1;
    if (raster != 0) {
        if (ReleaseMaterialPal__8VIRasteriR12VIDictionary(raster, *reinterpret_cast<int*>(self + 416), dictionary) < 0) return -1;
        if (ReleaseRefMap__7VIScenei(scene, *reinterpret_cast<int*>(self + 420)) < 0) return -1;
        for (int k = 0; k < 19; ++k) {
            int surface = *reinterpret_cast<int*>(self + 3908 + 24 * k);
            if (surface != -1) ReleaseSurface__8VIRasteriR12VIDictionary(raster, surface, dictionary);
        }
    }
    for (int k = 0; k < 3; ++k) {
        int part = *reinterpret_cast<int*>(self + 4372 + 12 * k);
        if (part != -1) ReleaseSprite__7VIScenei(scene, part);
    }
    if (*reinterpret_cast<unsigned*>(self + 4576) != 0) {
        VITrailFxEnd__FP9VITrailFx(*reinterpret_cast<unsigned*>(self + 4576));
        *reinterpret_cast<unsigned*>(self + 4576) = 0;
    }
    if (sound != 0 && *reinterpret_cast<int*>(self + 4592) != -1) {
        if (*reinterpret_cast<int*>(self + 4600) != -1) Stop__13VISoundDevicei(sound, *reinterpret_cast<int*>(self + 4600));
        ReleaseSound__13VISoundDeviceiR12VIDictionary(sound, *reinterpret_cast<int*>(self + 4592), dictionary);
    }
    if (system != 0) {
        for (int i = 0; i < *reinterpret_cast<int*>(self + 4616); ++i) {
            unsigned char* entries = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 4620));
            if (*reinterpret_cast<int*>(entries + 1056 * i) != -1) {
                Destroy__17VIParticleEmitter(entries + 1056 * i + 16);
                entries = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 4620));
                ReleaseParticleDefinition__16VIParticleSystemiR12VIDictionary(system, *reinterpret_cast<int*>(entries + 1056 * i), dictionary);
            }
        }
    }
    return 0;
}
}
