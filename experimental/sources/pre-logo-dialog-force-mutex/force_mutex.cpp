// Observed DLG_ForceMutexRelease__Fv at 0x001d3918, 8 bytes.
// It stores 0 in the four-byte dialogMutex word at 0x00485080 and returns.
// dialogMutex is not defined here.
extern "C" int dialogMutex;

extern "C" void DLG_ForceMutexRelease__Fv() {
    dialogMutex = 0;
}
