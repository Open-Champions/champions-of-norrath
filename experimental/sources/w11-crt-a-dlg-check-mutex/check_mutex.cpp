// Observed DLG_CheckMutex__Fv at 0x001d3910, 8 bytes, pinned SLUS_205.65.
// Returns the four-byte dialogMutex word at 0x00485080 and changes nothing.
// DLG_SetMutex__Fv sets that word to 1 and DLG_ForceMutexRelease__Fv clears it;
// dialogMutex is not defined here.
extern "C" int dialogMutex;

extern "C" int DLG_CheckMutex__Fv() {
    return dialogMutex;
}
