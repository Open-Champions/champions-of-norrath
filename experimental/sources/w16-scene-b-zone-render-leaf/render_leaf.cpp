// Observed: RenderLeaf__6VIZoneiR15VISceneRendVars at 0x01119330 is 128 bytes.
// Arguments: the zone, a leaf number and the render vars. The zone's word at +80 is a
// table of 8-byte entries; the first word of entry [leaf] is a room index. The room
// record is at (zone word +96) + 120 * room index. The first word of the vars is a
// stamp. If the record's word +32 already equals the stamp it returns 0. Otherwise it
// stores the stamp there; then, if bit 0 of the record's flag word (+0) is clear and
// the vars word +44 is non-zero, it returns 0 without rendering. Else it calls
// RenderRoom__6VIZoneiR15VISceneRendVars(zone, room index, vars) and returns -1 on a
// negative result or 0 otherwise.
// Inferred: a per-frame visit stamp prevents rendering a room twice; the +44 word and
// flag bit 0 name some "force"/"skip" condition. Unknown: their names and sources.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif

extern "C" {
int RenderRoom__6VIZoneiR15VISceneRendVars(unsigned char* zone, int room, unsigned char* vars);

int RenderLeaf__6VIZoneiR15VISceneRendVars(unsigned char* zone, int leaf, unsigned char* vars) {
    unsigned* entry = reinterpret_cast<unsigned*>(at(*reinterpret_cast<unsigned*>(zone + 80)) + leaf * 8);
    int stamp = *reinterpret_cast<int*>(vars);
    int index = static_cast<int>(entry[0]);
    unsigned char* room = at(*reinterpret_cast<unsigned*>(zone + 96)) + index * 120;
    if (*reinterpret_cast<int*>(room + 32) == stamp)
        return 0;
    unsigned flags = *reinterpret_cast<unsigned*>(room);
    *reinterpret_cast<int*>(room + 32) = stamp;
    if (!(flags & 1u) && *reinterpret_cast<int*>(vars + 44) != 0)
        return 0;
    return RenderRoom__6VIZoneiR15VISceneRendVars(zone, index, vars) < 0 ? -1 : 0;
}
}
