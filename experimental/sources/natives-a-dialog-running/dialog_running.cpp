// amxDialogRunning is the 20-byte native at 0x0013b480. It returns
// DLG_Active and does not read the AMX arguments.
extern "C" int DLG_Active__Fv();
extern "C" int amxDialogRunning__FP5__amxPi(void* amx, int* params) {
    (void)amx;
    (void)params;
    return DLG_Active__Fv();
}
