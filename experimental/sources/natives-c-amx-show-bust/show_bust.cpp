// Observed amxShowBust__FP5__amxPi at 0x0013a880, 152 bytes. Cells 1 and 2
// are AMX strings. textGetString translates the second. hudShowBust
// receives the first copy, that text, cell 3, and cell 4. The native
// returns 0.
extern "C" int amx_GetAddr(void* amx, int address, int** physical);
extern "C" int amx_GetString(char* dest, int* source);
extern "C" unsigned short* textGetString__FPc(char* key);
extern "C" void hudShowBust__FPcPUsii(char* name, unsigned short* text, int x, int y);

extern "C" int amxShowBust__FP5__amxPi(void* amx, int* params) {
    char name[160];
    char key[160];
    int* physical = nullptr;
    amx_GetAddr(amx, params[1], &physical);
    amx_GetString(name, physical);
    amx_GetAddr(amx, params[2], &physical);
    amx_GetString(key, physical);
    hudShowBust__FPcPUsii(name, textGetString__FPc(key), params[3], params[4]);
    return 0;
}
