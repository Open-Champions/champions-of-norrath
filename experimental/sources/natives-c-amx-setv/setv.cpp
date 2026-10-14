// Observed amxSetv__FP5__amxPi at 0x0013b038, 72 bytes. Cell 1 is an AMX
// string and cell 2 is the stored word. scriptSetGameState receives both.
// The native returns 0.
extern "C" int amx_GetAddr(void* amx, int address, int** physical);
extern "C" int amx_GetString(char* dest, int* source);
extern "C" void scriptSetGameState__FPci(char* name, int value);

extern "C" int amxSetv__FP5__amxPi(void* amx, int* params) {
    char name[160];
    int* physical = nullptr;
    amx_GetAddr(amx, params[1], &physical);
    amx_GetString(name, physical);
    scriptSetGameState__FPci(name, params[2]);
    return 0;
}
