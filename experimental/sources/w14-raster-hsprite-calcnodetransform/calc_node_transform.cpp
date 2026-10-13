// Observed: CalcNodeTransform__9VIHSpriteiR10VIMatrix44R7VISceneT2 at 0x0105fca0 is 432 bytes in the pinned SLUS-20565 image.
// Arguments: the sprite (this), a node index, a parent matrix, the scene and an output matrix. Two local 4x4 matrices A and B are
// zero-initialised (B is never used and is only constructed and destroyed). The routine:
//   1. calls CalcTransAboutCenter__9VIHSpriteRC10VIMatrix44R7VISceneR10VIMatrix44(sprite, parent, scene, &A);
//   2. calls GetTime__7VIScene(scene) and compares the full 64-bit value with the 64-bit word at sprite+200;
//      if they differ it does:
//        if Size__Ct6VIPool1Z13VIHSpritePlay(sprite + 120) > 0:
//            level = CalcLODLevelIndex__9VIHSpriteRC10VIMatrix44(sprite, parent);
//            status = ProcessPlayList__9VIHSpriteiR7VIScene(sprite, level, scene);
//            if status >= 0: status = ProcessUpHierarchy__9VIHSpritei(sprite, index);
//        else: status = SetBindPose__9VIHSprite(sprite);
//        if status < 0: destroy B then A (flag 2 each) and return -1;
//   3. calls Mul__10VIMatrix44RC10VIMatrix44T1(out, M, &A) where M is the matrix at (word at sprite+72) + index*256 + 48;
//   4. destroys B then A (_$_10VIMatrix44 with flag 2) and returns 0.
// When the time equals the stored value, steps 2's work is skipped and the routine goes straight to the product.
// Inferred: computes a node's world transform: the sprite's own transform about its centre multiplied into the stored bind matrix of
// the node (256-byte node records, matrix at +48); the time stamp avoids reprocessing the play list twice in a frame.
// Unknown: the 64-bit time field's writer (none of the callees here is known to update it), the meaning of the pool size, and the
// callees' bodies. Nothing here ran on the EE.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
void CalcTransAboutCenter__9VIHSpriteRC10VIMatrix44R7VISceneR10VIMatrix44(unsigned char* sprite, const float* parent, unsigned char* scene, float* out);
int GetTime__7VIScene(unsigned char* scene);
int Size__Ct6VIPool1Z13VIHSpritePlay(const unsigned char* pool);
int CalcLODLevelIndex__9VIHSpriteRC10VIMatrix44(unsigned char* sprite, const float* parent);
int ProcessPlayList__9VIHSpriteiR7VIScene(unsigned char* sprite, int level, unsigned char* scene);
int ProcessUpHierarchy__9VIHSpritei(unsigned char* sprite, int index);
int SetBindPose__9VIHSprite(unsigned char* sprite);
void Mul__10VIMatrix44RC10VIMatrix44T1(float* out, const float* a, const float* b);
void _$_10VIMatrix44(float* matrix, int flags);

int CalcNodeTransform__9VIHSpriteiR10VIMatrix44R7VISceneT2(unsigned char* sprite, int index, const float* parent, unsigned char* scene, float* out) {
    alignas(16) float a[16] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    alignas(16) float b[16] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    CalcTransAboutCenter__9VIHSpriteRC10VIMatrix44R7VISceneR10VIMatrix44(sprite, parent, scene, a);
    long long time = GetTime__7VIScene(scene);
    if (time != *reinterpret_cast<long long*>(sprite + 200)) {
        int status;
        if (Size__Ct6VIPool1Z13VIHSpritePlay(sprite + 120) > 0) {
            int level = CalcLODLevelIndex__9VIHSpriteRC10VIMatrix44(sprite, parent);
            status = ProcessPlayList__9VIHSpriteiR7VIScene(sprite, level, scene);
            if (status >= 0) status = ProcessUpHierarchy__9VIHSpritei(sprite, index);
        } else {
            status = SetBindPose__9VIHSprite(sprite);
        }
        if (status < 0) {
            _$_10VIMatrix44(b, 2);
            _$_10VIMatrix44(a, 2);
            return -1;
        }
    }
    const unsigned char* nodes = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(sprite + 72));
    Mul__10VIMatrix44RC10VIMatrix44T1(out, reinterpret_cast<const float*>(nodes + index * 256 + 48), a);
    _$_10VIMatrix44(b, 2);
    _$_10VIMatrix44(a, 2);
    return 0;
}
}
