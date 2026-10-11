// amxFollowPlayer is the 156-byte native at 0x0013b868. Cell 1 names objects,
// up to 512. An object whose flags word at byte 12 has bit 0x20 gets bit
// 0x00800000 set in the 8-byte word at byte 160, then charm(object, -1).
// The native returns zero.
extern "C" int amx_GetAddr(void* amx, int address, int** physical);
extern "C" int amx_GetString(char* destination, int* source);
extern "C" void objectFindObject__FPcPP10GameObjecti(char* name, void** list, int limit);
extern "C" void charm__8Creaturei(unsigned char* object, int value);
static unsigned load_u32(const unsigned char* address) {
    return static_cast<unsigned>(address[0]) | (static_cast<unsigned>(address[1]) << 8) |
           (static_cast<unsigned>(address[2]) << 16) | (static_cast<unsigned>(address[3]) << 24);
}
static unsigned long long load_u64(const unsigned char* address) {
    unsigned long long value = 0ull;
    for (int index = 0; index < 8; ++index)
        value |= static_cast<unsigned long long>(address[index]) << (8 * index);
    return value;
}
static void store_u64(unsigned char* address, unsigned long long value) {
    for (int index = 0; index < 8; ++index)
        address[index] = static_cast<unsigned char>(value >> (8 * index));
}
extern "C" int amxFollowPlayer__FP5__amxPi(void* amx, int* params) {
    int* cell = 0;
    char name[160];
    void* list[512];
    amx_GetAddr(amx, params[1], &cell);
    amx_GetString(name, cell);
    objectFindObject__FPcPP10GameObjecti(name, list, 512);
    if (list[0] == 0) return 0;
    int index = 0;
    for (;;) {
        unsigned char* object = static_cast<unsigned char*>(list[index]);
        if ((load_u32(object + 12) & 0x20u) != 0u) {
            store_u64(object + 160, load_u64(object + 160) | 0x00800000ull);
            charm__8Creaturei(object, -1);
        }
        index += 1;
        if (list[index] == 0) break;
    }
    return 0;
}
