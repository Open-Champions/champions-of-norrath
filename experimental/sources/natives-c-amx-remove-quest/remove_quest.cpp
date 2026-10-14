// Observed amxRemoveQuest__FP5__amxPi at 0x0013afc0, 56 bytes. Cell 1 is
// an AMX string. The native copies it into a 160-byte buffer and passes
// that buffer to removeQuestFromLog. It returns 0.
extern "C" int amx_GetAddr(void* amx, int address, int** physical);
extern "C" int amx_GetString(char* dest, int* source);
extern "C" void removeQuestFromLog__FPc(char* name);

extern "C" int amxRemoveQuest__FP5__amxPi(void* amx, int* params) {
    char name[160];
    int* physical = nullptr;
    amx_GetAddr(amx, params[1], &physical);
    amx_GetString(name, physical);
    removeQuestFromLog__FPc(name);
    return 0;
}
