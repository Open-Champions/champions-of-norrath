// Observed amxPreloadResource at 0x0013a6b0, 56 bytes. It resolves
// the first AMX cell, copies that script string, passes the copy to
// lumpLoad, and returns zero.
extern "C" int amx_GetAddr(void* amx, int address, void** physical);
extern "C" int amx_GetString(char* destination, void* cells);
extern "C" int lumpLoad__FPc(char* name);

extern "C" int amxPreloadResource__FP5__amxPi(void* amx, int* params) {
    char text[160];
    void* cells = nullptr;
    (void)amx_GetAddr(amx, params[1], &cells);
    (void)amx_GetString(text, cells);
    (void)lumpLoad__FPc(text);
    return 0;
}
