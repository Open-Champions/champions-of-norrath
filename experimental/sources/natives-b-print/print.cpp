// Observed amxPrint at 0x0013ade8, 48 bytes. It resolves the first
// AMX cell and copies that script string into a 160-byte temporary.
// The temporary is not passed on. The native returns zero.
extern "C" int amx_GetAddr(void* amx, int address, void** physical);
extern "C" int amx_GetString(char* destination, void* cells);

extern "C" int amxPrint__FP5__amxPi(void* amx, int* params) {
    char text[160];
    void* cells = nullptr;
    (void)amx_GetAddr(amx, params[1], &cells);
    (void)amx_GetString(text, cells);
    return 0;
}
