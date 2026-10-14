// Observed: CollideObj__9VICollideR12VICollSpherei at 0x010926e0, 340 bytes, in the pinned SLUS-20565 image. Arguments: the collide record (C), a sphere object (X) that the caller owns and an index I.
// A local 12VICollSphere object L (96 bytes) is built with __12VICollSphere(L), then Init__12VICollSphereRC12VICollSphereRC10VIMatrix44(L, X, C+80) runs. A negative result returns -1 straight away.
// Otherwise Collide__9VICollideR12VICollSphereR12VICollBuffer(C, L, B) runs, where B is the buffer fetched above; a zero result returns 0. On a hit: the float at L+68 is copied to X+68, the 32-bit word at L+72 to X+72;
// the three floats at L+76..84 form a four-float vector with w = 0.0, which __ml__FRC7VIVect4RC10VIMatrix44(result, vector, C+16) multiplies by the world matrix.
// All four result lanes are copied to a second four-float vector, Normalize__7VIVect4 normalises that in place, and its x, y, z are stored to X+76, X+80 and X+84 (w is dropped). The function returns 1.
// Inferred: X+68/+72 receive a hit distance and a hit identifier of some kind and X+76..84 a world-space direction; L's field roles are not otherwise confirmed, and L's size of 96 bytes comes from the
// stack slot the original reserves. Unknown: the library names' semantics beyond their call sites, and whether the original constructs the two local vectors with a default constructor (observed: their four lanes are stored as 0.0).
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
void __12VICollSphere(unsigned char* self);
int Init__12VICollSphereRC12VICollSphereRC10VIMatrix44(unsigned char* self, const unsigned char* source, const unsigned char* matrix);
int Collide__9VICollideR12VICollSphereR12VICollBuffer(unsigned char* self, unsigned char* probe, unsigned char* buffer);
float* __ml__FRC7VIVect4RC10VIMatrix44(float* result, const float* vector, const unsigned char* matrix);
void Normalize__7VIVect4(float* self);

int CollideObj__9VICollideR12VICollSpherei(unsigned char* self, unsigned char* sphere, int index) {
    unsigned char* buffer = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 536)) + index * 12));
    alignas(16) unsigned char probe[96];
    __12VICollSphere(probe);
    float point[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    float direction[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    if (Init__12VICollSphereRC12VICollSphereRC10VIMatrix44(probe, sphere, self + 80) < 0) return -1;
    if (Collide__9VICollideR12VICollSphereR12VICollBuffer(self, probe, buffer) == 0) return 0;
    const float* lanes = reinterpret_cast<const float*>(probe);
    *reinterpret_cast<float*>(sphere + 68) = lanes[17];
    *reinterpret_cast<unsigned*>(sphere + 72) = *reinterpret_cast<const unsigned*>(probe + 72);
    point[0] = lanes[19];
    point[1] = lanes[20];
    point[2] = lanes[21];
    point[3] = 0.0f;
    alignas(16) float world[4];
    __ml__FRC7VIVect4RC10VIMatrix44(world, point, self + 16);
    direction[0] = world[0];
    direction[1] = world[1];
    direction[2] = world[2];
    direction[3] = world[3];
    Normalize__7VIVect4(direction);
    *reinterpret_cast<float*>(sphere + 76) = direction[0];
    *reinterpret_cast<float*>(sphere + 84) = direction[2];
    *reinterpret_cast<float*>(sphere + 80) = direction[1];
    return 1;
}
}
