// Release__9VIHSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide at 0x01061698, 332 bytes.
// Observed: if the word at S+0 is nonzero it calls Remove__12VIDictionaryR10VIResource(dictionary, S) and returns -1 when the result is negative. Then it walks the pool at
// S+96 (entries of 16 bytes: an index from Begin__Ct6VIPool1Z19VIHSpriteAttachment, advanced by Next until it equals End) calling ReleaseSprite__7VIScenei(scene, the first word of
// each entry), and walks the pool at S+120 (entries of 104 bytes, VIHSpritePlay) calling ReleaseAnimation__7VIScenei(scene, the first word of each entry); a negative result from any of
// these calls returns -1 at once. Then, if the word at S+388 is not -1, it calls ReleaseAnimation__7VIScenei(scene, that word) and returns -1 on a negative result. Last it calls
// ReleaseRefMap__7VIScenei(scene, word at S+384) and returns -1 if that result is negative, otherwise 0. The raster, sound device and collide arguments are not used.
// Inferred: the pools hold the sprite's attachments and plays; the words at S+384 and S+388 are a reference-map id and an animation id. Unknown: their names and the rest of each entry.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int Remove__12VIDictionaryR10VIResource(unsigned char* dictionary, unsigned char* resource);
int Begin__Ct6VIPool1Z19VIHSpriteAttachment(const unsigned char* pool);
int Next__Ct6VIPool1Z19VIHSpriteAttachmenti(const unsigned char* pool, int index);
int End__Ct6VIPool1Z19VIHSpriteAttachment(const unsigned char* pool);
int Begin__Ct6VIPool1Z13VIHSpritePlay(const unsigned char* pool);
int Next__Ct6VIPool1Z13VIHSpritePlayi(const unsigned char* pool, int index);
int End__Ct6VIPool1Z13VIHSpritePlay(const unsigned char* pool);
int ReleaseSprite__7VIScenei(unsigned char* scene, int sprite);
int ReleaseAnimation__7VIScenei(unsigned char* scene, int animation);
int ReleaseRefMap__7VIScenei(unsigned char* scene, int refMap);

int Release__9VIHSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide(unsigned char* self, unsigned char* scene, unsigned char* dictionary, unsigned char*, unsigned char*, unsigned char*) {
    if (*reinterpret_cast<int*>(self) != 0 && Remove__12VIDictionaryR10VIResource(dictionary, self) < 0) return -1;
    const unsigned char* attachments = self + 96;
    for (int i = Begin__Ct6VIPool1Z19VIHSpriteAttachment(attachments); i != End__Ct6VIPool1Z19VIHSpriteAttachment(attachments); i = Next__Ct6VIPool1Z19VIHSpriteAttachmenti(attachments, i))
        if (ReleaseSprite__7VIScenei(scene, *reinterpret_cast<int*>(NORRATH_WORD_TO_POINTER(*reinterpret_cast<const unsigned*>(attachments + 8)) + 16 * i)) < 0) return -1;
    const unsigned char* plays = self + 120;
    for (int i = Begin__Ct6VIPool1Z13VIHSpritePlay(plays); i != End__Ct6VIPool1Z13VIHSpritePlay(plays); i = Next__Ct6VIPool1Z13VIHSpritePlayi(plays, i))
        if (ReleaseAnimation__7VIScenei(scene, *reinterpret_cast<int*>(NORRATH_WORD_TO_POINTER(*reinterpret_cast<const unsigned*>(plays + 8)) + 104 * i)) < 0) return -1;
    if (*reinterpret_cast<int*>(self + 388) != -1 && ReleaseAnimation__7VIScenei(scene, *reinterpret_cast<int*>(self + 388)) < 0) return -1;
    return ReleaseRefMap__7VIScenei(scene, *reinterpret_cast<int*>(self + 384)) < 0 ? -1 : 0;
}
}
