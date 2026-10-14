// Observed: QueryEmitters__6VIZoneiR23VISceneEmitterQueryVars at 0x01118cd8 is 404 bytes in the pinned SLUS-20565 image. Arguments: zone,
// node (int), vars. Node numbers carry flags in the top nibble: if (node & 0xf0000000) is non-zero and bit 0x40000000 is set, it tail-calls
// QueryEmittersLeaf__6VIZoneiR23VISceneEmitterQueryVars(zone, node & 0x0fffffff, vars) and returns that result unchanged; if the nibble
// is non-zero without that bit, the nibble is just masked off. Otherwise the node is a 24-byte record at (word at zone+72) + 24 * node:
// four floats (plane a, b, c, d at +0, +4, +8, +12) and two child node numbers at +16 and +20. With x, y, z the floats at vars+12, +16, +20
// and r the float at vars+24, dist = ((x*a + y*b) + z*c) + d (separate multiplies and adds):
//   r < dist: visit child +16 only;  dist < -r: visit child +20 only;
//   otherwise visit both: child +16 then +20 if 0 < dist, else child +20 then +16 (the base pointer is re-read after the first visit).
// Each visit is a recursive call; a negative result returns -1 at once. It returns 0 when every visit succeeded.
// Inferred: a BSP-style tree over the zone with plane tests against a sphere (x, y, z, r in the query variables); +16 is the front child
// and +20 the back child; the 0x40000000 flag marks a leaf and is handled by the sibling routine.
// Unknown: the leaf routine (an import), what a "flagged but not leaf" node means (the flag bits other than 0x40000000), and the query
// variables' other fields. Nothing here ran on the EE.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif
extern "C" {
int QueryEmittersLeaf__6VIZoneiR23VISceneEmitterQueryVars(unsigned char* zone, int node, unsigned char* vars);

int QueryEmitters__6VIZoneiR23VISceneEmitterQueryVars(unsigned char* zone, int node, unsigned char* vars) {
    unsigned bits = static_cast<unsigned>(node);
    if ((bits & 0xf0000000u) != 0) {
        if ((bits & 0x40000000u) != 0)
            return QueryEmittersLeaf__6VIZoneiR23VISceneEmitterQueryVars(zone, static_cast<int>(bits & 0x0fffffffu), vars);
        bits &= 0x0fffffffu;
    }
    int offset = static_cast<int>(bits) * 24;
    const float* plane = reinterpret_cast<const float*>(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(zone + 72)) + offset);
    const float* point = reinterpret_cast<const float*>(vars + 12);
    float radius = *reinterpret_cast<const float*>(vars + 24);
    float dist = point[0] * plane[0];
    float second = point[1] * plane[1];
    float third = point[2] * plane[2];
    dist = dist + second;
    dist = dist + third;
    dist = dist + plane[3];
    int front = *reinterpret_cast<const int*>(reinterpret_cast<const unsigned char*>(plane) + 16);
    int back = *reinterpret_cast<const int*>(reinterpret_cast<const unsigned char*>(plane) + 20);
    int next;
    if (radius < dist) {
        next = front;
    } else if (dist < -radius) {
        next = back;
    } else {
        int first = 0.0f < dist ? front : back;
        if (QueryEmitters__6VIZoneiR23VISceneEmitterQueryVars(zone, first, vars) < 0) return -1;
        const unsigned char* again = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(zone + 72)) + offset;
        next = *reinterpret_cast<const int*>(again + (0.0f < dist ? 20 : 16));
    }
    return QueryEmitters__6VIZoneiR23VISceneEmitterQueryVars(zone, next, vars) < 0 ? -1 : 0;
}
}
