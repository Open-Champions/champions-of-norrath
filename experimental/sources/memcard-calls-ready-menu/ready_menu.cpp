// Observed: MC_GetReadyForMainMenu__Fv at 0x0025acb8 is 28 bytes. It calls
// resetQuickLoad and returns.
// Inferred: nothing about resetQuickLoad.
// Unknown: which menu read follows this call.

extern "C" void resetQuickLoad__Fv();

extern "C" void MC_GetReadyForMainMenu__Fv() {
    resetQuickLoad__Fv();
}
