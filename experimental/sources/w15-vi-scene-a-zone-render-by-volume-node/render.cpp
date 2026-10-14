// Observed: RenderByVolume__6VIZoneiR15VISceneRendVars at 0x011155b0, 628 bytes, recursive. Arguments: the zone (Z), a node value (N) and the
// render-variables record (V).
//  - If N has any of the bits 0xf0000000 and also bit 0x40000000, it tail-calls RenderLeaf__6VIZoneiR15VISceneRendVars(Z, N & 0x0fffffff, V)
//    and returns its result. High bits without 0x40000000 are masked off (N &= 0x0fffffff); otherwise N is used unchanged.
//  - Otherwise N indexes 24-byte records at the pointer word in Z+72: four floats a, b, c, d, then child values at
//    +16 and +20. V+24 is a signed count and V+28 a pointer to that many three-float points. For each point, dot(point, abc) + d, summed in the
//    order x, y, z, d, sets the "front" flag when it is greater than 0.0 and the "back" flag otherwise (so NaN counts as back). The loop
//    stops as soon as both flags are set. The position (V word +12, floats at +28..+36) is evaluated the same way and is "behind" when
//    its value is below 0.0.
//  - Behind: the child at +16 is rendered when front is set, then the child at +20 when back is set. Not behind: +20 when back is set, then
//    +16 when front is set. A child is rendered by calling this function again with (Z, child, V). A negative result of the first child
//    returns -1; the result of the second decides between 0 (non-negative) and -1; the record pointer is re-read from Z before the second
//    child. If neither child is due the result is 0.
// Inferred: a BSP-style walk like RenderByVolume__7VIWorldiR15VISceneRendVars on the zone's own planes, pruning with the point set's spread and ordering
// children by the viewer side; child values with bit 0x40000000 are leaves.
// Unknown: the meaning of the other high nibble bits, the types of the record and the vars struct, and what RenderLeaf draws.
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
int RenderLeaf__6VIZoneiR15VISceneRendVars(unsigned char* zone, int leaf, unsigned char* vars);

int RenderByVolume__6VIZoneiR15VISceneRendVars(unsigned char* zone, int node, unsigned char* vars) {
    if ((node & 0xf0000000) != 0) {
        if ((node & 0x40000000) != 0) return RenderLeaf__6VIZoneiR15VISceneRendVars(zone, node & 0x0fffffff, vars);
        node &= 0x0fffffff;
    }
    bool front = false;
    bool back = false;
    int count = *reinterpret_cast<int*>(vars + 24);
    const float* plane = reinterpret_cast<const float*>(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(zone + 72)) + node * 24);
    if (count > 0) {
        const float* point = reinterpret_cast<const float*>(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(vars + 28)));
        for (int i = 0; i < count; ++i) {
            float distance = point[0] * plane[0] + point[1] * plane[1] + point[2] * plane[2] + plane[3];
            if (0.0f < distance) front = true;
            else back = true;
            if (front && back) break;
            point += 3;
        }
    }
    plane = reinterpret_cast<const float*>(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(zone + 72)) + node * 24);
    const float* eye = reinterpret_cast<const float*>(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(vars + 12)) + 28);
    float eyeDistance = eye[0] * plane[0] + eye[1] * plane[1] + eye[2] * plane[2] + plane[3];
    bool behind = eyeDistance < 0.0f;
    if (behind) {
        if (front) {
            if (RenderByVolume__6VIZoneiR15VISceneRendVars(zone, *reinterpret_cast<const int*>(plane + 4), vars) < 0) return -1;
        }
        if (!back) return 0;
        plane = reinterpret_cast<const float*>(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(zone + 72)) + node * 24);
        return RenderByVolume__6VIZoneiR15VISceneRendVars(zone, *reinterpret_cast<const int*>(plane + 5), vars) >= 0 ? 0 : -1;
    }
    if (back) {
        if (RenderByVolume__6VIZoneiR15VISceneRendVars(zone, *reinterpret_cast<const int*>(plane + 5), vars) < 0) return -1;
    }
    if (!front) return 0;
    plane = reinterpret_cast<const float*>(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(zone + 72)) + node * 24);
    return RenderByVolume__6VIZoneiR15VISceneRendVars(zone, *reinterpret_cast<const int*>(plane + 4), vars) >= 0 ? 0 : -1;
}
}
