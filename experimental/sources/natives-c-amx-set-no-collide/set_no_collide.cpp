// Observed amxSetNoCollide__FP5__amxPi at 0x0013c198, 112 bytes. Cell 1 is
// an AMX string and cell 2 selects the bit. A nonzero cell sets bit 0 of
// the word at object byte 12. A zero cell clears it. The native returns 0.
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

void store_int(unsigned char* bytes, int value) {
    auto* in = reinterpret_cast<const unsigned char*>(&value);
    bytes[0] = in[0];
    bytes[1] = in[1];
    bytes[2] = in[2];
    bytes[3] = in[3];
}
}

extern "C" int amxSetNoCollide__FP5__amxPi(void* amx, int* params) {
    char name[160];
    int* physical = nullptr;
    amx_GetAddr(amx, params[1], &physical);
    amx_GetString(name, physical);
    auto* object = static_cast<unsigned char*>(objectFindObject__FPc(name));
    int flags = load_int(object + 12);
    if (params[2] != 0) flags |= 1;
    else flags &= ~1;
    store_int(object + 12, flags);
    return 0;
}
