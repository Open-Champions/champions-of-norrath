#include "frontend_init_controller_config.cpp"

#include <cstdio>

int curFocus = 9, frontendControllerConfigRunning, gameNoDraw, curSel = 5, newSel = 6, t = 7;
int configMode = 8, inGameConfigShuttingDown = 1, askedForConfigLoad = 2, initializedLump = 3;
int myLumpBuffer = 4, finishFrameCount = 5;
signed char configLumpLoaded = 1;
static int calls, snapshot_ok;
extern "C" void texturePrepareToLockCacheBuffer__Fv() {
    ++calls;
    snapshot_ok = finishFrameCount == 0 && myLumpBuffer == 0 && configLumpLoaded == 0 && curFocus == 3;
}

int main() {
    frontendInitControllerConfig__Fi(3);
    if (calls != 1 || !snapshot_ok) return 1;
    if (curFocus != 3 || frontendControllerConfigRunning != 1 || gameNoDraw != 1) return 2;
    if (curSel || newSel || t || configMode || inGameConfigShuttingDown || askedForConfigLoad) return 3;
    if (initializedLump || myLumpBuffer || finishFrameCount || configLumpLoaded) return 4;
    frontendInitControllerConfig__Fi(-1);
    if (curFocus != -1) return 5;
    std::puts("frontend init controller config passed");
    return 0;
}
