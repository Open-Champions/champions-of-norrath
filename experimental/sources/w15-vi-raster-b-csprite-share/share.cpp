// Observed: Share__9VICSpriteR7VISceneP8VIRaster at 0x010b1e60 is 620 bytes in the pinned SLUS-20565 image. Arguments: the sprite
// (this), a scene and a raster pointer (may be null). It increments or registers the resources the sprite uses and returns -1 at the
// first negative result of the checked callees, otherwise 0. In order:
//  1. For 66 records of 52 bytes (record i starts at sprite offset 424 + 52 * i): the two ints at +0 and +4 that are not negative are
//     given to ShareAnimation__7VIScenei(scene, id) (negative result -> -1); then, only when the scene word at +19360 (0x4ba0) is
//     non-null, the two ints at +24 and +28 of the record (sprite offsets 448 + 52 * i, +4) that are not negative are given to
//     ShareSound__13VISoundDevicei(sound device word, id) (negative result -> -1).
//  2. The sprite word at +3856 (0xf10), unless -1, goes to ShareSprite__7VIScenei(scene, id) (negative -> -1).
//  3. If the raster is non-null: ShareMaterialPal__8VIRasteri(raster, sprite word at +416) (negative -> -1) and
//     ShareRefMap__7VIScenei(scene, sprite word at +420) (negative -> -1).
//  4. The sprite word at +388, unless -1, goes to ShareAnimation (negative -> -1); then ShareRefMap__7VIScenei(scene, sprite word at
//     +384) (negative -> -1; this one is always called).
//  5. If the raster is non-null, the 19 records of 24 bytes whose third word is at sprite offsets 3908 + 24 * k (the text-slot records
//     of InitTextSlots) with a word other than -1 go to ShareSurface__8VIRasteri(raster, id); the results are ignored.
//  6. If the sound device is non-null and the sprite word at +4592 is not -1, ShareSound(device, id) (result ignored).
//  7. If the address scene + 1352 (kept as a 32-bit word, so the test is a real one) is non-zero: the array at sprite +4616 (count at +0,
//     data pointer at +4, entries of 1056 bytes) has its first word read for each entry, in order, with the count re-read each time;
//     any value other than -1 goes to ShareParticleDefinition__16VIParticleSystemi(scene + 1352, id) (result ignored).
// Inferred: the +424 records hold two animation ids and two sound ids each, and the call sequence is a reference-count pass over
// everything the sprite owns before it is shared between scenes; the first word of each 1056-byte entry is a particle definition.
// Unknown: the layouts of the sprite and scene, what the other words of each record hold, the callees' behaviour (imports of this
// unit; ShareSurface is authored in w14-raster-raster-sharesurface and ShareSurface's argument is the surface index). Nothing here
// ran on the EE.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
extern "C" unsigned norrath_host_address(const unsigned char* pointer);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#define NORRATH_POINTER_TO_WORD(pointer) norrath_host_address(pointer)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#define NORRATH_POINTER_TO_WORD(pointer) static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer))
#endif
extern "C" {
int ShareAnimation__7VIScenei(unsigned char* scene, int id);
int ShareSound__13VISoundDevicei(unsigned char* device, int id);
int ShareSprite__7VIScenei(unsigned char* scene, int id);
int ShareMaterialPal__8VIRasteri(unsigned char* raster, int id);
int ShareRefMap__7VIScenei(unsigned char* scene, int id);
int ShareSurface__8VIRasteri(unsigned char* raster, int id);
int ShareParticleDefinition__16VIParticleSystemi(unsigned char* system, int id);
int Share__9VICSpriteR7VISceneP8VIRaster(unsigned char* self, unsigned char* scene, unsigned char* raster) {
    unsigned particles = NORRATH_POINTER_TO_WORD(scene) + 1352;
    unsigned sound = *reinterpret_cast<unsigned*>(scene + 19360);
    for (int i = 0; i < 66; ++i) {
        const int* animations = reinterpret_cast<const int*>(self + 424 + 52 * i);
        for (int k = 0; k < 2; ++k) {
            int id = animations[k];
            if (id >= 0) {
                if (ShareAnimation__7VIScenei(scene, id) < 0) return -1;
            }
        }
        const int* sounds = reinterpret_cast<const int*>(self + 448 + 52 * i);
        for (int k = 0; k < 2; ++k) {
            if (sound != 0) {
                int id = sounds[k];
                if (id >= 0) {
                    if (ShareSound__13VISoundDevicei(NORRATH_WORD_TO_POINTER(sound), id) < 0) return -1;
                }
            }
        }
    }
    int spriteId = *reinterpret_cast<int*>(self + 3856);
    if (spriteId != -1) {
        if (ShareSprite__7VIScenei(scene, spriteId) < 0) return -1;
    }
    if (raster != 0) {
        if (ShareMaterialPal__8VIRasteri(raster, *reinterpret_cast<int*>(self + 416)) < 0) return -1;
        if (ShareRefMap__7VIScenei(scene, *reinterpret_cast<int*>(self + 420)) < 0) return -1;
    }
    int animation = *reinterpret_cast<int*>(self + 388);
    if (animation != -1) {
        if (ShareAnimation__7VIScenei(scene, animation) < 0) return -1;
    }
    if (ShareRefMap__7VIScenei(scene, *reinterpret_cast<int*>(self + 384)) < 0) return -1;
    if (raster != 0) {
        for (int k = 0; k < 19; ++k) {
            int id = *reinterpret_cast<int*>(self + 3908 + 24 * k);
            if (id != -1) ShareSurface__8VIRasteri(raster, id);
        }
    }
    if (sound != 0) {
        int id = *reinterpret_cast<int*>(self + 4592);
        if (id != -1) ShareSound__13VISoundDevicei(NORRATH_WORD_TO_POINTER(sound), id);
    }
    if (particles != 0) {
        unsigned char* array = self + 4616;
        if (*reinterpret_cast<int*>(array) > 0) {
            int i = 0;
            do {
                unsigned char* entries = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(array + 4));
                int id = *reinterpret_cast<int*>(entries + i * 1056);
                i++;
                if (id != -1) ShareParticleDefinition__16VIParticleSystemi(NORRATH_WORD_TO_POINTER(particles), id);
            } while (i < *reinterpret_cast<int*>(array));
        }
    }
    return 0;
}
}
