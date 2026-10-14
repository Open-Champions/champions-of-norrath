// amxCutscene is the 140-byte native at 0x0013b5b0. Cell 1 is an AMX string.
// It builds "<levelName+81>.lmp" and "<cell>.cut". levelName is the 322-byte
// object at 0x0034b660; byte 81 is the observed source of the lump stem.
// lumpFindResource receives those two names, and cutStartScene receives the
// resource and the cut name. The native returns zero.
extern "C" int amx_GetAddr(void* amx, int address, int** physical);
extern "C" int amx_GetString(char* destination, int* source);
extern "C" char levelName[];
extern "C" int sprintf(char* destination, const char* format, ...);
extern "C" void* lumpFindResource__FPcT0(char* lump, char* cut);
extern "C" void cutStartScene__FP9_cutscenePc(void* scene, char* cut);
extern "C" int amxCutscene__FP5__amxPi(void* amx, int* params) {
    int* cell = 0;
    char script_name[160];
    char lump_name[80];
    char cut_name[80];
    amx_GetAddr(amx, params[1], &cell);
    amx_GetString(script_name, cell);
    sprintf(lump_name, "%s.lmp", levelName + 81);
    sprintf(cut_name, "%s.cut", script_name);
    void* scene = lumpFindResource__FPcT0(lump_name, cut_name);
    cutStartScene__FP9_cutscenePc(scene, cut_name);
    return 0;
}
