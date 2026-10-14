// Observed amxSetInfo__FP5__amxPi at 0x0013ae58, 76 bytes. Cell 1 is an
// AMX string. textGetString translates the 160-byte copy, and
// hudSetInfoMessage receives that pointer, 180, -1, and 0. The native
// returns 0.
extern "C" int amx_GetAddr(void* amx, int address, int** physical);
extern "C" int amx_GetString(char* dest, int* source);
extern "C" unsigned short* textGetString__FPc(char* key);
extern "C" void hudSetInfoMessage__FPUsiic(unsigned short* text, int duration, int color, int channel);

extern "C" int amxSetInfo__FP5__amxPi(void* amx, int* params) {
    char key[160];
    int* physical = nullptr;
    amx_GetAddr(amx, params[1], &physical);
    amx_GetString(key, physical);
    hudSetInfoMessage__FPUsiic(textGetString__FPc(key), 180, -1, 0);
    return 0;
}
