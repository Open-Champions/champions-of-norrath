// Observed debugClearStartMenu__Fv at 0x0017ff18, 92 bytes. It does nothing
// unless debugPauseMenuOn (a signed byte) is non-zero. Then it clears
// startMenuState (the object at 0x004841c8; a second local of that name sits
// at 0x00484bdc) and exitToMenuMode, and only when netNetworkMode is zero it
// also clears gamePause (word) and particlesPaused (byte). It clears
// debugPauseMenuOn, marks the local network player (netGetLocalPlayerNm(0))
// not busy, and removes the task whose entry is pauseMenuFrameRun__Fv.
// Unknown: the meaning of the busy flag beyond the callee's name.
extern "C" {
signed char netGetLocalPlayerNm__Fi(int index);
void msgHudSetPlayerBusy__Fcc(signed char player, signed char busy);
void pauseMenuFrameRun__Fv();
void engineRemoveTask__FPFv_v(void (*task)());
extern signed char debugPauseMenuOn;
extern signed char netNetworkMode;
extern unsigned char startMenuState;
extern unsigned char exitToMenuMode;
extern int gamePause;
extern unsigned char particlesPaused;

void debugClearStartMenu__Fv() {
    if (debugPauseMenuOn) {
        startMenuState = 0;
        exitToMenuMode = 0;
        if (!netNetworkMode) {
            gamePause = 0;
            particlesPaused = 0;
        }
        debugPauseMenuOn = 0;
        msgHudSetPlayerBusy__Fcc(netGetLocalPlayerNm__Fi(0), 0);
        engineRemoveTask__FPFv_v(pauseMenuFrameRun__Fv);
    }
}
}
