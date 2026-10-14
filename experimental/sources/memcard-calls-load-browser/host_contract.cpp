#include "load_browser.cpp"

#include <cstdio>

extern "C" int disableMC = 0;
extern "C" int actionCoolOff = 0;
extern "C" signed char whichPad = 0;
extern "C" int frontEndRunning = 0;
extern "C" unsigned char frontEndWasRunning = 0;
extern "C" signed char fullLaunchOK = 0;
extern "C" int ranConfig = 1;
extern "C" int ranBrowser = 1;
extern "C" int flareDelta = 1;
extern "C" int flareAlpha = 1;
extern "C" unsigned char MC_SavingOrLoading = 1;
extern "C" unsigned char MC_LastModeSuccessful = 1;

static int pauses;
static int pad_at_pause;
static int mode;
static int last_at_mode;

extern "C" void pushPause__Fv() {
    ++pauses;
    pad_at_pause = whichPad;
}
extern "C" int newMode__Fii(int next, int) {
    mode = next;
    last_at_mode = MC_LastModeSuccessful;
    return 0;
}

int main() {
    disableMC = 1;
    actionCoolOff = 0;
    if (pauses != 0) return 1;
    MC_LoadGameBrowser__Fii(2, 1);
    if (pauses != 0) return 2;
    disableMC = 0;
    actionCoolOff = 3;
    MC_LoadGameBrowser__Fii(2, 1);
    if (pauses != 0) return 3;

    actionCoolOff = 0;
    frontEndRunning = 0x105;
    ranConfig = 9;
    ranBrowser = 9;
    flareDelta = 9;
    flareAlpha = 9;
    MC_SavingOrLoading = 9;
    MC_LastModeSuccessful = 9;
    MC_LoadGameBrowser__Fii(0x180, 0x101);
    if (pauses != 1 || pad_at_pause != -128) return 4;
    if (fullLaunchOK != 1 || frontEndWasRunning != 5) return 5;
    if (ranConfig != 0 || ranBrowser != 0 || flareDelta != 0 || flareAlpha != 0) return 6;
    if (MC_SavingOrLoading != 0 || last_at_mode != 0 || mode != 3) return 7;
    std::puts("load browser passed");
    return 0;
}
