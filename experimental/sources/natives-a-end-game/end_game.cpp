// amxEndGame is the 36-byte native at 0x0013bf08. It calls
// MC_SaveConfigSilent(0, 1) and returns zero.
extern "C" int MC_SaveConfigSilent__Fii(int slot, int mode);
extern "C" int amxEndGame__FP5__amxPi(void* amx, int* params) {
    (void)amx;
    (void)params;
    MC_SaveConfigSilent__Fii(0, 1);
    return 0;
}
