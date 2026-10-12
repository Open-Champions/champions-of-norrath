// Observed: MC_SaveConfig__Fv at 0x0025ada8 is 52 bytes. A nonzero
// disableMC returns that word. Otherwise it clears ranConfig and
// delayCount, calls pushPause, and returns newMode(4, 0). delayCount is
// cleared before pushPause runs.
// Inferred: nothing about the two callees.
// Unknown: what mode 4 selects.

extern "C" int disableMC;
extern "C" int ranConfig;
extern "C" int delayCount;
extern "C" void pushPause__Fv();
extern "C" int newMode__Fii(int mode, int argument);

extern "C" int MC_SaveConfig__Fv() {
    if (disableMC != 0) return disableMC;
    ranConfig = 0;
    delayCount = 0;
    pushPause__Fv();
    return newMode__Fii(4, 0);
}
