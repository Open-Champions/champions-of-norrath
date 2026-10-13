// Observed: FindLeaf__C7VIWorldRC7VIVect3RiT2 at 0x010fd248 is 300 bytes. Arguments: the
// world (a0), a point of three floats x, y, z (a1) and two int output references (a2, a3).
// The world holds, at +52, a node tree: a word at +0 (zero means "no tree") and a pointer
// at +4 to an array of 24-byte nodes. A node is four floats (a plane: n0, n1, n2, d) at
// +0..+12 followed by two words, +16 and +20. Walk:
//   index = 0; if the tree word is nonzero, repeat: let node = nodes + index * 24, with the
//   index used unmasked for the plane and masked with 0x0fffffff for the child words;
//   distance = ((x*n0 + y*n1) + z*n2) + d; the next index is the word at +16 when 0.0f <=
//   distance and the word at +20 otherwise; stop once bit 0x40000000 of the next index is
//   set. Then leaf = index & 0x0fffffff (leaf = 0 when there is no tree).
// Then: zone = the int at [word at world+64] + leaf * 4; *a2 = zone; the zone object is at
// [world word +76] + zone * 592; r = FindLeaf__C6VIZoneRC7VIVect3(zone object, point);
// *a3 = r; the function returns -1 if r < 0 and 0 otherwise.
// Inferred: a BSP-like tree over the whole world with leaves mapping to zone numbers, then
// a per-zone lookup. The 592-byte stride is the zone object size. Unknown: member names.
// The same node walk appears in the zone's own FindLeaf (tree at zone+68) with the same
// mask and flag constants.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int FindLeaf__C6VIZoneRC7VIVect3(const void* zone, const float* point);

int FindLeaf__C7VIWorldRC7VIVect3RiT2(const unsigned char* world, const float* point, int* out_zone, int* out_leaf) {
    const unsigned char* tree = world + 52;
    unsigned index = 0;
    if (*reinterpret_cast<const unsigned*>(tree) != 0) {
        const unsigned char* nodes = NORRATH_WORD_TO_POINTER(*reinterpret_cast<const unsigned*>(tree + 4));
        float x = point[0], y = point[1], z = point[2];
        do {
            const float* plane = reinterpret_cast<const float*>(nodes + index * 24);
            index &= 0x0fffffffu;
            const unsigned* children = reinterpret_cast<const unsigned*>(nodes + index * 24);
            float distance = x * plane[0];
            float second = y * plane[1];
            float third = z * plane[2];
            distance = distance + second;
            distance = distance + third;
            distance = distance + plane[3];
            index = 0.0f <= distance ? children[4] : children[5];
        } while ((index & 0x40000000u) == 0);
        index &= 0x0fffffffu;
    }
    unsigned zones = *reinterpret_cast<const unsigned*>(world + 64);
    unsigned zone_objects = *reinterpret_cast<const unsigned*>(world + 76);
    int zone = *reinterpret_cast<const int*>(NORRATH_WORD_TO_POINTER(zones) + index * 4);
    *out_zone = zone;
    int result = FindLeaf__C6VIZoneRC7VIVect3(NORRATH_WORD_TO_POINTER(zone_objects) + zone * 592, point);
    *out_leaf = result;
    return result < 0 ? -1 : 0;
}
}
