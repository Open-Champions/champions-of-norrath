// Observed amxUnFriendly__FP5__amxPi at 0x00139880, 164 bytes. Cell 1 is an
// AMX string. For each collected object whose word at byte 12 has bit 5
// set, bit 24 of that word is cleared and bit 26 of the 8-byte word at
// byte 160 is cleared. The native returns 0.
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

void store_int(unsigned char* bytes, int value) {
    auto* in = reinterpret_cast<const unsigned char*>(&value);
    bytes[0] = in[0];
    bytes[1] = in[1];
    bytes[2] = in[2];
    bytes[3] = in[3];
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

extern "C" int amxUnFriendly__FP5__amxPi(void* amx, int* params) {
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
        store_int(object + 12, flags & ~0x01000000);
        store_wide(object + 160, load_wide(object + 160) & ~0x04000000ull);
    }
    return 0;
}
