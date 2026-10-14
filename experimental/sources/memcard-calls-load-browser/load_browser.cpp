// Observed: MC_LoadGameBrowser__Fii at 0x0025ad38 is 108 bytes. A nonzero
// disableMC or actionCoolOff returns without stores. Otherwise it stores
// the first argument's low byte in whichPad before pushPause runs, copies
// the low byte of frontEndRunning into frontEndWasRunning, stores the
// second argument's low byte in fullLaunchOK, clears ranConfig, ranBrowser,
// flareDelta, flareAlpha, and MC_SavingOrLoading, clears
// MC_LastModeSuccessful, and calls newMode(3, 0).
// Inferred: nothing about pushPause or newMode.
// Unknown: what mode 3 selects.

extern "C" int disableMC;
extern "C" int actionCoolOff;
extern "C" signed char whichPad;
extern "C" int frontEndRunning;
extern "C" unsigned char frontEndWasRunning;
extern "C" signed char fullLaunchOK;
extern "C" int ranConfig;
extern "C" int ranBrowser;
extern "C" int flareDelta;
extern "C" int flareAlpha;
extern "C" unsigned char MC_SavingOrLoading;
extern "C" unsigned char MC_LastModeSuccessful;
extern "C" void pushPause__Fv();
extern "C" int newMode__Fii(int mode, int argument);

namespace {
int low_byte(int value) {
    int low = value & 0xff;
    return low < 128 ? low : low - 256;
}
}

extern "C" void MC_LoadGameBrowser__Fii(int first, int second) {
    if (disableMC != 0) return;
    if (actionCoolOff != 0) return;
    whichPad = static_cast<signed char>(low_byte(first));
    pushPause__Fv();
    fullLaunchOK = static_cast<signed char>(low_byte(second));
    frontEndWasRunning = static_cast<unsigned char>(frontEndRunning);
    ranConfig = 0;
    ranBrowser = 0;
    flareDelta = 0;
    flareAlpha = 0;
    MC_SavingOrLoading = 0;
    MC_LastModeSuccessful = 0;
    newMode__Fii(3, 0);
}
