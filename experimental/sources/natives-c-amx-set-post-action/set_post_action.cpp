// Observed amxSetPostAction__FP5__amxPi at 0x0013b578, 56 bytes. Cell 1 is
// an AMX string. The 160-byte copy is passed to scriptSetPostAction.
// The native returns 0.
extern "C" int amx_GetAddr(void* amx, int address, int** physical);
extern "C" int amx_GetString(char* dest, int* source);
extern "C" void scriptSetPostAction__FPc(char* text);

extern "C" int amxSetPostAction__FP5__amxPi(void* amx, int* params) {
    char text[160];
    int* physical = nullptr;
    amx_GetAddr(amx, params[1], &physical);
    amx_GetString(text, physical);
    scriptSetPostAction__FPc(text);
    return 0;
}
