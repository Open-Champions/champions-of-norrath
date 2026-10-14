// Observed: Pick__9VIHSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster at 0x01060c30, 712 bytes. Arguments: the sprite (S), the pick segment (G), the scene filter (F), the scene (V) and
// the raster (R). Two 64-byte matrices M0 and M1 are zero-filled and always destroyed on return (_$_10VIMatrix44 with flag 2: M1,
// then M0). The hit flag starts at 0.
//  1. CalcLODLevelIndex__9VIHSpriteR13VIPickSegmentR8VIRaster(S, G, R) gives a level L; Process__9VIHSpriteiR7VIScene(S, L, V) runs. A
//     negative result returns -1 after the destructors, with nothing else done.
//  2. CalcTransAboutCenter__9VIHSpriteRC10VIMatrix44R7VISceneR10VIMatrix44(S, GetModelView__C8VIRaster(R), V, M0), then
//     SetBlendMatrices__9VIHSpriteR8VIRaster(S, R); a negative result returns -1 after the destructors.
//  3. The pool at S+96 is walked with Begin__Ct6VIPool1Z19VIHSpriteAttachment, Next__...i and End__... (the end value is fetched
//     again after every step). For index k the 16-byte record is at (word at S+104) + 16*k: word +0 is a sprite index and word +4 a
//     transform index. If the transform index is -1, SetModelView__8VIRasterRC10VIMatrix44(R, M0) runs; otherwise
//     Mul__10VIMatrix44RC10VIMatrix44T1(M1, (word at S+72) + 256*transform + 48, M0) and SetModelView(R, M1) run. Then
//     Sprite__7VIScenei(V, record word +0) gives sprite P (the record pointer is read again after the model-view call); its word at +60
//     is a table, and the 8-byte entry at table+48 holds a signed halfword adjustment and a function word at +4. That function is called
//     with (P + adjustment, G, F, V, R).
//  4. A nonzero result sets the hit flag to 1 and stores k at F+12; the walk continues, so the last hit wins.
//  5. The result is the hit flag. The raster's model-view is left at the last matrix set.
// Inferred: S+72 holds an array of 256-byte transform records (matrix at +48) and the pool lists attachments; the slot is the pick virtual;
// a record without a transform is placed by the sprite's own matrix M0. Same walk as the HSprite collide virtuals, which skip such
// records. Unknown: names of all fields, and why F+12 receives the attachment index.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
extern "C" void* norrath_host_function(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#define NORRATH_WORD_TO_FUNCTION(word) norrath_host_function(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#define NORRATH_WORD_TO_FUNCTION(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int CalcLODLevelIndex__9VIHSpriteR13VIPickSegmentR8VIRaster(unsigned char* self, unsigned char* segment, unsigned char* raster);
int Process__9VIHSpriteiR7VIScene(unsigned char* self, int value, unsigned char* scene);
unsigned char* GetModelView__C8VIRaster(unsigned char* raster);
void CalcTransAboutCenter__9VIHSpriteRC10VIMatrix44R7VISceneR10VIMatrix44(unsigned char* self, unsigned char* matrix, unsigned char* scene, unsigned char* out);
int SetBlendMatrices__9VIHSpriteR8VIRaster(unsigned char* self, unsigned char* raster);
int Begin__Ct6VIPool1Z19VIHSpriteAttachment(unsigned char* pool);
int Next__Ct6VIPool1Z19VIHSpriteAttachmenti(unsigned char* pool, int index);
int End__Ct6VIPool1Z19VIHSpriteAttachment(unsigned char* pool);
void Mul__10VIMatrix44RC10VIMatrix44T1(unsigned char* out, unsigned char* left, unsigned char* right);
void SetModelView__8VIRasterRC10VIMatrix44(unsigned char* raster, unsigned char* matrix);
unsigned Sprite__7VIScenei(unsigned char* scene, int index);
void _$_10VIMatrix44(unsigned char* matrix, int flags);

int Pick__9VIHSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster(unsigned char* self, unsigned char* segment, unsigned char* filter, unsigned char* scene, unsigned char* raster) {
    alignas(16) float m0[16] = {0}, m1[16] = {0};
    unsigned char* M0 = reinterpret_cast<unsigned char*>(m0);
    unsigned char* M1 = reinterpret_cast<unsigned char*>(m1);
    int hit = 0;
    if (Process__9VIHSpriteiR7VIScene(self, CalcLODLevelIndex__9VIHSpriteR13VIPickSegmentR8VIRaster(self, segment, raster), scene) < 0) {
        _$_10VIMatrix44(M1, 2);
        _$_10VIMatrix44(M0, 2);
        return -1;
    }
    CalcTransAboutCenter__9VIHSpriteRC10VIMatrix44R7VISceneR10VIMatrix44(self, GetModelView__C8VIRaster(raster), scene, M0);
    if (SetBlendMatrices__9VIHSpriteR8VIRaster(self, raster) < 0) {
        _$_10VIMatrix44(M1, 2);
        _$_10VIMatrix44(M0, 2);
        return -1;
    }
    unsigned char* pool = self + 96;
    for (int k = Begin__Ct6VIPool1Z19VIHSpriteAttachment(pool); k != End__Ct6VIPool1Z19VIHSpriteAttachment(pool);
         k = Next__Ct6VIPool1Z19VIHSpriteAttachmenti(pool, k)) {
        int transform = *reinterpret_cast<int*>(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 104)) + 16 * k + 4);
        if (transform == -1) {
            SetModelView__8VIRasterRC10VIMatrix44(raster, M0);
        } else {
            Mul__10VIMatrix44RC10VIMatrix44T1(M1, NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 72)) + 256 * transform + 48, M0);
            SetModelView__8VIRasterRC10VIMatrix44(raster, M1);
        }
        unsigned char* sprite = NORRATH_WORD_TO_POINTER(Sprite__7VIScenei(scene,
            *reinterpret_cast<int*>(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 104)) + 16 * k)));
        unsigned char* slot = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(sprite + 60)) + 48;
        typedef int (*Slot)(unsigned char*, unsigned char*, unsigned char*, unsigned char*, unsigned char*);
        Slot function = reinterpret_cast<Slot>(NORRATH_WORD_TO_FUNCTION(*reinterpret_cast<unsigned*>(slot + 4)));
        if (function(sprite + *reinterpret_cast<short*>(slot), segment, filter, scene, raster) != 0) {
            hit = 1;
            *reinterpret_cast<int*>(filter + 12) = k;
        }
    }
    _$_10VIMatrix44(M1, 2);
    _$_10VIMatrix44(M0, 2);
    return hit;
}
}
