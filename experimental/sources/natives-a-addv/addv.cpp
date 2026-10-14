// amxAddv is the 88-byte native at 0x0013b080. Cell 1 is the state name and
// cell 2 is added, with 32-bit wrap, to scriptGetGameState. The sum is stored
// with scriptSetGameState and returned.
extern "C" int amx_GetAddr(void* amx, int address, int** physical);
extern "C" int amx_GetString(char* destination, int* source);
extern "C" int scriptGetGameState__FPc(char* name);
extern "C" void scriptSetGameState__FPci(char* name, int value);
extern "C" int amxAddv__FP5__amxPi(void* amx, int* params) {
    int* cell = 0;
    amx_GetAddr(amx, params[1], &cell);
    char name[160];
    amx_GetString(name, cell);
    unsigned sum = static_cast<unsigned>(scriptGetGameState__FPc(name));
    sum += static_cast<unsigned>(params[2]);
    scriptSetGameState__FPci(name, static_cast<int>(sum));
    return static_cast<int>(sum);
}
