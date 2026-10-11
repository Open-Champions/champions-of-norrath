// Observed amxMoveMonster at 0x001395a0, 244 bytes. The first AMX cell
// fills a 512-pointer list. The second names one destination. A null
// destination or an empty list returns zero. Otherwise each object
// copies 12 bytes from destination+16 and the halfword at destination+56,
// then objectUpdateInGrid runs.
extern "C" int amx_GetAddr(void* amx, int address, void** physical);
extern "C" int amx_GetString(char* destination, void* cells);
extern "C" void objectFindObject__FPcPP10GameObjecti(char* name, void** list, int limit);
extern "C" void* objectFindObject__FPc(char* name);
extern "C" void objectUpdateInGrid__FP10GameObject(void* object);

extern "C" int amxMoveMonster__FP5__amxPi(void* amx, int* params) {
    char name[160];
    char destination_name[160];
    void* cells = nullptr;
    void* found[512];
    (void)amx_GetAddr(amx, params[1], &cells);
    (void)amx_GetString(name, cells);
    objectFindObject__FPcPP10GameObjecti(name, found, 512);
    (void)amx_GetAddr(amx, params[2], &cells);
    (void)amx_GetString(destination_name, cells);
    unsigned char* destination = static_cast<unsigned char*>(objectFindObject__FPc(destination_name));
    if (destination == nullptr || found[0] == nullptr) return 0;
    for (void** slot = found; *slot != nullptr; ++slot) {
        unsigned char* object = static_cast<unsigned char*>(*slot);
        __builtin_memcpy(object + 16, destination + 16, 12);
        __builtin_memcpy(object + 56, destination + 56, 2);
        objectUpdateInGrid__FP10GameObject(object);
    }
    return 0;
}
