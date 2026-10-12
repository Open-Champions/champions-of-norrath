// Observed amxPauseGameControl at 0x0013be30, 24 bytes. It stores 1
// in gameControlDontResetPause at 0x00483f20 and the first AMX cell
// in gameControlPaused at 0x00483f1c, then returns zero.
extern "C" int gameControlDontResetPause;
extern "C" int gameControlPaused;

extern "C" int amxPauseGameControl__FP5__amxPi(void*, int* params) {
    gameControlDontResetPause = 1;
    gameControlPaused = params[1];
    return 0;
}
