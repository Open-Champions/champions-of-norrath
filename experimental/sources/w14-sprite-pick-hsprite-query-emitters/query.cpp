// Observed: QueryEmitters__9VIHSpriteR7VISceneRC10VIMatrix44R14VIEmitterQuery at 0x01060ef8, 532 bytes. Arguments: the sprite (S), the scene (V), the matrix (X) and the emitter query (Q). Two
// 64-byte matrices M0 and M1 are zero-filled and always destroyed on return (_$_10VIMatrix44 with flag 2: M1, then M0).
//  1. CalcLODLevelIndex__9VIHSpriteRC10VIMatrix44(S, X) gives a level L; Process__9VIHSpriteiR7VIScene(S, L, V) runs. A negative
//     result returns -1 after the destructors, with nothing else done.
//  2. CalcTransAboutCenter__9VIHSpriteRC10VIMatrix44R7VISceneR10VIMatrix44(S, X, V, M1).
//  3. The pool at S+96 is walked with Begin__Ct6VIPool1Z19VIHSpriteAttachment, Next__...i and End__... (the end value is fetched
//     again after every step). For index k the 16-byte record is at (word at S+104) + 16*k: word +0 is a sprite index and word +4 a
//     transform index. A transform index of -1 skips the record. Otherwise Mul__10VIMatrix44RC10VIMatrix44T1(M0, (word at S+72) +
//     256*transform + 48, M1) runs, the record's sprite index is stored at Q+12, and Sprite__7VIScenei(V, that index) gives sprite P.
//     Its word at +60 is a table; the 8-byte entry at table+56 holds a signed halfword adjustment and a function word at +4. That
//     function is called with (P + adjustment, V, M0, Q).
//  4. A negative result returns -1 after the destructors; otherwise the walk goes on. If it ends the result is 0.
// Inferred: the HSprite walk of its intersection query, asking each attachment for emitters instead; Q+12 records which attachment.
// Unknown: names of all fields.
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
int CalcLODLevelIndex__9VIHSpriteRC10VIMatrix44(unsigned char* self, unsigned char* matrix);
int Process__9VIHSpriteiR7VIScene(unsigned char* self, int value, unsigned char* scene);
void CalcTransAboutCenter__9VIHSpriteRC10VIMatrix44R7VISceneR10VIMatrix44(unsigned char* self, unsigned char* matrix, unsigned char* scene, unsigned char* out);
int Begin__Ct6VIPool1Z19VIHSpriteAttachment(unsigned char* pool);
int Next__Ct6VIPool1Z19VIHSpriteAttachmenti(unsigned char* pool, int index);
int End__Ct6VIPool1Z19VIHSpriteAttachment(unsigned char* pool);
void Mul__10VIMatrix44RC10VIMatrix44T1(unsigned char* out, unsigned char* left, unsigned char* right);
unsigned Sprite__7VIScenei(unsigned char* scene, int index);
void _$_10VIMatrix44(unsigned char* matrix, int flags);

int QueryEmitters__9VIHSpriteR7VISceneRC10VIMatrix44R14VIEmitterQuery(unsigned char* self, unsigned char* scene, unsigned char* matrix, unsigned char* query) {
    alignas(16) float m0[16] = {0}, m1[16] = {0};
    unsigned char* M0 = reinterpret_cast<unsigned char*>(m0);
    unsigned char* M1 = reinterpret_cast<unsigned char*>(m1);
    if (Process__9VIHSpriteiR7VIScene(self, CalcLODLevelIndex__9VIHSpriteRC10VIMatrix44(self, matrix), scene) < 0) {
        _$_10VIMatrix44(M1, 2);
        _$_10VIMatrix44(M0, 2);
        return -1;
    }
    CalcTransAboutCenter__9VIHSpriteRC10VIMatrix44R7VISceneR10VIMatrix44(self, matrix, scene, M1);
    unsigned char* pool = self + 96;
    for (int k = Begin__Ct6VIPool1Z19VIHSpriteAttachment(pool); k != End__Ct6VIPool1Z19VIHSpriteAttachment(pool);
         k = Next__Ct6VIPool1Z19VIHSpriteAttachmenti(pool, k)) {
        int transform = *reinterpret_cast<int*>(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 104)) + 16 * k + 4);
        if (transform == -1) continue;
        Mul__10VIMatrix44RC10VIMatrix44T1(M0, NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 72)) + 256 * transform + 48, M1);
        int index = *reinterpret_cast<int*>(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 104)) + 16 * k);
        *reinterpret_cast<int*>(query + 12) = index;
        unsigned char* sprite = NORRATH_WORD_TO_POINTER(Sprite__7VIScenei(scene, index));
        unsigned char* slot = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(sprite + 60)) + 56;
        typedef int (*Slot)(unsigned char*, unsigned char*, unsigned char*, unsigned char*);
        Slot function = reinterpret_cast<Slot>(NORRATH_WORD_TO_FUNCTION(*reinterpret_cast<unsigned*>(slot + 4)));
        if (function(sprite + *reinterpret_cast<short*>(slot), scene, M0, query) < 0) {
            _$_10VIMatrix44(M1, 2);
            _$_10VIMatrix44(M0, 2);
            return -1;
        }
    }
    _$_10VIMatrix44(M1, 2);
    _$_10VIMatrix44(M0, 2);
    return 0;
}
}
