// Observed dropItem__4itemG6Point3Pc at 0x00100ca0, 384 bytes, pinned
// SLUS_205.65. Arguments: a pointer to a 12-byte point (the by-value copy
// made by the caller) and a name string.
// Observed: the point is copied locally. A 72-byte-class description buffer
// receives only these stores: bytes 32 and 33 = 0xff, halfwords at bytes 16
// through 30 (eight halfwords) = 0, and halfwords at bytes 36, 44, 52 and 60 =
// 0xffff. Every other byte is left as incoming stack state. It then calls
// create__4itemPcPQ24item11descriptionii(name, buffer, -1, 0) and
// blockAlloc__Fi(584). If the block is null nothing else happens. Otherwise
// the spawn point is (x + 0, y, z + 36.0f). getProxyMeshId is called twice on
// the buffer: the first result selects word 0 and the second selects word 1 of
// a 16-byte proxy-table record at 0x31f540. The PhProp constructor
// (__6PhPropG6Point3PcN22liPQ24item11descriptionf) gets the block, the spawn
// point by reference, the unnamed string "equipmnt.lmp" at 0x454330, those two
// table words, 1, 1089, the buffer and 1.0f in $f12.
// Inferred: the two table words are model and texture strings, the 72-byte
// extent follows the sibling authored sources, and the constructor result is
// ignored. Not executed on hardware; the PhProp constructor and the item
// creation semantics are other packages.
#ifdef NORRATH_HOST_CONTRACT
extern unsigned char* norrath_host_base;
#define NORRATH_PTR(w) (norrath_host_base + (w))
#else
#define NORRATH_PTR(w) reinterpret_cast<unsigned char*>(w)
#endif
extern "C" {
extern unsigned norrath_player_view_31f540[];
void create__4itemPcPQ24item11descriptionii(char* name, void* description, int kind, int flags);
void* blockAlloc__Fi(int bytes);
int getProxyMeshId__4itemPQ24item11description(void* description);
void __6PhPropG6Point3PcN22liPQ24item11descriptionf(void* block, float* point, char* a, char* b, char* c,
                                                    long kind, int type, void* description, float scale);
}

extern "C" void dropItem__4itemG6Point3Pc(const float* position, char* name) {
    float where[3] = {position[0], position[1], position[2]};
    unsigned char description[72];
    for (int offset = 36; offset <= 60; offset += 8) {
        description[offset] = 0xff;
        description[offset + 1] = 0xff;
    }
    description[33] = 0xff;
    description[32] = 0xff;
    for (int offset = 16; offset < 32; ++offset) description[offset] = 0;
    create__4itemPcPQ24item11descriptionii(name, description, -1, 0);
    void* block = blockAlloc__Fi(584);
    if (block == nullptr) return;
    float lift[3] = {0.0f, 0.0f, 36.0f};
    float spawn[3] = {where[0] + lift[0], where[1], where[2] + lift[2]};
    int first = getProxyMeshId__4itemPQ24item11description(description);
    char* model = reinterpret_cast<char*>(NORRATH_PTR(norrath_player_view_31f540[first * 4]));
    int second = getProxyMeshId__4itemPQ24item11description(description);
    char* texture = reinterpret_cast<char*>(NORRATH_PTR(norrath_player_view_31f540[second * 4 + 1]));
    __6PhPropG6Point3PcN22liPQ24item11descriptionf(block, spawn, const_cast<char*>("equipmnt.lmp"), model,
                                                   texture, 1, 1089, description, 1.0f);
}
