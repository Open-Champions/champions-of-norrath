// Observed amxSetEntrance__FP5__amxPi at 0x0013aa28, 64 bytes. Cell 1 is
// an AMX string. strcpy copies the 160-byte buffer onto startEntranceName.
// The native returns 0.
extern "C" int amx_GetAddr(void* amx, int address, int** physical);
extern "C" int amx_GetString(char* dest, int* source);
extern "C" char* strcpy(char* dest, const char* source);
extern "C" char startEntranceName[];

extern "C" int amxSetEntrance__FP5__amxPi(void* amx, int* params) {
    char name[160];
    int* physical = nullptr;
    amx_GetAddr(amx, params[1], &physical);
    amx_GetString(name, physical);
    strcpy(startEntranceName, name);
    return 0;
}
