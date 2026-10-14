// amxAddDeferedCall is the 56-byte native at 0x0013b728. Cell 1 is an AMX
// string, copied through amx_GetAddr and amx_GetString into a 160-byte
// buffer and passed to scriptAddDeferedCall. The native returns zero.
extern "C" int amx_GetAddr(void* amx, int address, int** physical);
extern "C" int amx_GetString(char* destination, int* source);
extern "C" void scriptAddDeferedCall__FPc(char* name);
extern "C" int amxAddDeferedCall__FP5__amxPi(void* amx, int* params) {
    int* cell = 0;
    amx_GetAddr(amx, params[1], &cell);
    char name[160];
    amx_GetString(name, cell);
    scriptAddDeferedCall__FPc(name);
    return 0;
}
