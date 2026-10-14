// Observed amxStartAudio__FP5__amxPi at 0x0013bfd0, 64 bytes. Cell 1 is an
// AMX string. soundStartDialogStream receives 2, the 160-byte copy, and 2.
// The native returns 0.
extern "C" int amx_GetAddr(void* amx, int address, int** physical);
extern "C" int amx_GetString(char* dest, int* source);
extern "C" void soundStartDialogStream__FiPci(int channel, char* name, int mode);

extern "C" int amxStartAudio__FP5__amxPi(void* amx, int* params) {
    char name[160];
    int* physical = nullptr;
    amx_GetAddr(amx, params[1], &physical);
    amx_GetString(name, physical);
    soundStartDialogStream__FiPci(2, name, 2);
    return 0;
}
