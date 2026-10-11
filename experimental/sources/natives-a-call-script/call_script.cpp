// amxCallScript is the 52-byte native at 0x0013ae20. Cell 1 is an AMX
// string. amx_GetAddr resolves it and amx_GetString copies it into a 160-byte
// buffer. scriptStart receives that buffer. This native returns scriptStart's
// result.
extern "C" int amx_GetAddr(void* amx, int address, int** physical);
extern "C" int amx_GetString(char* destination, int* source);
extern "C" int scriptStart__FPc(char* name);
extern "C" int amxCallScript__FP5__amxPi(void* amx, int* params) {
    int* cell = 0;
    amx_GetAddr(amx, params[1], &cell);
    char name[160];
    amx_GetString(name, cell);
    return scriptStart__FPc(name);
}
