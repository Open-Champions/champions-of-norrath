// Observed: QueryIntersection__7VIWorldiR20VISceneIntersectVars at 0x010fe1b0, 592 bytes, in the pinned SLUS-20565 image; it calls itself. Arguments: the world (W), a 32-bit tree index (I) and
// the intersect variables (V). Offsets used on W: the word at W+56 is a node array (24 bytes per node), the word at W+64 an array of 32-bit zone numbers and the word at W+76 the zone array (592 bytes per zone).
// If I has any of its top four bits set: when bit 30 (0x40000000) is also set it is a leaf; the zone is (W+76 word) + 592 * (zone-number array [I & 0x0fffffff]), the result is
// QueryIntersection__6VIZoneR20VISceneIntersectVars(zone, V), and the function returns -1 if that is negative, else 0. With bit 30 clear the top nibble is masked off (I & 0x0fffffff) and the node path below runs.
// Node path: a node holds a plane (three floats and a fourth, offset) at +0..+12 and two child indices at +16 and +20. The signed distance is
// ((V[140]*n0 + V[144]*n1) + V[148]*n2) + d, with the three floats at V+140; V+152 holds a float extent r.
//  - r < distance: recurse into the child at +16 and return -1 if that result is negative, else 0.
//  - distance < -r: the same with the child at +20.
//  - otherwise each of eight points, three floats apiece at V+44+12k, is tested with the same plane formula; a positive value sets the "front" flag, anything else the "back" flag.
//    If front, the +16 child is queried and a negative result returns -1. If not back, return 0. Otherwise the +20 child is queried and the function returns -1 for a negative result, else 0.
// Inferred: a binary-space-partition walk that tests a swept box (eight corners, a centre and a radius) against the splitting planes and ends in zone queries at the leaves; the negative result means a hit or failure
// in the zone query (its meaning is not established here). Field names for V are not established. Unknown: whether index values with the top nibble 1-3 or 5-f are ever produced, and the zone routine's result values.
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
int QueryIntersection__6VIZoneR20VISceneIntersectVars(unsigned char* zone, unsigned char* vars);

int QueryIntersection__7VIWorldiR20VISceneIntersectVars(unsigned char* self, int index, unsigned char* vars) {
    unsigned tree = static_cast<unsigned>(index);
    if ((tree & 0xf0000000u) != 0) {
        if ((tree & 0x40000000u) != 0) {
            unsigned char* numbers = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 64));
            unsigned char* zones = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 76));
            const int zone = *reinterpret_cast<int*>(numbers + 4 * (tree & 0x0fffffffu));
            return QueryIntersection__6VIZoneR20VISceneIntersectVars(zones + 592 * zone, vars) < 0 ? -1 : 0;
        }
        tree &= 0x0fffffffu;
    }
    unsigned char* nodes = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 56));
    const float* plane = reinterpret_cast<const float*>(nodes + 24 * tree);
    const float* centre = reinterpret_cast<const float*>(vars + 140);
    const float extent = *reinterpret_cast<const float*>(vars + 152);
    const float distance = centre[0] * plane[0] + centre[1] * plane[1] + centre[2] * plane[2] + plane[3];
    if (extent < distance) {
        return QueryIntersection__7VIWorldiR20VISceneIntersectVars(self, *reinterpret_cast<const int*>(plane + 4), vars) < 0 ? -1 : 0;
    }
    if (distance < -extent) {
        return QueryIntersection__7VIWorldiR20VISceneIntersectVars(self, *reinterpret_cast<const int*>(plane + 5), vars) < 0 ? -1 : 0;
    }
    bool front = false, back = false;
    for (int k = 0; k < 8; ++k) {
        const float* corner = reinterpret_cast<const float*>(vars + 44 + 12 * k);
        const float side = corner[0] * plane[0] + corner[1] * plane[1] + corner[2] * plane[2] + plane[3];
        if (0.0f < side) front = true; else back = true;
    }
    if (front && QueryIntersection__7VIWorldiR20VISceneIntersectVars(self, *reinterpret_cast<const int*>(plane + 4), vars) < 0) return -1;
    if (!back) return 0;
    return QueryIntersection__7VIWorldiR20VISceneIntersectVars(self, *reinterpret_cast<const int*>(plane + 5), vars) < 0 ? -1 : 0;
}
}
