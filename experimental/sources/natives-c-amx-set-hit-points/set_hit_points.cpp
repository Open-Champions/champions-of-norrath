// Observed amxSetHitPoints__FP5__amxPi at 0x0013a970, 96 bytes. Cell 1 is
// an AMX string and cell 2 is a signed word. When the object's word at
// byte 12 has bit 5 set, cell 2 is converted to a float and stored at
// byte 560. The native returns 0.
extern "C" int amx_GetAddr(void* amx, int address, int** physical);
extern "C" int amx_GetString(char* dest, int* source);
extern "C" void* objectFindObject__FPc(char* name);

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

void store_float(unsigned char* bytes, float value) {
    auto* in = reinterpret_cast<const unsigned char*>(&value);
    bytes[0] = in[0];
    bytes[1] = in[1];
    bytes[2] = in[2];
    bytes[3] = in[3];
}
}

extern "C" int amxSetHitPoints__FP5__amxPi(void* amx, int* params) {
    char name[160];
    int* physical = nullptr;
    amx_GetAddr(amx, params[1], &physical);
    amx_GetString(name, physical);
    auto* object = static_cast<unsigned char*>(objectFindObject__FPc(name));
    if (load_int(object + 12) & 0x20) store_float(object + 560, static_cast<float>(params[2]));
    return 0;
}
