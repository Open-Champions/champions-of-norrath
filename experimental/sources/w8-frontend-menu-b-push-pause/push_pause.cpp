// Observed: pushPause__Fv at 0x0025b970 is 72 bytes. A nonzero netNetworkMode
// returns at once. Otherwise it stores the low byte of gamePause into
// pauseStack at the signed byte index pauseStackPos, then increments the
// index byte. If frontEndRunning is zero, it then sets particlesPaused to 1
// and gamePause to 1.
// Inferred: pauseStack is a saved-pause-state stack, from the paired use in
// popPause. The stack capacity is unknown; nothing here bounds the index.
// Unknown: the pauseStack object size and whether another caller relies on
// the signed index.
extern "C" signed char netNetworkMode;
extern "C" unsigned char pauseStack[];
extern "C" unsigned char pauseStackPos;
extern "C" int gamePause;
extern "C" int frontEndRunning;
extern "C" signed char particlesPaused;

extern "C" void pushPause__Fv() {
    if (netNetworkMode != 0) return;
    pauseStack[static_cast<signed char>(pauseStackPos)] = static_cast<unsigned char>(gamePause);
    pauseStackPos = static_cast<unsigned char>(pauseStackPos + 1);
    if (frontEndRunning == 0) {
        particlesPaused = 1;
        gamePause = 1;
    }
}
