// Observed: mcHeartBeat__Fv at 0x00251370 is 224 bytes. It returns without
// calls when disableMC is nonzero and asyncMode is neither 0 nor 7.
// Otherwise it calls MCA_Process with errorCondition. asyncMode below 8
// selects a handler: 0 calls asyncConfigure, 2 calls asyncSaveGame and then
// netBlankController with the sign-extended whichPad byte and 1, 3 calls
// asyncLoadGame, 4 calls asyncSaveConfig, 5 calls asyncSaveConfigSilent,
// and 7 calls asyncChangePlayers. Modes 1 and 6 call nothing. A nonzero
// actionCoolOff then decreases by one, and errorCondition is cleared.
// Inferred: the eight targets are the jump table at 0x00474d20.
// Unknown: what the eight mode numbers mean to the card browser.

extern "C" int disableMC;
extern "C" int asyncMode;
extern "C" int errorCondition;
extern "C" int actionCoolOff;
extern "C" signed char whichPad;
extern "C" int MCA_Process__FPi(int* error);
extern "C" void asyncConfigure__Fv();
extern "C" void asyncSaveGame__Fv();
extern "C" void asyncLoadGame__Fv();
extern "C" void asyncSaveConfig__Fv();
extern "C" void asyncSaveConfigSilent__Fv();
extern "C" void asyncChangePlayers__Fv();
extern "C" void netBlankController__Fii(int pad, int argument);

extern "C" void mcHeartBeat__Fv() {
    if (disableMC != 0 && asyncMode != 0 && asyncMode != 7) return;
    MCA_Process__FPi(&errorCondition);
    unsigned mode = static_cast<unsigned>(asyncMode);
    if (mode < 8u) {
        if (asyncMode == 0) asyncConfigure__Fv();
        else if (asyncMode == 2) {
            asyncSaveGame__Fv();
            netBlankController__Fii(whichPad, 1);
        } else if (asyncMode == 3) asyncLoadGame__Fv();
        else if (asyncMode == 4) asyncSaveConfig__Fv();
        else if (asyncMode == 5) asyncSaveConfigSilent__Fv();
        else if (asyncMode == 7) asyncChangePlayers__Fv();
    }
    if (actionCoolOff != 0) actionCoolOff -= 1;
    errorCondition = 0;
}
