// Observed: Collide__7VIWorldiR18VISceneCollRayVars at 0x010fc130, 840 bytes, in the pinned SLUS-20565 image; it calls itself. Arguments: the world (W), a 32-bit tree index (I) and the ray variables (V).
// W+56 is a node array (24 bytes a node), W+64 an array of zone numbers and W+76 the zone array (592 bytes a zone). Index handling is that of QueryIntersection__7VIWorldiR20VISceneIntersectVars: a leaf (bit 30 of an index
// with a nonzero top nibble) tail-calls Collide__6VIZoneR18VISceneCollRayVars(zone, V) and returns its result unchanged; otherwise the top nibble is masked off and the node path runs.
// Node path: the node holds a plane (three floats, offset d) at +0..+12 and child indices at +16 and +20. Two points are read from V: P at V+20 and Q at V+32 (three floats each).
// dP and dQ are ((x0*n0 + x1*n1) + x2*n2) + d. The tolerance is e = 0.03125 (0x3d000000).
//  - e < dP and e < dQ: query the child at +16 and return 1 when its result is nonzero, else 0.
//  - dP < -e and dQ < -e: the same with the child at +20.
//  - otherwise both sides may hold a hit. If 0 < dP the child +16 is queried first, then (with dP and dQ recomputed from V after the call) the child +20 is queried when dP < e or dQ < e.
//    If dP <= 0 the child +20 is queried first and the child +16 second when -e < dP or -e < dQ. The two queries are both run when their conditions hold; the result is 1 if either returned nonzero, else 0.
// Inferred: a segment-versus-BSP walk (P to Q) collecting zone hits into V; the recomputation after the first query is real in the original, so the routine tolerates a query that moves V's points.
// Unknown: V's field names and what a zone hit writes back.
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
int Collide__6VIZoneR18VISceneCollRayVars(unsigned char* zone, unsigned char* vars);

namespace {
inline float Side(const float* plane, const float* point) {
    return point[0] * plane[0] + point[1] * plane[1] + point[2] * plane[2] + plane[3];
}
}

int Collide__7VIWorldiR18VISceneCollRayVars(unsigned char* self, int index, unsigned char* vars) {
    unsigned tree = static_cast<unsigned>(index);
    if ((tree & 0xf0000000u) != 0) {
        if ((tree & 0x40000000u) != 0) {
            unsigned char* numbers = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 64));
            unsigned char* zones = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 76));
            const int zone = *reinterpret_cast<int*>(numbers + 4 * (tree & 0x0fffffffu));
            return Collide__6VIZoneR18VISceneCollRayVars(zones + 592 * zone, vars);
        }
        tree &= 0x0fffffffu;
    }
    const float* start = reinterpret_cast<const float*>(vars + 20);
    const float* finish = reinterpret_cast<const float*>(vars + 32);
    const float* plane = reinterpret_cast<const float*>(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 56)) + 24 * tree);
    const float dP = Side(plane, start);
    const float dQ = Side(plane, finish);
    const float epsilon = 0.03125f;
    if (epsilon < dP && epsilon < dQ) {
        return Collide__7VIWorldiR18VISceneCollRayVars(self, *reinterpret_cast<const int*>(plane + 4), vars) != 0;
    }
    if (dP < -epsilon && dQ < -epsilon) {
        return Collide__7VIWorldiR18VISceneCollRayVars(self, *reinterpret_cast<const int*>(plane + 5), vars) != 0;
    }
    int hit;
    if (0.0f < dP) {
        hit = Collide__7VIWorldiR18VISceneCollRayVars(self, *reinterpret_cast<const int*>(plane + 4), vars) != 0;
        plane = reinterpret_cast<const float*>(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 56)) + 24 * tree);
        if (Side(plane, start) < epsilon || Side(plane, finish) < epsilon) {
            if (Collide__7VIWorldiR18VISceneCollRayVars(self, *reinterpret_cast<const int*>(plane + 5), vars) != 0) hit = 1;
        }
        return hit;
    }
    hit = Collide__7VIWorldiR18VISceneCollRayVars(self, *reinterpret_cast<const int*>(plane + 5), vars) != 0;
    plane = reinterpret_cast<const float*>(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 56)) + 24 * tree);
    if (-epsilon < Side(plane, start) || -epsilon < Side(plane, finish)) {
        if (Collide__7VIWorldiR18VISceneCollRayVars(self, *reinterpret_cast<const int*>(plane + 4), vars) != 0) hit = 1;
    }
    return hit;
}
}
