// Observed amxHideMonster at 0x00139520, 128 bytes. It fills a
// 512-pointer list from the first AMX cell. Each found object is
// removed from the grid. When the word at object+12 has bit 0x20
// set, byte 570 is ORed with 8. The list pointer advances either way.
extern "C" int amx_GetAddr(void* amx, int address, void** physical);
extern "C" int amx_GetString(char* destination, void* cells);
extern "C" void objectFindObject__FPcPP10GameObjecti(char* name, void** list, int limit);
extern "C" void objectRemoveFromGrid__FP10GameObject(void* object);

static int word_at(const unsigned char* bytes) {
    return static_cast<int>(static_cast<unsigned>(bytes[0])
        | (static_cast<unsigned>(bytes[1]) << 8)
        | (static_cast<unsigned>(bytes[2]) << 16)
        | (static_cast<unsigned>(bytes[3]) << 24));
}

extern "C" int amxHideMonster__FP5__amxPi(void* amx, int* params) {
    char text[160];
    void* cells = nullptr;
    void* found[512];
    (void)amx_GetAddr(amx, params[1], &cells);
    (void)amx_GetString(text, cells);
    objectFindObject__FPcPP10GameObjecti(text, found, 512);
    for (void** slot = found; *slot != nullptr; ++slot) {
        unsigned char* object = static_cast<unsigned char*>(*slot);
        objectRemoveFromGrid__FP10GameObject(object);
        object = static_cast<unsigned char*>(*slot);
        if ((word_at(object + 12) & 0x20) != 0) object[570] = static_cast<unsigned char>(object[570] | 0x8);
    }
    return 0;
}
