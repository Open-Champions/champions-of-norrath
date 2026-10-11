// Observed amxStartDialog__FP5__amxPi at 0x00139020, 168 bytes. Cell 1 is
// an AMX string. It formats "%s.dbin" from that name and "%s.lmp" from
// levelName byte 81, loads the lump, and finds the dbin. The pointer word
// at the found header's byte 4 is passed to DLG_StartDialog with the name
// and cells 2 and 3. The native returns 0. The original name, dbin, and
// lump slots are 160, 128, and 80 bytes. amx_GetAddr's stack slot is zeroed
// first; the original slot was left uninitialized when that call failed.
extern "C" int amx_GetAddr(void* amx, int address, int** physical);
extern "C" int amx_GetString(char* dest, int* source);
extern "C" char levelName[];
extern "C" int sprintf(char* dest, const char* format, ...);
extern "C" void* lumpLoad__FPc(char* path);
extern "C" void* lumpFind__FPvPc(void* loaded, char* name);
extern "C" void DLG_StartDialog__FPcP9DlgPacketii(char* name, void* packet, int first, int second);

namespace {
void* load_pointer(const unsigned char* bytes) {
    void* value = nullptr;
    auto* out = reinterpret_cast<unsigned char*>(&value);
    for (unsigned i = 0; i < sizeof value; ++i) out[i] = bytes[i];
    return value;
}
}

extern "C" int amxStartDialog__FP5__amxPi(void* amx, int* params) {
    char name[160];
    char dbin[128];
    char lump_name[80];
    int* physical = nullptr;
    amx_GetAddr(amx, params[1], &physical);
    amx_GetString(name, physical);
    sprintf(dbin, "%s.dbin", name);
    sprintf(lump_name, "%s.lmp", levelName + 81);
    void* loaded = lumpLoad__FPc(lump_name);
    auto* found = static_cast<unsigned char*>(lumpFind__FPvPc(loaded, dbin));
    DLG_StartDialog__FPcP9DlgPacketii(name, load_pointer(found + 4), params[2], params[3]);
    return 0;
}
