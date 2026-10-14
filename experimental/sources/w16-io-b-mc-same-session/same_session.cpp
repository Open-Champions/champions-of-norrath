// Observed: VIIsMcSameSession__Fv at 0x0109fda8 is 36 bytes. It calls
// scePrintf("VIIsMcSameSessionId()\n") and returns 1. It does not read
// sSessionId.
// Inferred: nothing beyond the call being a stub-like always-true check.
// Unknown: whether a different session check was removed from this build.
extern "C" int scePrintf(const char* format, ...);

extern "C" int VIIsMcSameSession__Fv() {
    scePrintf("VIIsMcSameSessionId()\n");
    return 1;
}
