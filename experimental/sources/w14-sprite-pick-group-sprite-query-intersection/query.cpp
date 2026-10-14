// Observed: QueryIntersection__13VIGroupSpriteRC6VIBBoxR14VIQueryTriListRC10VIMatrix44R7VIScene at 0x0101d658, 468 bytes. Arguments: the sprite (S), the box (B), the triangle list (L), the matrix (X) and the
// scene (V). Two 64-byte matrices M0 and M1 are zero-filled and always destroyed on return (_$_10VIMatrix44 with flag 2: M1, then M0).
//  1. The word at S+64 is a signed count (read again after every pass). For each entry i from 0, with the 32-byte entry at S+68+32*i:
//     Transform__10VIMatrix44RC7VIVect3fT1(M0, S+72+32*i, float at S+84+32*i, S+88+32*i); Mul__10VIMatrix44RC10VIMatrix44T1(M1, M0, X).
//  2. Sprite__7VIScenei(V, word at the entry start) gives sprite P. Its word at +60 is a table; the 8-byte entry at table+64 holds a
//     signed halfword adjustment and a function word at +4. That function is called with (P + adjustment, B, L, M1, V).
//  3. A negative result ends the loop with result -1 (non-negative results, including positive ones, are ignored); if no entry
//     gives a negative result the result is 0.
// Inferred: a group of child sprites placed by transforms and queried one after another; the slot is the intersection-query virtual,
// and M1 is the entry's placement composed with the caller's matrix. Unknown: the names of the entry fields and the Transform argument roles.
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
unsigned char* GetModelView__C8VIRaster(unsigned char* raster);
void sceVu0CopyMatrix(unsigned char* to, unsigned char* from);
void Transform__10VIMatrix44RC7VIVect3fT1(unsigned char* matrix, unsigned char* first, float scalar, unsigned char* second);
void Mul__10VIMatrix44RC10VIMatrix44T1(unsigned char* out, unsigned char* left, unsigned char* right);
void SetModelView__8VIRasterRC10VIMatrix44(unsigned char* raster, unsigned char* matrix);
unsigned Sprite__7VIScenei(unsigned char* scene, int index);
void _$_10VIMatrix44(unsigned char* matrix, int flags);

int QueryIntersection__13VIGroupSpriteRC6VIBBoxR14VIQueryTriListRC10VIMatrix44R7VIScene(unsigned char* self, unsigned char* box, unsigned char* list, unsigned char* matrix, unsigned char* scene) {
    alignas(16) float m0[16] = {0}, m1[16] = {0};
    unsigned char* M0 = reinterpret_cast<unsigned char*>(m0);
    unsigned char* M1 = reinterpret_cast<unsigned char*>(m1);
    int result = 0;
    for (int i = 0; i < *reinterpret_cast<int*>(self + 64); ++i) {
        unsigned char* entry = self + 68 + 32 * i;
        Transform__10VIMatrix44RC7VIVect3fT1(M0, self + 72 + 32 * i, *reinterpret_cast<float*>(entry + 16), self + 88 + 32 * i);
        Mul__10VIMatrix44RC10VIMatrix44T1(M1, M0, matrix);
        unsigned char* sprite = NORRATH_WORD_TO_POINTER(Sprite__7VIScenei(scene, *reinterpret_cast<int*>(entry)));
        unsigned char* slot = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(sprite + 60)) + 64;
        typedef int (*Slot)(unsigned char*, unsigned char*, unsigned char*, unsigned char*, unsigned char*);
        Slot function = reinterpret_cast<Slot>(NORRATH_WORD_TO_FUNCTION(*reinterpret_cast<unsigned*>(slot + 4)));
        if (function(sprite + *reinterpret_cast<short*>(slot), box, list, M1, scene) < 0) {
            result = -1;
            break;
        }
    }
    _$_10VIMatrix44(M1, 2);
    _$_10VIMatrix44(M0, 2);
    return result;
}
}
