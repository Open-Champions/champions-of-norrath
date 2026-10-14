// amxCountQuestItems is the 56-byte native at 0x0013baa8. Cell 1 is an AMX
// string. queryInventoryItem receives that string and a zero second argument.
// The native returns queryInventoryItem's result.
extern "C" int amx_GetAddr(void* amx, int address, int** physical);
extern "C" int amx_GetString(char* destination, int* source);
extern "C" int queryInventoryItem__FPci(char* name, int owner);
extern "C" int amxCountQuestItems__FP5__amxPi(void* amx, int* params) {
    int* cell = 0;
    amx_GetAddr(amx, params[1], &cell);
    char name[160];
    amx_GetString(name, cell);
    return queryInventoryItem__FPci(name, 0);
}
