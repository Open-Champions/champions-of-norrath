// Observed: Collide__7VIWorldiR8VIEnvRayR7VIVect3T3 at 0x010fd378, 700 bytes, in the pinned SLUS-20565 image; it calls itself. Arguments: the world (W), a 32-bit tree index (I), the ray object (R),
// and two points A and B (three floats each, taken by reference). W+56 is a node array (24 bytes a node), W+64 an array of zone numbers and W+76 the zone array (592 bytes a zone).
// Index handling is that of QueryIntersection__7VIWorldiR20VISceneIntersectVars: with any of the top four bits set, bit 30 marks a leaf. A leaf calls
// Collide__6VIZoneR8VIEnvRayR7VIVect3T2(zone, R, A, B) and returns its result unchanged; otherwise the top nibble is masked off and the node path runs.
// Node path: the node holds a plane (three floats, then the offset d) at +0..+12 and child indices at +16 and +20. dA = ((A0*n0 + A1*n1) + A2*n2) + d and dB likewise for B.
//  - 0 <= dA and 0 <= dB: recurse into the child at +16 with (R, A, B); the result is 1 when that is nonzero, else 0.
//  - dA <= 0 and dB <= 0: the same with the child at +20.
//  - otherwise the segment is split at t = dA / (dA - dB): M = A + (B - A) * t, computed per lane as (B[i]-A[i])*t + A[i], into a local three-float vector.
//    If 0 < dA the child +16 is queried with (A, M) and, only if that result is zero, the child +20 with (M, B); otherwise the child +20 first with (A, M) and then the child +16 with (M, B).
//    The function returns 1 as soon as the first query is nonzero, else the second query's result as 0 or 1.
// Inferred: a ray-versus-BSP walk testing a segment A-B against the splitting planes, ending in per-zone queries; the result is a hit flag. The ray object is only passed along.
// Unknown: the meaning of the ray object's fields and whether a ray of zero length (dA == dB) can reach the division (it would divide zero by zero, as the original does).
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
int Collide__6VIZoneR8VIEnvRayR7VIVect3T2(unsigned char* zone, unsigned char* ray, const float* a, const float* b);

int Collide__7VIWorldiR8VIEnvRayR7VIVect3T3(unsigned char* self, int index, unsigned char* ray, const float* a, const float* b) {
    float middle[3] = {0.0f, 0.0f, 0.0f};
    unsigned tree = static_cast<unsigned>(index);
    if ((tree & 0xf0000000u) != 0) {
        if ((tree & 0x40000000u) != 0) {
            unsigned char* numbers = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 64));
            unsigned char* zones = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 76));
            const int zone = *reinterpret_cast<int*>(numbers + 4 * (tree & 0x0fffffffu));
            return Collide__6VIZoneR8VIEnvRayR7VIVect3T2(zones + 592 * zone, ray, a, b);
        }
        tree &= 0x0fffffffu;
    }
    const float* plane = reinterpret_cast<const float*>(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 56)) + 24 * tree);
    const float distanceA = a[0] * plane[0] + a[1] * plane[1] + a[2] * plane[2] + plane[3];
    const float distanceB = b[0] * plane[0] + b[1] * plane[1] + b[2] * plane[2] + plane[3];
    const int front = *reinterpret_cast<const int*>(plane + 4);
    const int back = *reinterpret_cast<const int*>(plane + 5);
    if (0.0f <= distanceA && 0.0f <= distanceB) {
        return Collide__7VIWorldiR8VIEnvRayR7VIVect3T3(self, front, ray, a, b) != 0;
    }
    if (distanceA <= 0.0f && distanceB <= 0.0f) {
        return Collide__7VIWorldiR8VIEnvRayR7VIVect3T3(self, back, ray, a, b) != 0;
    }
    const float t = distanceA / (distanceA - distanceB);
    middle[0] = (b[0] - a[0]) * t + a[0];
    middle[1] = (b[1] - a[1]) * t + a[1];
    middle[2] = (b[2] - a[2]) * t + a[2];
    if (0.0f < distanceA) {
        if (Collide__7VIWorldiR8VIEnvRayR7VIVect3T3(self, front, ray, a, middle) != 0) return 1;
        return Collide__7VIWorldiR8VIEnvRayR7VIVect3T3(self, back, ray, middle, b) != 0;
    }
    if (Collide__7VIWorldiR8VIEnvRayR7VIVect3T3(self, back, ray, a, middle) != 0) return 1;
    return Collide__7VIWorldiR8VIEnvRayR7VIVect3T3(self, front, ray, middle, b) != 0;
}
}
