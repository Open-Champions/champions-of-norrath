// Observed amxSaveCheckPoint__FP5__amxPi at 0x0013a9d0, 52 bytes. It prints
// the literal "amxSaveCheckPoint\n" and calls MC_SaveCheckPoint(0, 0, 1).
// The native returns 0. The AMX arguments are not read.
extern "C" int scePrintf(const char* text);
extern "C" void MC_SaveCheckPoint__Fiii(int first, int second, int third);

extern "C" int amxSaveCheckPoint__FP5__amxPi(void*, int*) {
    scePrintf("amxSaveCheckPoint\n");
    MC_SaveCheckPoint__Fiii(0, 0, 1);
    return 0;
}
