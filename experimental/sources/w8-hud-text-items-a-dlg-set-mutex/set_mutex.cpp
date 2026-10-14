// Observed DLG_SetMutex__Fv at 0x001d38f0, 32 bytes, pinned SLUS_205.65.
// Reads the four-byte dialogMutex word at 0x00485080. If it is nonzero the
// function returns -1 and leaves it. Otherwise it stores 1 and returns 0.
// dialogMutex is not defined here. DLG_ForceMutexRelease__Fv stores 0.
extern "C" int dialogMutex;

extern "C" int DLG_SetMutex__Fv() {
    if (dialogMutex != 0) return -1;
    dialogMutex = 1;
    return 0;
}
