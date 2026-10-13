// Copy__9VICSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei at 0x010b0b80, 2076 bytes.
// Observed (first stage): it creates a sprite with CreateSprite__7VIScene12VISpriteTypei(scene, word at S+52, i) and returns -1 when the result is negative. Otherwise it fetches the new
// sprite N with Sprite__7VIScenei(scene, id), then runs the same six container initializers as Copy__9VIHSprite in the same order (offsets 64, 76, 88, 96, 120, 144; any negative result
// returns -1). Then it copies these parts of S to N: the 44 bytes S+8..S+51 and the word at S+52; the words at S+400, S+384, S+388, S+404, S+408, S+412, S+420, S+3856; the float at S+208;
// the bytes S+168..S+183, S+424..S+3855, S+3860..S+3895, S+3896..S+4351, S+4352..S+4363, S+4364..S+4399, S+4400..S+4423, S+4424..S+4455, S+4460..S+4523 and S+4592..S+4603. The word at
// N+416 is first set to -1; the bytes S+4456..S+4459, S+4524..S+4591 and the rest are not copied. If the raster is nonnull, N+416 is then set to CopyMaterialPal__8VIRasterii(raster, word
// at S+416, i), which is stored even when negative and returns -1 when negative.
// Observed (second stage): the pool at N+96 (16-byte entries) is walked with the Begin/Next/End routines of VIHSpriteAttachment. For each entry, id = its first word and the type is
// GetSpriteType__7VIScenei(scene, id), called afresh for each comparison: it is compared with 6, then with 3, then (if neither) with 7. If the type is 6 or 3: copy = CopySprite__7VISceneii(scene, id, 0) (a negative result returns -1); if the entry's type is read again and is 3 and the raster is
// nonnull, SetMaterialPal__12VISkinSpriteiR8VIRasterR12VIDictionary(Sprite(scene, copy), word at N+416, raster, the pointer word at scene+19352) must not be negative; the entry's first word becomes copy.
// If the type is 7 (a level-of-detail sprite): copy = CopySprite(scene, id, 0) (negative returns -1); lod = Sprite(scene, copy); for j from 0 while j is below the word at lod+68 (reread each
// time) and with child = the word at lod+76+8*j: if GetSpriteType(scene, child) is 3, child2 = CopySprite(scene, child, 0) (negative returns -1), then, if the raster is nonnull, SetMaterialPal on
// Sprite(scene, child2) as above (negative returns -1), then SetLevelSprite__11VILODSpriteiiR7VIScene(lod, j, child2, scene) (negative returns -1); finally the entry's first word becomes copy.
// Other types leave the entry alone.
// Observed (third stage): Init__t7VIArray1Z20VICSpritePartEmitteri(N+4616, word at S+4616) (negative returns -1); then for i below the word at S+4616 the eight words at offsets 0, 1008, 1012, 1016,
// 1020, 1024, 1028 and 1032 of the 1056-byte entry i are copied from the array whose pointer is the word at S+4620 to the one at N+4620; CreateParticleEmitters__9VICSpriteR7VIScene(N, scene)
// must not be negative.
// Observed (fourth stage): the pool at N+96 is walked again calling ShareSprite__7VIScenei(scene, first word of each entry); the pool at S+120 (104-byte entries) is walked calling
// ShareAnimation__7VIScenei(scene, first word of each entry); a negative result of either returns -1. Then ShareSprite is called (results unused) for each of the three words at S+4372, S+4384, S+4396
// that is not -1. Last, Share__9VICSpriteR7VISceneP8VIRaster(N, scene, raster) is called; the function returns -1 if that is negative and the id otherwise. The sound device and collide arguments are not used.
// Inferred: sprite types 3, 6 and 7 are the skin, an unknown kind, and the level-of-detail kind (matching the constructors in sibling units, which store 3 for skin sprites and 7 for LOD sprites);
// the dictionary at scene+19352 is the resource dictionary. Unknown: every field name, the meaning of type 6, and the layout inside the copied blocks.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int CreateSprite__7VIScene12VISpriteTypei(unsigned char* scene, int type, int copyArgument);
unsigned Sprite__7VIScenei(unsigned char* scene, int index);
int Init__t8VIVector1Z13VIHSpriteNodeRCt8VIVector1Z13VIHSpriteNode(unsigned char* destination, const unsigned char* source);
int Init__t8VIVector1Z10VIMatrix44RCt8VIVector1Z10VIMatrix44(unsigned char* destination, const unsigned char* source);
int Init__t7VIArray1Z16VIHSpriteTriggerRCt7VIArray1Z16VIHSpriteTrigger(unsigned char* destination, const unsigned char* source);
int Init__t6VIPool1Z19VIHSpriteAttachmentRCt6VIPool1Z19VIHSpriteAttachment(unsigned char* destination, const unsigned char* source);
int Init__t6VIPool1Z13VIHSpritePlayRCt6VIPool1Z13VIHSpritePlay(unsigned char* destination, const unsigned char* source);
int Init__t6VIPool1Z17VIHSpritePlayNodeRCt6VIPool1Z17VIHSpritePlayNode(unsigned char* destination, const unsigned char* source);
int Begin__Ct6VIPool1Z19VIHSpriteAttachment(const unsigned char* pool);
int Next__Ct6VIPool1Z19VIHSpriteAttachmenti(const unsigned char* pool, int index);
int End__Ct6VIPool1Z19VIHSpriteAttachment(const unsigned char* pool);
int Begin__Ct6VIPool1Z13VIHSpritePlay(const unsigned char* pool);
int Next__Ct6VIPool1Z13VIHSpritePlayi(const unsigned char* pool, int index);
int End__Ct6VIPool1Z13VIHSpritePlay(const unsigned char* pool);
int Init__t7VIArray1Z20VICSpritePartEmitteri(unsigned char* array, int count);
int CopyMaterialPal__8VIRasterii(unsigned char* raster, int palette, int copyArgument);
int GetSpriteType__7VIScenei(unsigned char* scene, int index);
int CopySprite__7VISceneii(unsigned char* scene, int index, int copyArgument);
int SetMaterialPal__12VISkinSpriteiR8VIRasterR12VIDictionary(unsigned char* sprite, int palette, unsigned char* raster, unsigned char* dictionary);
int SetLevelSprite__11VILODSpriteiiR7VIScene(unsigned char* lod, int level, int sprite, unsigned char* scene);
int CreateParticleEmitters__9VICSpriteR7VIScene(unsigned char* sprite, unsigned char* scene);
int ShareSprite__7VIScenei(unsigned char* scene, int sprite);
int ShareAnimation__7VIScenei(unsigned char* scene, int animation);
int Share__9VICSpriteR7VISceneP8VIRaster(unsigned char* sprite, unsigned char* scene, unsigned char* raster);

typedef long long Unaligned __attribute__((aligned(1)));

static void copy_base(unsigned char* to, const unsigned char* from) {
    float* d = reinterpret_cast<float*>(to + 8);
    const float* s = reinterpret_cast<const float*>(from + 8);
    d[0] = s[0]; d[1] = s[1]; d[2] = s[2]; d[3] = s[3]; d[4] = s[4]; d[5] = s[5];
    d[6] = s[6]; d[7] = s[7]; d[8] = s[8]; d[9] = s[9]; d[10] = s[10];
    *reinterpret_cast<int*>(to + 52) = *reinterpret_cast<const int*>(from + 52);
}
static void copy_word(unsigned char* to, const unsigned char* from, int offset) {
    *reinterpret_cast<int*>(to + offset) = *reinterpret_cast<const int*>(from + offset);
}
static void copy_floats(unsigned char* to, const unsigned char* from, int offset, int count) {
    for (int i = 0; i < count; ++i) reinterpret_cast<float*>(to + offset)[i] = reinterpret_cast<const float*>(from + offset)[i];
}
static void copy_doublewords(unsigned char* to, const unsigned char* from, int offset, int count) {
    for (int i = 0; i < count; ++i) reinterpret_cast<Unaligned*>(to + offset)[i] = reinterpret_cast<const Unaligned*>(from + offset)[i];
}

int Copy__9VICSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei(unsigned char* self, unsigned char* scene, unsigned char* raster, unsigned char*, unsigned char*, int copyArgument) {
    int id = CreateSprite__7VIScene12VISpriteTypei(scene, *reinterpret_cast<int*>(self + 52), copyArgument);
    if (id < 0) return -1;
    unsigned char* copy = NORRATH_WORD_TO_POINTER(Sprite__7VIScenei(scene, id));
    if (Init__t8VIVector1Z13VIHSpriteNodeRCt8VIVector1Z13VIHSpriteNode(copy + 64, self + 64) < 0) return -1;
    if (Init__t8VIVector1Z10VIMatrix44RCt8VIVector1Z10VIMatrix44(copy + 76, self + 76) < 0) return -1;
    if (Init__t7VIArray1Z16VIHSpriteTriggerRCt7VIArray1Z16VIHSpriteTrigger(copy + 88, self + 88) < 0) return -1;
    if (Init__t6VIPool1Z19VIHSpriteAttachmentRCt6VIPool1Z19VIHSpriteAttachment(copy + 96, self + 96) < 0) return -1;
    if (Init__t6VIPool1Z13VIHSpritePlayRCt6VIPool1Z13VIHSpritePlay(copy + 120, self + 120) < 0) return -1;
    if (Init__t6VIPool1Z17VIHSpritePlayNodeRCt6VIPool1Z17VIHSpritePlayNode(copy + 144, self + 144) < 0) return -1;
    copy_base(copy, self);
    copy_word(copy, self, 400);
    copy_doublewords(copy, self, 168, 2);
    copy_word(copy, self, 384);
    copy_floats(copy, self, 208, 1);
    copy_word(copy, self, 388);
    copy_floats(copy, self, 4424, 3);
    *reinterpret_cast<int*>(copy + 416) = -1;
    copy_word(copy, self, 404);
    copy_word(copy, self, 412);
    copy_word(copy, self, 408);
    copy_word(copy, self, 420);
    copy_floats(copy, self, 4436, 3);
    copy_floats(copy, self, 4448, 2);
    copy_doublewords(copy, self, 424, 429);
    copy_word(copy, self, 3856);
    copy_doublewords(copy, self, 3860, 4);
    copy_word(copy, self, 3892);
    copy_doublewords(copy, self, 3896, 57);
    copy_doublewords(copy, self, 4352, 1);
    copy_word(copy, self, 4360);
    copy_doublewords(copy, self, 4364, 4);
    copy_word(copy, self, 4396);
    copy_doublewords(copy, self, 4400, 3);
    copy_doublewords(copy, self, 4592, 1);
    copy_word(copy, self, 4600);
    copy_doublewords(copy, self, 4492, 4);
    copy_doublewords(copy, self, 4460, 4);
    if (raster != 0) {
        int palette = CopyMaterialPal__8VIRasterii(raster, *reinterpret_cast<int*>(self + 416), copyArgument);
        *reinterpret_cast<int*>(copy + 416) = palette;
        if (palette < 0) return -1;
    }
    unsigned char* attachments = copy + 96;
    for (int i = Begin__Ct6VIPool1Z19VIHSpriteAttachment(attachments); i != End__Ct6VIPool1Z19VIHSpriteAttachment(attachments); i = Next__Ct6VIPool1Z19VIHSpriteAttachmenti(attachments, i)) {
        unsigned char* entry = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(attachments + 8)) + 16 * i;
        if (GetSpriteType__7VIScenei(scene, *reinterpret_cast<int*>(entry)) == 6 || GetSpriteType__7VIScenei(scene, *reinterpret_cast<int*>(entry)) == 3) {
            int duplicate = CopySprite__7VISceneii(scene, *reinterpret_cast<int*>(entry), 0);
            if (duplicate < 0) return -1;
            entry = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(attachments + 8)) + 16 * i;
            if (GetSpriteType__7VIScenei(scene, *reinterpret_cast<int*>(entry)) == 3 && raster != 0) {
                if (SetMaterialPal__12VISkinSpriteiR8VIRasterR12VIDictionary(NORRATH_WORD_TO_POINTER(Sprite__7VIScenei(scene, duplicate)), *reinterpret_cast<int*>(copy + 416), raster,
                                                                          NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(scene + 19352))) < 0) return -1;
            }
            entry = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(attachments + 8)) + 16 * i;
            *reinterpret_cast<int*>(entry) = duplicate;
        } else if (GetSpriteType__7VIScenei(scene, *reinterpret_cast<int*>(entry)) == 7) {
            int duplicate = CopySprite__7VISceneii(scene, *reinterpret_cast<int*>(entry), 0);
            if (duplicate < 0) return -1;
            unsigned char* lod = NORRATH_WORD_TO_POINTER(Sprite__7VIScenei(scene, duplicate));
            for (int j = 0; j < *reinterpret_cast<int*>(lod + 68); ++j) {
                int child = *reinterpret_cast<int*>(lod + 76 + 8 * j);
                if (GetSpriteType__7VIScenei(scene, child) == 3) {
                    int childCopy = CopySprite__7VISceneii(scene, child, 0);
                    if (childCopy < 0) return -1;
                    if (raster != 0 && SetMaterialPal__12VISkinSpriteiR8VIRasterR12VIDictionary(NORRATH_WORD_TO_POINTER(Sprite__7VIScenei(scene, childCopy)), *reinterpret_cast<int*>(copy + 416), raster,
                                                                                            NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(scene + 19352))) < 0) return -1;
                    if (SetLevelSprite__11VILODSpriteiiR7VIScene(lod, j, childCopy, scene) < 0) return -1;
                }
            }
            entry = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(attachments + 8)) + 16 * i;
            *reinterpret_cast<int*>(entry) = duplicate;
        }
    }
    if (Init__t7VIArray1Z20VICSpritePartEmitteri(copy + 4616, *reinterpret_cast<int*>(self + 4616)) < 0) return -1;
    for (int i = 0; i < *reinterpret_cast<int*>(self + 4616); ++i) {
        const unsigned char* from = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 4620)) + 1056 * i;
        unsigned char* to = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(copy + 4620)) + 1056 * i;
        copy_word(to, from, 0);
        copy_word(to, from, 1020);
        copy_word(to, from, 1012);
        copy_word(to, from, 1008);
        copy_word(to, from, 1024);
        copy_word(to, from, 1016);
        copy_word(to, from, 1028);
        copy_word(to, from, 1032);
    }
    if (CreateParticleEmitters__9VICSpriteR7VIScene(copy, scene) < 0) return -1;
    for (int i = Begin__Ct6VIPool1Z19VIHSpriteAttachment(attachments); i != End__Ct6VIPool1Z19VIHSpriteAttachment(attachments); i = Next__Ct6VIPool1Z19VIHSpriteAttachmenti(attachments, i))
        if (ShareSprite__7VIScenei(scene, *reinterpret_cast<int*>(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(attachments + 8)) + 16 * i)) < 0) return -1;
    const unsigned char* plays = self + 120;
    for (int i = Begin__Ct6VIPool1Z13VIHSpritePlay(plays); i != End__Ct6VIPool1Z13VIHSpritePlay(plays); i = Next__Ct6VIPool1Z13VIHSpritePlayi(plays, i))
        if (ShareAnimation__7VIScenei(scene, *reinterpret_cast<int*>(NORRATH_WORD_TO_POINTER(*reinterpret_cast<const unsigned*>(plays + 8)) + 104 * i)) < 0) return -1;
    for (int k = 0; k < 3; ++k) {
        int part = *reinterpret_cast<int*>(self + 4372 + 12 * k);
        if (part != -1) ShareSprite__7VIScenei(scene, part);
    }
    return Share__9VICSpriteR7VISceneP8VIRaster(copy, scene, raster) < 0 ? -1 : id;
}
}
