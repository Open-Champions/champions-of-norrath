// Observed: RenderByVolume__7VIWorldiR15VISceneRendVars at 0x010fbb48, 644 bytes, recursive.
// With W the world, `node` the int argument and V the render-vars argument:
//  - If node has any of the bits 0xf0000000 and bit 0x40000000 is set, it tail-calls
//    RenderByVolume__6VIZoneR15VISceneRendVarsi(zone, V, k) where i = node & 0x0fffffff,
//    k = the word at index i of the int array pointed to by W word +64, and zone = W word +76 + k * 592;
//    the result is returned as is. If the high bits are set without bit 0x40000000 they are
//    masked off (node &= 0x0fffffff) and processing continues; otherwise node is used unchanged.
//  - Otherwise node indexes 24-byte plane records at W word +56 (four floats a, b, c, d, then
//    word +16 and word +20 child values). V word +24 is a count n and V word +28 points to n
//    three-float points. For each point, dot(point, abc) + d > 0 sets a "front" flag, any other
//    value (including NaN) sets a "back" flag. The camera position (V word +12, floats at +28)
//    is evaluated the same way; "behind" means its value is below 0.
//  - If behind: render the child at +16 when the front flag is set, then the child at +20
//    when the back flag is set. If not behind: the child at +20 when the back flag is set,
//    then the child at +16 when the front flag is set. A child is rendered by calling this
//    function again with (W, child, V); a negative result is returned as -1 at once for the
//    first child, and for the second the result decides between 0 (non-negative) and -1.
//    The plane record pointer is re-read from W before the second child. With neither child
//    due the result is 0.
// Inferred: a BSP-style walk that prunes by a point set's spread across each plane and orders
// children by the camera side; child values with the 0x40000000 bit refer to zones.
// Unknown: the member names, the exact meaning of the nibble bits other than 0x40000000, the
// struct types, and the value of the unnamed 592-byte zone records.
#if defined(NORRATH_HOST_CONTRACT)
// On the 64-bit host a pointer field is modelled as a 32-bit offset into an arena.
extern "C" unsigned char* norrath_host_load_pointer(const void* field);
#define NORRATH_LOAD_POINTER(field) norrath_host_load_pointer(field)
#else
#define NORRATH_LOAD_POINTER(field) (*reinterpret_cast<unsigned char* const*>(field))
#endif

extern "C" {
int RenderByVolume__6VIZoneR15VISceneRendVarsi(void* zone, void* vars, int index);

int RenderByVolume__7VIWorldiR15VISceneRendVars(void* world, int node, void* vars) {
    unsigned char* w = static_cast<unsigned char*>(world);
    unsigned char* v = static_cast<unsigned char*>(vars);
    if ((node & 0xf0000000) != 0) {
        if ((node & 0x40000000) != 0) {
            int zone = reinterpret_cast<int*>(NORRATH_LOAD_POINTER(w + 64))[node & 0x0fffffff];
            return RenderByVolume__6VIZoneR15VISceneRendVarsi(NORRATH_LOAD_POINTER(w + 76) + zone * 592, vars, zone);
        }
        node &= 0x0fffffff;
    }
    bool front = false;
    bool back = false;
    bool behind = false;
    int count = *reinterpret_cast<int*>(v + 24);
    const float* plane = reinterpret_cast<const float*>(NORRATH_LOAD_POINTER(w + 56) + node * 24);
    if (count > 0) {
        const float* point = reinterpret_cast<const float*>(NORRATH_LOAD_POINTER(v + 28));
        do {
            float distance = point[0] * plane[0] + point[1] * plane[1] + point[2] * plane[2] + plane[3];
            if (0.0f < distance) front = true;
            else back = true;
            point += 3;
            --count;
        } while (count != 0);
    }
    plane = reinterpret_cast<const float*>(NORRATH_LOAD_POINTER(w + 56) + node * 24);
    const float* eye = reinterpret_cast<const float*>(NORRATH_LOAD_POINTER(v + 12) + 28);
    float eyeDistance = eye[0] * plane[0] + eye[1] * plane[1] + eye[2] * plane[2] + plane[3];
    if (eyeDistance < 0.0f) behind = true;
    const unsigned char* record = reinterpret_cast<const unsigned char*>(plane);
    if (behind) {
        if (front) {
            if (RenderByVolume__7VIWorldiR15VISceneRendVars(world, *reinterpret_cast<const int*>(record + 16), vars) < 0) return -1;
        }
        if (!back) return 0;
        record = NORRATH_LOAD_POINTER(w + 56) + node * 24;
        return RenderByVolume__7VIWorldiR15VISceneRendVars(world, *reinterpret_cast<const int*>(record + 20), vars) >= 0 ? 0 : -1;
    }
    if (back) {
        if (RenderByVolume__7VIWorldiR15VISceneRendVars(world, *reinterpret_cast<const int*>(record + 20), vars) < 0) return -1;
    }
    if (!front) return 0;
    record = NORRATH_LOAD_POINTER(w + 56) + node * 24;
    return RenderByVolume__7VIWorldiR15VISceneRendVars(world, *reinterpret_cast<const int*>(record + 16), vars) >= 0 ? 0 : -1;
}
}
