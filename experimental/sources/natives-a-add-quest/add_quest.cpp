// amxAddQuest is the 128-byte native at 0x0013af40. Cells 1 and 2 are AMX
// strings. addQuestToLog receives them in that order. The native returns zero.
extern "C" int amx_GetAddr(void* amx, int address, int** physical);
extern "C" int amx_GetString(char* destination, int* source);
extern "C" void addQuestToLog__FPcT0(char* title, char* body);
extern "C" int amxAddQuest__FP5__amxPi(void* amx, int* params) {
    int* cell = 0;
    char title[160];
    char body[160];
    amx_GetAddr(amx, params[1], &cell);
    amx_GetString(title, cell);
    amx_GetAddr(amx, params[2], &cell);
    amx_GetString(body, cell);
    addQuestToLog__FPcT0(title, body);
    return 0;
}
