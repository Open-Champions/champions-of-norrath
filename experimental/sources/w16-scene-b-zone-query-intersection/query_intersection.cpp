// Observed: QueryIntersection__6VIZoneR20VISceneIntersectVars at 0x01118e70 is 136
// bytes. Arguments: the zone and the intersect vars. If the zone's count word at +84
// is positive it calls SetPreTranslations__9VICollideiP7VIVect3(vars word +36, count,
// zone word +88) and a negative result returns -1. It then calls the sibling
// QueryIntersection__6VIZoneiR20VISceneIntersectVars(zone, node, vars), with node 0
// when the zone word +68 is non-zero and 0x40000000 when it is zero, and returns -1
// on a negative result or 0 otherwise.
// Inferred: the pre-translation array is set up once for the collide object, then the
// tree walk starts at the root (node 0) or, for a zone without a tree, at a leaf-flag
// node. 0x40000000 matches the leaf flag RenderByVolumeNode tests. Unknown: the
// zone words' names.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif

extern "C" {
int SetPreTranslations__9VICollideiP7VIVect3(void* collide, int count, void* translations);
int QueryIntersection__6VIZoneiR20VISceneIntersectVars(unsigned char* zone, int node, unsigned char* vars);

int QueryIntersection__6VIZoneR20VISceneIntersectVars(unsigned char* zone, unsigned char* vars) {
    int count = *reinterpret_cast<int*>(zone + 84);
    if (count > 0) {
        if (SetPreTranslations__9VICollideiP7VIVect3(at(*reinterpret_cast<unsigned*>(vars + 36)), count, at(*reinterpret_cast<unsigned*>(zone + 88))) < 0)
            return -1;
    }
    int node = *reinterpret_cast<int*>(zone + 68) != 0 ? 0 : 0x40000000;
    return QueryIntersection__6VIZoneiR20VISceneIntersectVars(zone, node, vars) < 0 ? -1 : 0;
}
}
