// Copy__9VIHSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei at 0x01061320, 888 bytes.
// Observed: it creates a sprite with CreateSprite__7VIScene12VISpriteTypei(scene, word at S+52, i) and returns -1 when the result is negative. Otherwise it fetches the new
// sprite N with Sprite__7VIScenei(scene, id) and initializes six embedded containers from the source's, in this order, each by a call (N + k, S + k) that returns -1 from the
// whole function when its result is negative: k = 64 (Init__t8VIVector1Z13VIHSpriteNode...), 76 (Init__t8VIVector1Z10VIMatrix44...), 88 (Init__t7VIArray1Z16VIHSpriteTrigger...),
// 96 (Init__t6VIPool1Z19VIHSpriteAttachment...), 120 (Init__t6VIPool1Z13VIHSpritePlay...) and 144 (Init__t6VIPool1Z17VIHSpritePlayNode...). Then it copies the common
// 44 bytes S+8..S+51 (eleven floats) and the word at S+52, and these fields: the 16 bytes S+168..S+183, the float S+208, floats at S+212..S+223 and S+224..S+235, the 20 bytes
// S+236..S+255, the floats S+256..S+267, the 20 bytes S+268..S+287, the words S+384 and S+388, and the floats S+288..S+299. The pools at S+96 (16-byte entries) and
// S+120 (104-byte entries) of the SOURCE are then walked: ShareSprite__7VIScenei(scene, first word of each attachment entry) and ShareAnimation__7VIScenei(scene, first word of
// each play entry), returning -1 on a negative result. Last it calls ShareAnimation__7VIScenei(scene, word at S+388) (even when that word is -1) and returns -1 if negative, then
// ShareRefMap__7VIScenei(scene, word at S+384), returning -1 if negative, else the id. The raster, sound device and collide arguments are not used.
// Inferred: the containers are a node vector, a matrix vector, a trigger array and three pools that the new sprite owns copies of; the shares take references on the
// shared scene resources. Unknown: the names and types of the copied fields; bytes not listed above (for example S+300..S+383) are not copied.
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
int ShareSprite__7VIScenei(unsigned char* scene, int sprite);
int ShareAnimation__7VIScenei(unsigned char* scene, int animation);
int ShareRefMap__7VIScenei(unsigned char* scene, int refMap);

static void copy_base(unsigned char* to, const unsigned char* from) {
    float* d = reinterpret_cast<float*>(to + 8);
    const float* s = reinterpret_cast<const float*>(from + 8);
    d[0] = s[0]; d[1] = s[1]; d[2] = s[2]; d[3] = s[3]; d[4] = s[4]; d[5] = s[5];
    d[6] = s[6]; d[7] = s[7]; d[8] = s[8]; d[9] = s[9]; d[10] = s[10];
    *reinterpret_cast<int*>(to + 52) = *reinterpret_cast<const int*>(from + 52);
}

static void copy_floats(unsigned char* to, const unsigned char* from, int offset, int count) {
    for (int i = 0; i < count; ++i) reinterpret_cast<float*>(to + offset)[i] = reinterpret_cast<const float*>(from + offset)[i];
}

int Copy__9VIHSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei(unsigned char* self, unsigned char* scene, unsigned char*, unsigned char*, unsigned char*, int copyArgument) {
    int id = CreateSprite__7VIScene12VISpriteTypei(scene, *reinterpret_cast<int*>(self + 52), copyArgument);
    if (id < 0) return -1;
    unsigned char* copy = NORRATH_WORD_TO_POINTER(Sprite__7VIScenei(scene, id));
    if (Init__t8VIVector1Z13VIHSpriteNodeRCt8VIVector1Z13VIHSpriteNode(copy + 64, self + 64) < 0) return -1;
    if (Init__t8VIVector1Z10VIMatrix44RCt8VIVector1Z10VIMatrix44(copy + 76, self + 76) < 0) return -1;
    if (Init__t7VIArray1Z16VIHSpriteTriggerRCt7VIArray1Z16VIHSpriteTrigger(copy + 88, self + 88) < 0) return -1;
    if (Init__t6VIPool1Z19VIHSpriteAttachmentRCt6VIPool1Z19VIHSpriteAttachment(copy + 96, self + 96) < 0) return -1;
    if (Init__t6VIPool1Z13VIHSpritePlayRCt6VIPool1Z13VIHSpritePlay(copy + 120, self + 120) < 0) return -1;
    if (Init__t6VIPool1Z17VIHSpritePlayNodeRCt6VIPool1Z17VIHSpritePlayNode(copy + 144, self + 144) < 0) return -1;
    typedef long long Unaligned __attribute__((aligned(1)));
    copy_base(copy, self);
    reinterpret_cast<Unaligned*>(copy + 168)[0] = reinterpret_cast<const Unaligned*>(self + 168)[0];
    reinterpret_cast<Unaligned*>(copy + 168)[1] = reinterpret_cast<const Unaligned*>(self + 168)[1];
    copy_floats(copy, self, 208, 1);
    copy_floats(copy, self, 212, 3);
    copy_floats(copy, self, 224, 3);
    reinterpret_cast<Unaligned*>(copy + 236)[0] = reinterpret_cast<const Unaligned*>(self + 236)[0];
    reinterpret_cast<Unaligned*>(copy + 236)[1] = reinterpret_cast<const Unaligned*>(self + 236)[1];
    copy_floats(copy, self, 252, 1);
    copy_floats(copy, self, 256, 3);
    reinterpret_cast<Unaligned*>(copy + 268)[0] = reinterpret_cast<const Unaligned*>(self + 268)[0];
    reinterpret_cast<Unaligned*>(copy + 268)[1] = reinterpret_cast<const Unaligned*>(self + 268)[1];
    copy_floats(copy, self, 284, 1);
    *reinterpret_cast<int*>(copy + 384) = *reinterpret_cast<int*>(self + 384);
    *reinterpret_cast<int*>(copy + 388) = *reinterpret_cast<int*>(self + 388);
    copy_floats(copy, self, 288, 3);
    const unsigned char* attachments = self + 96;
    for (int i = Begin__Ct6VIPool1Z19VIHSpriteAttachment(attachments); i != End__Ct6VIPool1Z19VIHSpriteAttachment(attachments); i = Next__Ct6VIPool1Z19VIHSpriteAttachmenti(attachments, i))
        if (ShareSprite__7VIScenei(scene, *reinterpret_cast<int*>(NORRATH_WORD_TO_POINTER(*reinterpret_cast<const unsigned*>(attachments + 8)) + 16 * i)) < 0) return -1;
    const unsigned char* plays = self + 120;
    for (int i = Begin__Ct6VIPool1Z13VIHSpritePlay(plays); i != End__Ct6VIPool1Z13VIHSpritePlay(plays); i = Next__Ct6VIPool1Z13VIHSpritePlayi(plays, i))
        if (ShareAnimation__7VIScenei(scene, *reinterpret_cast<int*>(NORRATH_WORD_TO_POINTER(*reinterpret_cast<const unsigned*>(plays + 8)) + 104 * i)) < 0) return -1;
    if (ShareAnimation__7VIScenei(scene, *reinterpret_cast<int*>(self + 388)) < 0) return -1;
    return ShareRefMap__7VIScenei(scene, *reinterpret_cast<int*>(self + 384)) < 0 ? -1 : id;
}
}
