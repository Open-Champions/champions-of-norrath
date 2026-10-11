// Observed amxNewWorldPost at 0x0013b498, 64 bytes. It copies the
// script string from the first AMX cell into DLG_NewWorld at
// 0x0042a4c0 and returns zero.
extern "C" int amx_GetAddr(void* amx, int address, void** physical);
extern "C" int amx_GetString(char* destination, void* cells);
extern "C" char* strcpy(char* destination, const char* source);
extern "C" char DLG_NewWorld[];

extern "C" int amxNewWorldPost__FP5__amxPi(void* amx, int* params) {
    char text[160];
    void* cells = nullptr;
    (void)amx_GetAddr(amx, params[1], &cells);
    (void)amx_GetString(text, cells);
    (void)strcpy(DLG_NewWorld, text);
    return 0;
}
