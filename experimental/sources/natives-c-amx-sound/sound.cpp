// Observed amxSound__FP5__amxPi at 0x0013b0d8, 164 bytes. Cells 1 and 2 are
// AMX strings. lumpFindResource looks up that pair. A non-null header is
// played at no point with floats 1, 1, and 0. A null header is not played.
// The native returns 0.
extern "C" int amx_GetAddr(void* amx, int address, int** physical);
extern "C" int amx_GetString(char* dest, int* source);
extern "C" void* lumpFindResource__FPcT0(char* group, char* name);
extern "C" void soundPlaySound__FP10_vagHeaderP6Point3fff(void* header, const float* point, float volume, float pitch, float unused);

extern "C" int amxSound__FP5__amxPi(void* amx, int* params) {
    char group[160];
    char name[160];
    int* physical = nullptr;
    amx_GetAddr(amx, params[1], &physical);
    amx_GetString(group, physical);
    amx_GetAddr(amx, params[2], &physical);
    amx_GetString(name, physical);
    void* header = lumpFindResource__FPcT0(group, name);
    if (header != nullptr) soundPlaySound__FP10_vagHeaderP6Point3fff(header, nullptr, 1.f, 1.f, 0.f);
    return 0;
}
