// Observed popPause__Fi at 0x0025b9b8, 100 bytes (pinned SLUS_205.65). Mirror
// of pushPause__Fv. A nonzero netNetworkMode returns at once. Otherwise the
// byte pauseStackPos is decremented and the new value is used as a signed
// index into pauseStack. When the argument is nonzero the entry is first
// zeroed. The entry is then read back: it is stored to gamePause as a signed
// byte, and, only while frontEndRunning is zero, to particlesPaused as an
// unsigned byte. If the value written to gamePause is zero, gameControlPaused
// is set to 60.
// Inferred: the argument means "clear the popped entry", from the zeroing store.
// Unknown: why 60 (the original gives no name), and the stack capacity.
extern "C" {
extern signed char netNetworkMode;
extern unsigned char pauseStack[];
extern unsigned char pauseStackPos;
extern int gamePause;
extern int frontEndRunning;
extern signed char particlesPaused;
extern int gameControlPaused;

void popPause__Fi(int clearEntry) {
    if (netNetworkMode != 0) return;
    pauseStackPos = static_cast<unsigned char>(pauseStackPos - 1);
    signed char index = static_cast<signed char>(pauseStackPos);
    if (clearEntry != 0) pauseStack[index] = 0;
    signed char value = static_cast<signed char>(pauseStack[index]);
    gamePause = value;
    if (frontEndRunning == 0) particlesPaused = static_cast<signed char>(pauseStack[index]);
    if (value == 0) gameControlPaused = 60;
}
}
