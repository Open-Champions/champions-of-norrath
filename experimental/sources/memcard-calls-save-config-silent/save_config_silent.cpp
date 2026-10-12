// Observed: MC_SaveConfigSilent__Fii at 0x0025ade0 is 96 bytes. It ORs the
// first argument into the first word of MC_GameConfig before testing
// disableMC. A nonzero disableMC returns 101, and a nonzero second
// argument also stores 101 in gameBailFromGameLoop. A zero disableMC stores
// the second argument's low byte in saveConfigEndGame, clears ranConfig and
// delayCount, calls pushPause, and returns newMode(5, 0).
// Inferred: nothing about the two callees.
// Unknown: which bit of the config word the first argument represents.

extern "C" int MC_GameConfig[];
extern "C" int disableMC;
extern "C" int gameBailFromGameLoop;
extern "C" unsigned char saveConfigEndGame;
extern "C" int ranConfig;
extern "C" int delayCount;
extern "C" void pushPause__Fv();
extern "C" int newMode__Fii(int mode, int argument);

extern "C" int MC_SaveConfigSilent__Fii(int first, int second) {
    MC_GameConfig[0] |= first;
    if (disableMC != 0) {
        if (second != 0) gameBailFromGameLoop = 101;
        return 101;
    }
    saveConfigEndGame = static_cast<unsigned char>(second);
    ranConfig = 0;
    delayCount = 0;
    pushPause__Fv();
    return newMode__Fii(5, 0);
}
