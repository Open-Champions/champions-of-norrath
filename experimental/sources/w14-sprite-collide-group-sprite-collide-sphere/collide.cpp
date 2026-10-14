// Observed: Collide__13VIGroupSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide at 0x0101cd30, 620 bytes. Arguments: the sprite (S), the sphere, the scene filter, the scene (V)
// and the collide record (C). Three 64-byte matrices M0, M1 and M2 are zero-filled and always destroyed on return
// (_$_10VIMatrix44 with flag 2: M2, then M1, then M0).
//  1. M0 receives a copy of GetWorld__C9VICollide(C) through sceVu0CopyMatrix.
//  2. The word at S+64 is a signed count. For each entry i from 0 (the count is read again after every pass), with the
//     32-byte entry at S+68+32*i: M1 = Transform__10VIMatrix44RC7VIVect3fT1(M1, S+72+32*i, float at S+84+32*i, S+88+32*i);
//     M2 = Mul__10VIMatrix44RC10VIMatrix44T1(M2, M1, M0); SetWorld__9VICollideRC10VIMatrix44(C, M2).
//  3. Sprite__7VIScenei(V, word at the entry start) gives sprite P. Its word at +60 is a table; the 8-byte entry at table+32 holds a
//     signed halfword adjustment and a function word at +4. That function is called with (P + adjustment, sphere, filter, V, C).
//  4. A nonzero result ends the loop with result 1. If every entry gives zero the result is 0. The world matrix of C is not
//     restored: it keeps the last value set.
// Inferred: a group of child sprites, each placed by a transform from a position, an angle and a second vector; the
// slot is the sphere collision virtual. Unknown: the names of the entry fields and whether the Transform arguments are an axis, angle and translation.
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
unsigned char* GetWorld__C9VICollide(unsigned char* collide);
void sceVu0CopyMatrix(unsigned char* to, unsigned char* from);
void Transform__10VIMatrix44RC7VIVect3fT1(unsigned char* matrix, unsigned char* first, float scalar, unsigned char* second);
void Mul__10VIMatrix44RC10VIMatrix44T1(unsigned char* out, unsigned char* left, unsigned char* right);
void SetWorld__9VICollideRC10VIMatrix44(unsigned char* collide, unsigned char* matrix);
unsigned Sprite__7VIScenei(unsigned char* scene, int index);
void _$_10VIMatrix44(unsigned char* matrix, int flags);

int Collide__13VIGroupSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide(unsigned char* self, unsigned char* shape, unsigned char* filter, unsigned char* scene, unsigned char* collide) {
    alignas(16) float m0[16] = {0}, m1[16] = {0}, m2[16] = {0};
    unsigned char* M0 = reinterpret_cast<unsigned char*>(m0);
    unsigned char* M1 = reinterpret_cast<unsigned char*>(m1);
    unsigned char* M2 = reinterpret_cast<unsigned char*>(m2);
    int result = 0;
    sceVu0CopyMatrix(M0, GetWorld__C9VICollide(collide));
    for (int i = 0; i < *reinterpret_cast<int*>(self + 64); ++i) {
        unsigned char* entry = self + 68 + 32 * i;
        Transform__10VIMatrix44RC7VIVect3fT1(M1, self + 72 + 32 * i, *reinterpret_cast<float*>(entry + 16), self + 88 + 32 * i);
        Mul__10VIMatrix44RC10VIMatrix44T1(M2, M1, M0);
        SetWorld__9VICollideRC10VIMatrix44(collide, M2);
        unsigned char* sprite = NORRATH_WORD_TO_POINTER(Sprite__7VIScenei(scene, *reinterpret_cast<int*>(entry)));
        unsigned char* slot = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(sprite + 60)) + 32;
        typedef int (*Slot)(unsigned char*, unsigned char*, unsigned char*, unsigned char*, unsigned char*);
        Slot function = reinterpret_cast<Slot>(NORRATH_WORD_TO_FUNCTION(*reinterpret_cast<unsigned*>(slot + 4)));
        if (function(sprite + *reinterpret_cast<short*>(slot), shape, filter, scene, collide) != 0) {
            result = 1;
            break;
        }
    }
    _$_10VIMatrix44(M2, 2);
    _$_10VIMatrix44(M1, 2);
    _$_10VIMatrix44(M0, 2);
    return result;
}
}
