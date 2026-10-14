// Observed amxShowMonster__FP5__amxPi at 0x00139698, 148 bytes. Cell 1 is
// an AMX string. Up to 512 objects are collected. Each object is passed to
// objectUpdateInGrid. When its word at byte 12 has bit 5 set, bit 3 of the
// byte at object byte 570 is cleared. The native returns 0.
extern "C" int amx_GetAddr(void* amx, int address, int** physical);
extern "C" int amx_GetString(char* dest, int* source);
extern "C" void objectFindObject__FPcPP10GameObjecti(char* name, void** objects, int count);
extern "C" void objectUpdateInGrid__FP10GameObject(void* object);

namespace {
int load_int(const unsigned char* bytes) {
    int value;
    auto* out = reinterpret_cast<unsigned char*>(&value);
    out[0] = bytes[0];
    out[1] = bytes[1];
    out[2] = bytes[2];
    out[3] = bytes[3];
    return value;
}
}

extern "C" int amxShowMonster__FP5__amxPi(void* amx, int* params) {
    char name[160];
    void* objects[513];
    int* physical = nullptr;
    amx_GetAddr(amx, params[1], &physical);
    amx_GetString(name, physical);
    objects[0] = nullptr;
    objectFindObject__FPcPP10GameObjecti(name, objects, 512);
    void** cursor = objects;
    while (*cursor != nullptr) {
        auto* object = static_cast<unsigned char*>(*cursor);
        objectUpdateInGrid__FP10GameObject(object);
        cursor += 1;
        if (load_int(object + 12) & 0x20) object[570] = static_cast<unsigned char>(object[570] & 0xf7);
    }
    return 0;
}
