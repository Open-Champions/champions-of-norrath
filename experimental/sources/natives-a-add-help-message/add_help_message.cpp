// amxAddHelpMessage is the 152-byte native at 0x0013aea8. Cells 1 and 2 are
// AMX strings. Each is passed through textGetString, and those two results
// are given to hudAddHelpMessage with -1. The native returns zero.
extern "C" int amx_GetAddr(void* amx, int address, int** physical);
extern "C" int amx_GetString(char* destination, int* source);
extern "C" unsigned short* textGetString__FPc(char* key);
extern "C" void hudAddHelpMessage__FiPUsT1(int id, unsigned short* first, unsigned short* second);
extern "C" int amxAddHelpMessage__FP5__amxPi(void* amx, int* params) {
    int* cell = 0;
    char first_key[160];
    char second_key[160];
    amx_GetAddr(amx, params[1], &cell);
    amx_GetString(first_key, cell);
    amx_GetAddr(amx, params[2], &cell);
    amx_GetString(second_key, cell);
    unsigned short* first = textGetString__FPc(first_key);
    unsigned short* second = textGetString__FPc(second_key);
    hudAddHelpMessage__FiPUsT1(-1, first, second);
    return 0;
}
