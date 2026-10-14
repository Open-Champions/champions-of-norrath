// Observed: AddNode__t10VIMultiMap2ZfZ14VIEmitterEntryii at 0x011f4368 is 252 bytes. With (map,
// parent, side) it takes an element off the map's free chain and links it into the tree. The map
// holds a used count at +0, a capacity at +4, the element array base at +8 (elements are 112
// bytes), the root index at +12 and the free-chain head at +16. If the head is -1 it doubles the
// capacity (2 when the capacity is zero), calls Allocate__t10VIMultiMap2ZfZ14VIEmitterEntry(map),
// returns -1 when that is negative, and re-reads the head. The new index n is the head; the head
// becomes the element's +4 word. The element gets +8 = 0, +0 = 1, +4 = parent and +12 = 0. With
// a nonzero parent, the parent's +8 word is set to n when side is nonzero, otherwise its +12
// word; with parent 0 the root word becomes n. The used count is incremented and n is returned.
// Inferred: element +0 is the colour (1 red), +4 parent, +8 left, +12 right; free elements chain
// through +4. The parent/side contract is as used by the insert routines of the sibling maps.
// Unknown: the template parameters beyond the mangled name.
extern "C" {
int Allocate__t10VIMultiMap2ZfZ14VIEmitterEntry(void* map);

static inline unsigned* word(void* map, unsigned offset) {
    return reinterpret_cast<unsigned*>(static_cast<unsigned char*>(map) + offset);
}
static inline unsigned* field(void* map, int index, unsigned offset) {
    unsigned char* elements = reinterpret_cast<unsigned char*>(
        static_cast<__UINTPTR_TYPE__>(*word(map, 8)));
    return reinterpret_cast<unsigned*>(elements + index * 112 + offset);
}

int AddNode__t10VIMultiMap2ZfZ14VIEmitterEntryii(void* map, int parent, int side) {
    if (static_cast<int>(*word(map, 16)) == -1) {
        const unsigned capacity = *word(map, 4);
        *word(map, 4) = capacity != 0 ? capacity << 1 : 2;
        if (Allocate__t10VIMultiMap2ZfZ14VIEmitterEntry(map) < 0) return -1;
    }
    const int created = static_cast<int>(*word(map, 16));
    *word(map, 16) = *field(map, created, 4);
    *field(map, created, 8) = 0;
    *field(map, created, 0) = 1;
    *field(map, created, 4) = static_cast<unsigned>(parent);
    *field(map, created, 12) = 0;
    if (parent != 0) {
        if (side != 0) {
            *field(map, parent, 8) = static_cast<unsigned>(created);
        } else {
            *field(map, parent, 12) = static_cast<unsigned>(created);
        }
    } else {
        *word(map, 12) = static_cast<unsigned>(created);
    }
    *word(map, 0) += 1;
    return created;
}
}
