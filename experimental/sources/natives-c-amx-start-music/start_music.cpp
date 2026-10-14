// Observed amxStartMusic__FP5__amxPi at 0x0013bf90, 60 bytes. Cell 1 is an
// AMX string. soundStartMusic receives the 160-byte copy and the word 3.
// The native returns 0.
extern "C" int amx_GetAddr(void* amx, int address, int** physical);
extern "C" int amx_GetString(char* dest, int* source);
extern "C" void soundStartMusic__FPci(char* name, int mode);

extern "C" int amxStartMusic__FP5__amxPi(void* amx, int* params) {
    char name[160];
    int* physical = nullptr;
    amx_GetAddr(amx, params[1], &physical);
    amx_GetString(name, physical);
    soundStartMusic__FPci(name, 3);
    return 0;
}
