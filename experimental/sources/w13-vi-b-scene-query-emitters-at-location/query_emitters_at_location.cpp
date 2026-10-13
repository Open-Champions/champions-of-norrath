// Observed: QueryEmittersAtLocation__7VISceneRC8VICameraR14VIEmitterQuery at 0x01143518, 252
// bytes. It calls FindLeaf__C7VIWorldRC7VIVect3RiT2(scene + 4, camera + 28, &a, &b); if that
// returns a negative value it returns -1. Otherwise it walks a chain: the scene pointer at
// +80, element a (592 bytes each), its pointer at +80, entry b (8 bytes each) whose first word
// is an index k; the same record's pointer at +96, element k (120 bytes each), whose pointer
// at +104 starts a linked list (the end marker is the all-ones word). For each list node it
// calls QueryEmittersAtLocationActor__7VISceneiRC8VICameraR14VIEmitterQuery(scene, node word
// +4, camera, query); a negative result makes it return -1 at once. The next node is the
// pointer at node + 16. After the list ends it increments the scene word at +1348 and returns 0.
// Inferred: the camera's position at +28 is looked up in the world to find its leaf, whose
// per-leaf actor list is scanned; the scene word at +1348 counts completed queries (the same
// counter the Collide and QueryIntersection wrappers raise).
// Unknown: the member names, the record types and the meaning of the node word at +4 (an
// actor index is assumed from the callee's name and int parameter).
#if defined(NORRATH_HOST_CONTRACT)
// On the 64-bit host a pointer field is modelled as a 32-bit offset into an arena.
extern "C" unsigned char* norrath_host_load_pointer(const void* field);
extern "C" unsigned char* norrath_host_link_end();
#define NORRATH_LOAD_POINTER(field) norrath_host_load_pointer(field)
#define NORRATH_LINK_END() norrath_host_link_end()
#else
#define NORRATH_LOAD_POINTER(field) (*reinterpret_cast<unsigned char* const*>(field))
#define NORRATH_LINK_END() reinterpret_cast<unsigned char*>(-1)
#endif

extern "C" {
int FindLeaf__C7VIWorldRC7VIVect3RiT2(const void* world, const void* position, int* leaf, int* sub);
int QueryEmittersAtLocationActor__7VISceneiRC8VICameraR14VIEmitterQuery(void* scene, int actor, const void* camera, void* query);

int QueryEmittersAtLocation__7VISceneRC8VICameraR14VIEmitterQuery(void* scene, const void* camera, void* query) {
    unsigned char* s = static_cast<unsigned char*>(scene);
    int found[2];
    if (FindLeaf__C7VIWorldRC7VIVect3RiT2(s + 4, static_cast<const unsigned char*>(camera) + 28, &found[0], &found[1]) < 0) return -1;
    unsigned char* leaf = NORRATH_LOAD_POINTER(s + 80) + found[0] * 592;
    unsigned char* entry = NORRATH_LOAD_POINTER(leaf + 80) + found[1] * 8;
    unsigned char* record = NORRATH_LOAD_POINTER(leaf + 96) + *reinterpret_cast<int*>(entry) * 120;
    unsigned char* node = NORRATH_LOAD_POINTER(record + 104);
    if (node != NORRATH_LINK_END()) {
        do {
            if (QueryEmittersAtLocationActor__7VISceneiRC8VICameraR14VIEmitterQuery(scene, *reinterpret_cast<int*>(node + 4), camera, query) < 0) return -1;
            node = NORRATH_LOAD_POINTER(node + 16);
        } while (node != NORRATH_LINK_END());
    }
    *reinterpret_cast<int*>(s + 1348) += 1;
    return 0;
}
}
