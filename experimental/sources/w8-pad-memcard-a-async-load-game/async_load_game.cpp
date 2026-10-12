// Observed: asyncLoadGame__Fv at 0x00251a28 is 1456 bytes. Same overall shape as
// asyncSaveGame__Fv, with these differences.
// Entry. With ranConfig zero it calls runConfigure__FiPUsi(0, text "menu_loadgame", 0).
// Result -1 stores 1 in ranConfig and runs the cancel sequence, then returns; any result
// other than 1 returns; result 1 stores 1 in ranConfig and continues.
// Browser. With ranBrowser zero it calls runBrowser__FPUs(text "MemcardLoad"); there is no
// configFormatted shortcut. Result -1 runs the cancel sequence and returns, -2 stores 0 in
// ranConfig and starts again from the top, 0 returns, any other result except 1 skips the
// setup. Result 1 stores 16 in flareDelta, 1 in ranBrowser and 0 in syncState; when the
// byte frontEndWasRunning (read first) and the byte fullLaunchOK are both nonzero it calls
// frontEndCleanUp__Fi(0); then hudResetFadeScreen__Fi(0), prepareBrowserUndo__Fv, a 64-byte
// copy of sortedDir record browserSlot (stride 64) into quickLoadStamp, and startProgress__Fv.
// Every pass then calls drawProgress__Fv unless syncState is 9, stores flareAlpha plus
// flareDelta clamped to 0..128 in flareAlpha, and returns while MCA_Busy__Fv is nonzero. A
// nonzero errorCondition with syncState not 9 calls textureReleaseCacheBuffer__Fv when the
// function-local localBuffer is nonzero, then resetWithClose__Fv and coreStartLoading__Fv, and
// stores 9 in syncState. MC_SavingOrLoading is then set to 1 and the state is dispatched; states
// 6 to 8 and anything above 9 do nothing.
// State 0: hudFadeScreen__Fi(8) must return 128. Then oldClearColor (function-local) takes
// coreClearColor, coreClearColor becomes 0x80000000, localBuffer becomes zero, gameNoDraw
// becomes 1, floorDrawEnable and objectDrawEnable become 0, hudResetFadeScreen__Fi(0) is called
// and syncState advances by one.
// State 1: flareAlpha must be 128; then texturePrepareToLockCacheBuffer__Fv and advance.
// State 2: localBuffer takes textureLockCacheBuffer__Fv (stored even when zero); a zero result
// stops. Otherwise it queues MCA_Cmd(0, quickLoadStamp + 32, 1, 0), MCA_Cmd(3, localBuffer,
// 0xb29c8, 0), MCA_Cmd(1, 0, 0, 0) and advances.
// State 3: stores -16 in flareDelta; advances only when flareAlpha is zero.
// State 4: copies 0xb29c8 bytes from localBuffer to ramSlot, calls textureReleaseCacheBuffer__Fv,
// restores coreClearColor from oldClearColor, stores 0 in gameNoDraw and 1 in floorDrawEnable and
// objectDrawEnable. A nonzero fullLaunchOK calls loadRamSlot__Fv and advances. Otherwise it
// stores 1 in MC_LastModeSuccessful, -1 in frontEndWasRunning and 0 in MC_SavingOrLoading,
// calls popPause__Fi(0), newMode__Fii(-1, 0) and coreStartLoading__Fv.
// State 5: gameGetFullLevelName__FPc fills a 256-byte stack buffer; it must strcmp equal to
// loadName. Then popPause__Fi(1), newMode__Fii(-1, 0), 0 in MC_SavingOrLoading, -1 in
// frontEndWasRunning, 1 in MC_LastModeSuccessful and a call to padInitPadIndexMap__Fv.
// State 9: formats hudFooterUni from text "MemcardBack" with (float)padBackButtonFontChar and
// seven zeros, fills a stack title with memcardTitle__FPUs and shows hudShowMsg(title, text
// "MemcardCantLoad", hudFooterUni, 128, 0, 0). If the word at netLocalPad + 88 * whichPad + 12
// has a padBackButton bit: gameBailFromGameLoop becomes 1 when frontEndWasRunning is nonzero;
// actionCoolOff 15, frontEndWasRunning -1, objectDrawEnable 1, coreClearColor from oldClearColor,
// floorDrawEnable 1, MC_SavingOrLoading 0, gameNoDraw 0, then undoBrowser__Fv, newMode__Fii(-1, 0)
// and popPause__Fi(0).
// Cancel sequence: newMode__Fii(-1, 0) then popPause__Fi(0).
// Inferred: nothing.
// Unknown: what loadName holds; the meaning of the other states.

extern "C" unsigned short* textGetString__FPc(char* key);
extern "C" void usprintf__FPUsT0ffffffff(unsigned short* dest, unsigned short* format,
                                         float a, float b, float c, float d, float e,
                                         float f, float g, float h);
extern "C" int runConfigure__FiPUsi(int mode, unsigned short* title, int extra);
extern "C" int runBrowser__FPUs(unsigned short* title);
extern "C" int MCA_Busy__Fv();
extern "C" void MCA_Cmd__Fiiii(int command, const void* text, int third, int fourth);
extern "C" void memcardTitle__FPUs(unsigned short* title);
extern "C" void hudShowMsg__FPUsN20iii(unsigned short* title, unsigned short* text,
                                        unsigned short* footer, int a, int b, int c);
extern "C" void popPause__Fi(int clear);
extern "C" int newMode__Fii(int mode, int keep);
extern "C" void resetWithClose__Fv();
extern "C" void startProgress__Fv();
extern "C" void drawProgress__Fv();
extern "C" void gameGetFullLevelName__FPc(char* name);
extern "C" void prepareBrowserUndo__Fv();
extern "C" void undoBrowser__Fv();
extern "C" int strcmp(const char* a, const char* b);
extern "C" void* memcpy(void* dest, const void* source, unsigned long count);
extern "C" void frontEndCleanUp__Fi(int mode);
extern "C" void hudResetFadeScreen__Fi(int mode);
extern "C" int hudFadeScreen__Fi(int mode);
extern "C" void coreStartLoading__Fv();
extern "C" void texturePrepareToLockCacheBuffer__Fv();
extern "C" unsigned char* textureLockCacheBuffer__Fv();
extern "C" void textureReleaseCacheBuffer__Fv();
extern "C" void loadRamSlot__Fv();
extern "C" void padInitPadIndexMap__Fv();
extern "C" int ranConfig;
extern "C" int ranBrowser;
extern "C" int syncState;
extern "C" int errorCondition;
extern "C" int flareAlpha;
extern "C" int flareDelta;
extern "C" int browserSlot;
extern "C" char sortedDir[];
extern "C" signed char MC_SavingOrLoading;
extern "C" unsigned char MC_LastModeSuccessful;
extern "C" signed char whichPad;
extern "C" signed char frontEndWasRunning;
extern "C" signed char fullLaunchOK;
extern "C" int actionCoolOff;
extern "C" int gameNoDraw;
extern "C" int gameBailFromGameLoop;
extern "C" unsigned coreClearColor;
extern "C" signed char floorDrawEnable;
extern "C" signed char objectDrawEnable;
extern "C" unsigned padBackButton;
extern "C" int padBackButtonFontChar;
extern "C" char loadName[];
extern "C" unsigned char netLocalPad[];
extern "C" unsigned char ramSlot[];
extern "C" unsigned char quickLoadStamp[64];
extern "C" unsigned short hudFooterUni[];

namespace {
unsigned char* localBuffer;
unsigned oldClearColor;

struct Stamp {
    unsigned char bytes[64];
};

void cancel() {
    newMode__Fii(-1, 0);
    popPause__Fi(0);
}

unsigned padWord() {
    return *reinterpret_cast<const unsigned*>(netLocalPad + whichPad * 88 + 12);
}
}

extern "C" void asyncLoadGame__Fv() {
    unsigned short title[256];
top:
    if (ranConfig == 0) {
        int result = runConfigure__FiPUsi(0, textGetString__FPc(const_cast<char*>("menu_loadgame")), 0);
        if (result == -1) {
            ranConfig = 1;
            cancel();
            return;
        }
        if (result != 1) {
            return;
        }
        ranConfig = result;
    }
    if (ranBrowser == 0) {
        int result = runBrowser__FPUs(textGetString__FPc(const_cast<char*>("MemcardLoad")));
        bool start = false;
        if (result == -1) {
            cancel();
            return;
        }
        if (result < 0) {
            if (result == -2) {
                ranConfig = 0;
                goto top;
            }
        } else if (result == 0) {
            return;
        } else if (result == 1) {
            start = true;
        }
        if (start) {
            int running = frontEndWasRunning;
            flareDelta = 16;
            ranBrowser = 1;
            syncState = 0;
            if (running != 0 && fullLaunchOK != 0) {
                frontEndCleanUp__Fi(0);
            }
            hudResetFadeScreen__Fi(0);
            prepareBrowserUndo__Fv();
            *reinterpret_cast<Stamp*>(quickLoadStamp) =
                *reinterpret_cast<const Stamp*>(sortedDir + browserSlot * 64);
            startProgress__Fv();
        }
    }
    if (syncState != 9) {
        drawProgress__Fv();
    }
    {
        int alpha = flareAlpha + flareDelta;
        if (alpha >= 129) {
            alpha = 128;
        }
        flareAlpha = alpha < 0 ? 0 : alpha;
    }
    if (MCA_Busy__Fv() != 0) {
        return;
    }
    if (errorCondition != 0 && syncState != 9) {
        if (localBuffer != 0) {
            textureReleaseCacheBuffer__Fv();
        }
        resetWithClose__Fv();
        coreStartLoading__Fv();
        syncState = 9;
    }
    MC_SavingOrLoading = 1;
    switch (static_cast<unsigned>(syncState)) {
    case 0:
        if (hudFadeScreen__Fi(8) == 128) {
            oldClearColor = coreClearColor;
            coreClearColor = 0x80000000u;
            localBuffer = 0;
            gameNoDraw = 1;
            floorDrawEnable = 0;
            objectDrawEnable = 0;
            hudResetFadeScreen__Fi(0);
            syncState = syncState + 1;
        }
        break;
    case 1:
        if (flareAlpha == 128) {
            texturePrepareToLockCacheBuffer__Fv();
            syncState = syncState + 1;
        }
        break;
    case 2:
        localBuffer = textureLockCacheBuffer__Fv();
        if (localBuffer != 0) {
            MCA_Cmd__Fiiii(0, quickLoadStamp + 32, 1, 0);
            MCA_Cmd__Fiiii(3, localBuffer, 0xb29c8, 0);
            MCA_Cmd__Fiiii(1, 0, 0, 0);
            syncState = syncState + 1;
        }
        break;
    case 3:
        flareDelta = -16;
        if (flareAlpha == 0) {
            syncState = syncState + 1;
        }
        break;
    case 4:
        memcpy(ramSlot, localBuffer, 0xb29c8);
        textureReleaseCacheBuffer__Fv();
        coreClearColor = oldClearColor;
        gameNoDraw = 0;
        floorDrawEnable = 1;
        objectDrawEnable = 1;
        if (fullLaunchOK != 0) {
            loadRamSlot__Fv();
            syncState = syncState + 1;
        } else {
            MC_LastModeSuccessful = 1;
            frontEndWasRunning = -1;
            MC_SavingOrLoading = 0;
            popPause__Fi(0);
            newMode__Fii(-1, 0);
            coreStartLoading__Fv();
        }
        break;
    case 5: {
        char name[256];
        gameGetFullLevelName__FPc(name);
        if (strcmp(name, loadName) == 0) {
            popPause__Fi(1);
            newMode__Fii(-1, 0);
            MC_SavingOrLoading = 0;
            frontEndWasRunning = -1;
            MC_LastModeSuccessful = 1;
            padInitPadIndexMap__Fv();
        }
        break;
    }
    case 9:
        usprintf__FPUsT0ffffffff(hudFooterUni, textGetString__FPc(const_cast<char*>("MemcardBack")),
                                 static_cast<float>(padBackButtonFontChar), 0.0f, 0.0f, 0.0f, 0.0f,
                                 0.0f, 0.0f, 0.0f);
        memcardTitle__FPUs(title);
        hudShowMsg__FPUsN20iii(title, textGetString__FPc(const_cast<char*>("MemcardCantLoad")),
                               hudFooterUni, 128, 0, 0);
        if ((padWord() & padBackButton) != 0) {
            if (frontEndWasRunning != 0) {
                gameBailFromGameLoop = 1;
            }
            actionCoolOff = 15;
            frontEndWasRunning = -1;
            objectDrawEnable = 1;
            coreClearColor = oldClearColor;
            floorDrawEnable = 1;
            MC_SavingOrLoading = 0;
            gameNoDraw = 0;
            undoBrowser__Fv();
            cancel();
        }
        break;
    default:
        break;
    }
}
