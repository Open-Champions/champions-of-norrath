// Observed: QueryEmitters__7VIWorldiR23VISceneEmitterQueryVars at 0x010fdff0, 444 bytes. Arguments: the world (W), a node word (N) and the emitter query variables (X).
//  1. If N has bit 0x40000000 set (and so some of the top nibble), the zone index is the word at (word at W+64) + 4*(N & 0x0fffffff),
//     the zone is at (word at W+76) + 592*(zone index), and the result is that of QueryEmitters__6VIZoneR23VISceneEmitterQueryVars(zone, X),
//     returned directly. Other top-nibble bits are cleared and N is then treated as an inner node.
//  2. An inner node N is a 24-byte record at (word at W+56) + 24*N: four floats (A, B, C, D), then two node words F (+16) and K (+20).
//     The distance is L = ((X[3]*A + X[4]*B) + X[5]*C) + D over the floats X+12, X+16, X+20, and the radius is the float X+24.
//     * radius < L: only F is visited. * L < -radius: only K is visited.
//     * otherwise both are: F then K when 0 < L, K then F when L <= 0 (the record is looked up again before the second visit).
//     Each visit is a recursive call of this function; a negative result returns -1 at once (for the first of two visits), and the
//     result of the last visit decides the return: -1 if negative, otherwise 0.
// Inferred: a binary space partition over planes whose leaves are zones, walked with a sphere (centre X+12, radius X+24).
// Unknown: the names of all fields, and what the other top-nibble bits of a node word mean.
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
int QueryEmitters__6VIZoneR23VISceneEmitterQueryVars(unsigned char* zone, unsigned char* vars);

int QueryEmitters__7VIWorldiR23VISceneEmitterQueryVars(unsigned char* world, int node, unsigned char* vars) {
    if ((node & 0xf0000000) != 0) {
        if ((node & 0x40000000) != 0) {
            int zoneIndex = *reinterpret_cast<int*>(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(world + 64)) + 4 * (node & 0x0fffffff));
            return QueryEmitters__6VIZoneR23VISceneEmitterQueryVars(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(world + 76)) + 592 * zoneIndex, vars);
        }
        node &= 0x0fffffff;
    }
    unsigned char* plane = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(world + 56)) + 24 * node;
    const float* p = reinterpret_cast<const float*>(plane);
    const float* v = reinterpret_cast<const float*>(vars + 12);
    float distance = v[0] * p[0] + v[1] * p[1] + v[2] * p[2] + p[3];
    float radius = *reinterpret_cast<float*>(vars + 24);
    int next;
    if (radius < distance) {
        next = *reinterpret_cast<int*>(plane + 16);
    } else if (distance < -radius) {
        next = *reinterpret_cast<int*>(plane + 20);
    } else {
        int first = 0 < distance ? 16 : 20;
        if (QueryEmitters__7VIWorldiR23VISceneEmitterQueryVars(world, *reinterpret_cast<int*>(plane + first), vars) < 0) return -1;
        plane = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(world + 56)) + 24 * node;
        next = *reinterpret_cast<int*>(plane + (first == 16 ? 20 : 16));
    }
    return QueryEmitters__7VIWorldiR23VISceneEmitterQueryVars(world, next, vars) >= 0 ? 0 : -1;
}
}
