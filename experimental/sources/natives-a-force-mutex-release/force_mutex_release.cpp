// amxForceMutexRelease is the 32-byte native at 0x0013c3d8. It calls
// DLG_ForceMutexRelease and returns zero.
extern "C" void DLG_ForceMutexRelease__Fv();
extern "C" int amxForceMutexRelease__FP5__amxPi(void* amx, int* params) {
    (void)amx;
    (void)params;
    DLG_ForceMutexRelease__Fv();
    return 0;
}
