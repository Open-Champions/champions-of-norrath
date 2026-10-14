// Observed amxStopFollowingPath__FP5__amxPi at 0x00139730, 172 bytes. Cell 1
// is an AMX string. For each collected object whose word at byte 12 has
// bit 5 set, bit 27 of the 8-byte word at byte 160 is cleared and the 12
// bytes at byte 16 are copied to byte 776. The native returns 0.
extern "C" int amx_GetAddr(void* amx, int address, int** physical);
extern "C" int amx_GetString(char* dest, int* source);
extern "C" void objectFindObject__FPcPP10GameObjecti(char* name, void** objects, int count);

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

unsigned long long load_wide(const unsigned char* bytes) {
    unsigned long long value = 0;
    auto* out = reinterpret_cast<unsigned char*>(&value);
    for (int i = 0; i < 8; ++i) out[i] = bytes[i];
    return value;
}

void store_wide(unsigned char* bytes, unsigned long long value) {
    auto* in = reinterpret_cast<const unsigned char*>(&value);
    for (int i = 0; i < 8; ++i) bytes[i] = in[i];
}
}

extern "C" int amxStopFollowingPath__FP5__amxPi(void* amx, int* params) {
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
        const int flags = load_int(object + 12);
        cursor += 1;
        if ((flags & 0x20) == 0) continue;
        store_wide(object + 160, load_wide(object + 160) & ~0x08000000ull);
        for (int i = 0; i < 12; ++i) object[776 + i] = object[16 + i];
    }
    return 0;
}
