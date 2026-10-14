// Observed setResource__10GameObjectPQ210GameObject11resourceMap at 0x00143bb0, 136 bytes. Stores
// the map pointer in the object at +36 (also when it is null), then does nothing more for a null
// map. The map is a record: word 0 is a lump name, word 1 a frame stamp, followed by entries of two
// words (a resource name, then the resolved resource) starting at +8 and ended by an entry whose
// name is null. When the stamp equals the global resourceFrame nothing else happens. Otherwise,
// for each entry in order, the resource word is set to lumpFindResource__FPcT0(lump, name), and
// after the last entry (or at once for an empty list) the stamp is set to the current
// resourceFrame. The image reloads the map pointer from the object after every lookup, so a
// lookup that changed object+36 would redirect later stores; the authored code does the same.
// Inferred: the stamp makes lookups repeat only after resourceFrame has advanced (the sibling
// get__11ResourceMap caches the same way). Unknown: the map's class beyond this layout.
// Nothing here ran on the EE.
extern "C" {
extern int resourceFrame;
void* lumpFindResource__FPcT0(char* lump, char* name);
}

namespace {
struct Entry {
    char* name;
    void* resource;
};
struct Map {
    char* lump;
    int frame;
    Entry entries[1];
};
}  // namespace

extern "C" void setResource__10GameObjectPQ210GameObject11resourceMap(void* object, void* map_pointer) {
    unsigned char* self = static_cast<unsigned char*>(object);
    Map* map = static_cast<Map*>(map_pointer);
    __builtin_memcpy(self + 36, &map, sizeof map);
    if (map == nullptr) return;
    if (map->frame == resourceFrame) return;
    if (map->entries[0].name != nullptr) {
        int i = 0;
        do {
            void* found = lumpFindResource__FPcT0(map->lump, map->entries[i].name);
            __builtin_memcpy(&map, self + 36, sizeof map);
            map->entries[i].resource = found;
            ++i;
        } while (map->entries[i].name != nullptr);
    }
    map->frame = resourceFrame;
}
