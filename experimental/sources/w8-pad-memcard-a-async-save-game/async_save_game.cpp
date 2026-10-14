// Observed: asyncSaveGame__Fv at 0x00251450 is 1492 bytes. All state words are
// globals except where noted.
// Entry. With ranConfig zero it calls runConfigure__FiPUsi(0, text "menu_savegame", 0).
// Result -1 stores 1 in ranConfig and runs the cancel sequence, then returns; a
// result other than 1 returns; result 1 stores 1 in ranConfig and continues.
// Browser. With ranBrowser zero: a nonzero configFormatted byte skips the browser;
// otherwise runBrowser__FPUs(text "MemcardSave") is called. Result -1 runs the cancel
// sequence and returns, -2 stores 0 in ranConfig and starts again from the top, 0
// returns, any other result except 1 skips the setup. Result 1 (or a formatted
// configuration) stores 1 in MC_SavingOrLoading, 8 in flareDelta, 1 in ranBrowser,
// zero in syncState and calls startProgress__Fv.
// Every pass then stores flareAlpha plus flareDelta clamped to 0..127 in flareAlpha,
// calls drawProgress__Fv unless syncState is 9 or 4, and returns while
// MCA_Busy__Fv is nonzero. A nonzero errorCondition calls resetWithClose__Fv and
// stores 9 in syncState unless it already is 9. States above 9 return.
// State 0 stores 1 in MC_KickProtect, calls gameGetFullLevelName__FPc on a 256-byte
// stack buffer that is not read afterwards, stores the CP0 Count register in the
// function-local cycleStart, calls prepareBrowserUndo__Fv, then fills the local
// string localSlotFileName: getBestSlotName__FPc when configFormatted is nonzero or
// browserSlot equals sortedDirCount, else strcpy from the 64-byte sortedDir record
// browserSlot at offset 32. It adds one to syncState.
// State 1 calls modifyRamSlotForNetSave__Fv when netNetworkMode is nonzero, else
// updateRamSlot__Fii(whichPad, 0); adds one to syncState.
// State 2 queues MCA_Cmd(0, name, 514, 0), MCA_Cmd(5, ramSlot, 0xb29c8, 0),
// MCA_Cmd(1, 0, 0, 0) and MCA_Cmd(6, name, 0, 0) and adds one to syncState.
// State 3 with flareDelta positive copies 64 bytes from MCA_DirContents to
// quickLoadStamp, calls unModifyRamSlotForNetSave__Fv when netNetworkMode is
// nonzero, stores 1 in MC_LastModeSuccessful, -16 in flareDelta and 0 in
// MC_KickProtect, and (netNetworkMode and levelName[0] both nonzero) also zeroes
// flareAlpha. With flareDelta zero it stores 8 in flareDelta and adds one to
// syncState; with flareDelta negative it does nothing.
// State 4 formats hudFooterUni from text "MemcardContinue" with (float)padAcceptButtonFontChar
// and seven zeros, then calls hudShowMsg__FPUsN20iii(text "MemcardSaveComplete",
// text "MemcardSaveGood", hudFooterUni, flareAlpha, 0, 0). It finishes when (netNetworkMode
// and levelName[0] are nonzero), cutMode is nonzero, or DLG_Active__Fv is nonzero, or
// else when flareDelta is positive and the word at netLocalPad + 88 * whichPad + 12 has a
// padAcceptButton bit (which stores -16 in flareDelta instead), or flareDelta is zero.
// Finishing calls popPause__Fi(0) unless netNetworkMode is nonzero, newMode__Fii(-1, 0),
// stores 0 in MC_SavingOrLoading and 15 in gameControlPaused and actionCoolOff, and
// calls msgHudSetPlayerBusy__Fcc(whichPad, 0).
// State 9 formats hudFooterUni from text "MemcardBack" with (float)padBackButtonFontChar and
// seven zeros, fills a stack title with memcardTitle__FPUs and shows
// hudShowMsg(title, text "MemcardCantSave", hudFooterUni, 128, 0, 0). A padBackButton bit
// in the same pad word, or netNetworkMode and levelName[0] both nonzero, calls
// msgHudSetPlayerBusy(whichPad, 0), stores 15 in actionCoolOff, stores 0 in
// MC_KickProtect, calls undoBrowser__Fv, then runs the cancel sequence.
// Cancel sequence: msgHudSetPlayerBusy__Fcc(whichPad, 0), newMode__Fii(-1, 0), and
// popPause__Fi(0) unless netNetworkMode is nonzero.
// Inferred: nothing.
// Unknown: the meaning of states 5 to 8 (they do nothing) and of the runBrowser results.

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
extern "C" void getBestSlotName__FPc(char* name);
extern "C" char* strcpy(char* dest, const char* source);
extern "C" void modifyRamSlotForNetSave__Fv();
extern "C" void unModifyRamSlotForNetSave__Fv();
extern "C" void updateRamSlot__Fii(int first, int second);
extern "C" void msgHudSetPlayerBusy__Fcc(int player, int busy);
extern "C" int DLG_Active__Fv();
extern "C" int ranConfig;
extern "C" int ranBrowser;
extern "C" int syncState;
extern "C" int errorCondition;
extern "C" int flareAlpha;
extern "C" int flareDelta;
extern "C" int browserSlot;
extern "C" int sortedDirCount;
extern "C" char sortedDir[];
extern "C" signed char configFormatted;
extern "C" signed char MC_SavingOrLoading;
extern "C" signed char MC_KickProtect;
extern "C" unsigned char MC_LastModeSuccessful;
extern "C" signed char netNetworkMode;
extern "C" signed char whichPad;
extern "C" signed char cutMode;
extern "C" int actionCoolOff;
extern "C" int gameControlPaused;
extern "C" unsigned padAcceptButton;
extern "C" unsigned padBackButton;
extern "C" int padAcceptButtonFontChar;
extern "C" int padBackButtonFontChar;
extern "C" char levelName[];
extern "C" unsigned char netLocalPad[];
extern "C" unsigned char ramSlot[];
extern "C" unsigned char MCA_DirContents[];
extern "C" unsigned char quickLoadStamp[64];
extern "C" unsigned short hudFooterUni[];

#ifdef __mips__
static inline unsigned readCycleCount() {
    unsigned value;
    asm volatile("mfc0 %0, $9" : "=r"(value));
    return value;
}
#else
extern "C" unsigned norrath_host_cycle_count();
static inline unsigned readCycleCount() { return norrath_host_cycle_count(); }
#endif

namespace {
unsigned cycleStart;
char localSlotFileName[672];

struct Stamp {
    unsigned char bytes[64];
};

void cancel() {
    msgHudSetPlayerBusy__Fcc(whichPad, 0);
    newMode__Fii(-1, 0);
    if (netNetworkMode == 0) {
        popPause__Fi(0);
    }
}

unsigned padWord() {
    return *reinterpret_cast<const unsigned*>(netLocalPad + whichPad * 88 + 12);
}
}

extern "C" void asyncSaveGame__Fv() {
    unsigned short title[256];
top:
    if (ranConfig == 0) {
        int result = runConfigure__FiPUsi(0, textGetString__FPc(const_cast<char*>("menu_savegame")), 0);
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
        bool start = true;
        if (configFormatted == 0) {
            int result = runBrowser__FPUs(textGetString__FPc(const_cast<char*>("MemcardSave")));
            if (result == -1) {
                cancel();
                return;
            }
            if (result < 0) {
                if (result == -2) {
                    ranConfig = 0;
                    goto top;
                }
                start = false;
            } else if (result == 0) {
                return;
            } else if (result != 1) {
                start = false;
            }
        }
        if (start) {
            MC_SavingOrLoading = 1;
            flareDelta = 8;
            ranBrowser = 1;
            syncState = 0;
            startProgress__Fv();
        }
    }
    {
        int alpha = flareAlpha + flareDelta;
        if (alpha >= 128) {
            flareAlpha = 127;
        } else {
            flareAlpha = alpha < 0 ? 0 : alpha;
        }
    }
    if (syncState != 9 && syncState != 4) {
        drawProgress__Fv();
    }
    if (MCA_Busy__Fv() != 0) {
        return;
    }
    if (errorCondition != 0 && syncState != 9) {
        resetWithClose__Fv();
        syncState = 9;
    }
    switch (static_cast<unsigned>(syncState)) {
    case 0: {
        char name[256];
        MC_KickProtect = 1;
        gameGetFullLevelName__FPc(name);
        cycleStart = readCycleCount();
        prepareBrowserUndo__Fv();
        if (configFormatted != 0 || browserSlot == sortedDirCount) {
            getBestSlotName__FPc(localSlotFileName);
        } else {
            strcpy(localSlotFileName, sortedDir + 32 + browserSlot * 64);
        }
        syncState = syncState + 1;
        break;
    }
    case 1:
        if (netNetworkMode != 0) {
            modifyRamSlotForNetSave__Fv();
        } else {
            updateRamSlot__Fii(whichPad, 0);
        }
        syncState = syncState + 1;
        break;
    case 2:
        MCA_Cmd__Fiiii(0, localSlotFileName, 514, 0);
        MCA_Cmd__Fiiii(5, ramSlot, 0xb29c8, 0);
        MCA_Cmd__Fiiii(1, 0, 0, 0);
        MCA_Cmd__Fiiii(6, localSlotFileName, 0, 0);
        syncState = syncState + 1;
        break;
    case 3:
        if (flareDelta > 0) {
            *reinterpret_cast<Stamp*>(quickLoadStamp) = *reinterpret_cast<const Stamp*>(MCA_DirContents);
            if (netNetworkMode != 0) {
                unModifyRamSlotForNetSave__Fv();
            }
            MC_LastModeSuccessful = 1;
            flareDelta = -16;
            MC_KickProtect = 0;
            if (netNetworkMode != 0 && levelName[0] != 0) {
                flareAlpha = 0;
            }
        } else if (flareDelta == 0) {
            flareDelta = 8;
            syncState = syncState + 1;
        }
        break;
    case 4: {
        usprintf__FPUsT0ffffffff(hudFooterUni, textGetString__FPc(const_cast<char*>("MemcardContinue")),
                                 static_cast<float>(padAcceptButtonFontChar), 0.0f, 0.0f, 0.0f, 0.0f,
                                 0.0f, 0.0f, 0.0f);
        unsigned short* heading = textGetString__FPc(const_cast<char*>("MemcardSaveComplete"));
        hudShowMsg__FPUsN20iii(heading, textGetString__FPc(const_cast<char*>("MemcardSaveGood")),
                               hudFooterUni, flareAlpha, 0, 0);
        bool done = false;
        if ((netNetworkMode != 0 && levelName[0] != 0) || cutMode != 0 || DLG_Active__Fv() != 0) {
            done = true;
        } else if (flareDelta > 0) {
            if ((padWord() & padAcceptButton) != 0) {
                flareDelta = -16;
            }
        } else if (flareDelta == 0) {
            done = true;
        }
        if (done) {
            if (netNetworkMode == 0) {
                popPause__Fi(0);
            }
            newMode__Fii(-1, 0);
            MC_SavingOrLoading = 0;
            gameControlPaused = 15;
            actionCoolOff = 15;
            msgHudSetPlayerBusy__Fcc(whichPad, 0);
        }
        break;
    }
    case 9:
        usprintf__FPUsT0ffffffff(hudFooterUni, textGetString__FPc(const_cast<char*>("MemcardBack")),
                                 static_cast<float>(padBackButtonFontChar), 0.0f, 0.0f, 0.0f, 0.0f,
                                 0.0f, 0.0f, 0.0f);
        memcardTitle__FPUs(title);
        hudShowMsg__FPUsN20iii(title, textGetString__FPc(const_cast<char*>("MemcardCantSave")),
                               hudFooterUni, 128, 0, 0);
        if ((padWord() & padBackButton) != 0 || (netNetworkMode != 0 && levelName[0] != 0)) {
            msgHudSetPlayerBusy__Fcc(whichPad, 0);
            actionCoolOff = 15;
            MC_KickProtect = 0;
            undoBrowser__Fv();
            cancel();
        }
        break;
    default:
        break;
    }
}
