// Observed: frontendInitControllerConfig__Fi at 0x001ca968 is 76 bytes. It
// stores its argument in curFocus, one in frontendControllerConfigRunning and
// gameNoDraw, zero in the words curSel, newSel, t, configMode,
// inGameConfigShuttingDown, askedForConfigLoad, initializedLump, myLumpBuffer
// and finishFrameCount, and zero in the byte configLumpLoaded. It then calls
// texturePrepareToLockCacheBuffer__Fv.
// Inferred: nothing.
// Unknown: what each word means to the controller-config screen.

extern "C" void texturePrepareToLockCacheBuffer__Fv();
extern "C" int curFocus;
extern "C" int frontendControllerConfigRunning;
extern "C" int gameNoDraw;
extern "C" int curSel;
extern "C" int newSel;
extern "C" int t;
extern "C" int configMode;
extern "C" int inGameConfigShuttingDown;
extern "C" int askedForConfigLoad;
extern "C" signed char configLumpLoaded;
extern "C" int initializedLump;
extern "C" int myLumpBuffer;
extern "C" int finishFrameCount;

extern "C" void frontendInitControllerConfig__Fi(int focus) {
    curFocus = focus;
    frontendControllerConfigRunning = 1;
    gameNoDraw = 1;
    curSel = 0;
    newSel = 0;
    t = 0;
    configMode = 0;
    inGameConfigShuttingDown = 0;
    askedForConfigLoad = 0;
    configLumpLoaded = 0;
    initializedLump = 0;
    myLumpBuffer = 0;
    finishFrameCount = 0;
    texturePrepareToLockCacheBuffer__Fv();
}
