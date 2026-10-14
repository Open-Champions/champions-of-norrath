// Observed amxGetv at 0x0013aff8, 52 bytes. It resolves the first AMX
// cell, copies that script string, and returns scriptGetGameState.
extern "C" int amx_GetAddr(void* amx, int address, void** physical);
extern "C" int amx_GetString(char* destination, void* cells);
extern "C" int scriptGetGameState__FPc(char* name);

extern "C" int amxGetv__FP5__amxPi(void* amx, int* params) {
    char text[160];
    void* cells = nullptr;
    (void)amx_GetAddr(amx, params[1], &cells);
    (void)amx_GetString(text, cells);
    return scriptGetGameState__FPc(text);
}
