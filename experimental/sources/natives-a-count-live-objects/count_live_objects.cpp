// amxCountLiveObjects is the 148-byte native at 0x0013be48. Cell 1 names
// objects, up to 128. An object counts when (flags & 0x28) is not 0x28.
// Flags are the word at byte 12. The native returns that count.
extern "C" int amx_GetAddr(void* amx, int address, int** physical);
extern "C" int amx_GetString(char* destination, int* source);
extern "C" void objectFindObject__FPcPP10GameObjecti(char* name, void** list, int limit);
static unsigned load_u32(const unsigned char* address) {
    return static_cast<unsigned>(address[0]) | (static_cast<unsigned>(address[1]) << 8) |
           (static_cast<unsigned>(address[2]) << 16) | (static_cast<unsigned>(address[3]) << 24);
}
extern "C" int amxCountLiveObjects__FP5__amxPi(void* amx, int* params) {
    int* cell = 0;
    char name[160];
    void* list[128];
    amx_GetAddr(amx, params[1], &cell);
    amx_GetString(name, cell);
    objectFindObject__FPcPP10GameObjecti(name, list, 128);
    if (list[0] == 0) return 0;
    int count = 0;
    int index = 0;
    for (;;) {
        unsigned char* object = static_cast<unsigned char*>(list[index]);
        unsigned masked = (load_u32(object + 12) & 0x28u) ^ 0x28u;
        if (masked != 0u) count += 1;
        index += 1;
        if (list[index] == 0) break;
    }
    return count;
}
