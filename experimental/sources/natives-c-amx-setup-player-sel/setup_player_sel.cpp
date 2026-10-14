// Observed amxSetupPlayerSel__FP5__amxPi at 0x0013aa68, 32 bytes. It calls
// playerSelectionSetup and returns 0. The AMX arguments are not read.
extern "C" void playerSelectionSetup__Fv();

extern "C" int amxSetupPlayerSel__FP5__amxPi(void*, int*) {
    playerSelectionSetup__Fv();
    return 0;
}
