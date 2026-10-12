// Observed: asyncSaveConfigSilent__Fv at 0x00256a38 is 748 bytes. With ranConfig zero it
// calls runConfigure__FiPUsi(0, text "MemcardEndGame", 0). Result -1 stores 1 in
// ranConfig, stores 101 in gameBailFromGameLoop when the byte saveConfigEndGame is
// nonzero, runs the closing sequence below and returns. A result other than 1
// returns with ranConfig untouched. Result 1 stores 1 in ranConfig and continues.
// With ranConfig set it returns when MCA_Busy__Fv is nonzero, otherwise dispatches
// on syncState. Unlike asyncSaveConfig__Fv it has no delayCount and shows no header.
// State 0 stores 1 in MC_KickProtect, queues MCA_Cmd(0, configFileName, 2, 0),
// MCA_Cmd(5, MC_GameConfig, 252, 0) and MCA_Cmd(1, 0, 0, 0), and adds one to syncState.
// State 1 stores 9 in syncState when errorCondition is nonzero or MCA_Result is
// negative; otherwise it stores 101 in gameBailFromGameLoop when saveConfigEndGame
// is nonzero, runs the closing sequence and stores zero in MC_KickProtect.
// State 9 formats hudFooterUni from text "MemcardBack" with (float)padBackButtonFontChar
// and seven zero floats, fills a stack title with memcardTitle__FPUs, and shows
// hudShowMsg(title, text "MemcardCantSaveConfig", hudFooterUni, 128, 0, 0). If the word at
// netLocalPad+12 has a padBackButton bit set, or netNetworkMode and the first
// byte of levelName are both nonzero, it stores 15 in actionCoolOff, zero in
// MC_KickProtect and runs the closing sequence.
// The closing sequence stores asyncMode in oldAsyncMode, -1 in asyncMode, 0 in
// syncState, and calls popPause__Fi(0); the original expands popPause inline.
// Inferred: nothing.
// Unknown: the numbering of other syncState values; the meaning of 101.

extern "C" unsigned short* textGetString__FPc(char* key);
extern "C" void usprintf__FPUsT0ffffffff(unsigned short* dest, unsigned short* format,
                                         float a, float b, float c, float d, float e,
                                         float f, float g, float h);
extern "C" int runConfigure__FiPUsi(int mode, unsigned short* title, int extra);
extern "C" int MCA_Busy__Fv();
extern "C" void MCA_Cmd__Fiiii(int command, const void* text, int third, int fourth);
extern "C" void memcardTitle__FPUs(unsigned short* title);
extern "C" void hudShowMsg__FPUsN20iii(unsigned short* title, unsigned short* text,
                                        unsigned short* footer, int a, int b, int c);
extern "C" void popPause__Fi(int clear);
extern "C" int ranConfig;
extern "C" int syncState;
extern "C" int errorCondition;
extern "C" int MCA_Result;
extern "C" int asyncMode;
extern "C" int oldAsyncMode;
extern "C" int actionCoolOff;
extern "C" int gameBailFromGameLoop;
extern "C" signed char saveConfigEndGame;
extern "C" signed char MC_KickProtect;
extern "C" signed char netNetworkMode;
extern "C" int padBackButton;
extern "C" int padBackButtonFontChar;
extern "C" char levelName[];
extern "C" char configFileName[];
extern "C" unsigned char MC_GameConfig[];
extern "C" unsigned char netLocalPad[];
extern "C" unsigned short hudFooterUni[];

namespace {
void finish() {
    oldAsyncMode = asyncMode;
    asyncMode = -1;
    syncState = 0;
    popPause__Fi(0);
}
}

extern "C" void asyncSaveConfigSilent__Fv() {
    unsigned short title[256];
    if (ranConfig == 0) {
        int result = runConfigure__FiPUsi(0, textGetString__FPc(const_cast<char*>("MemcardEndGame")), 0);
        if (result == -1) {
            ranConfig = 1;
            if (saveConfigEndGame != 0) {
                gameBailFromGameLoop = 101;
            }
            finish();
            return;
        }
        if (result != 1) {
            return;
        }
        ranConfig = result;
    }
    if (MCA_Busy__Fv() != 0) {
        return;
    }
    switch (syncState) {
    case 0:
        MC_KickProtect = 1;
        MCA_Cmd__Fiiii(0, configFileName, 2, 0);
        MCA_Cmd__Fiiii(5, MC_GameConfig, 252, 0);
        MCA_Cmd__Fiiii(1, 0, 0, 0);
        syncState = syncState + 1;
        break;
    case 1:
        if (errorCondition != 0 || MCA_Result < 0) {
            syncState = 9;
        } else {
            if (saveConfigEndGame != 0) {
                gameBailFromGameLoop = 101;
            }
            finish();
            MC_KickProtect = 0;
        }
        break;
    case 9: {
        usprintf__FPUsT0ffffffff(hudFooterUni, textGetString__FPc(const_cast<char*>("MemcardBack")),
                                 static_cast<float>(padBackButtonFontChar), 0.0f, 0.0f, 0.0f, 0.0f,
                                 0.0f, 0.0f, 0.0f);
        memcardTitle__FPUs(title);
        hudShowMsg__FPUsN20iii(title, textGetString__FPc(const_cast<char*>("MemcardCantSaveConfig")),
                               hudFooterUni, 128, 0, 0);
        const unsigned* pad = reinterpret_cast<const unsigned*>(netLocalPad + 12);
        if ((*pad & static_cast<unsigned>(padBackButton)) != 0 ||
            (netNetworkMode != 0 && levelName[0] != 0)) {
            actionCoolOff = 15;
            MC_KickProtect = 0;
            finish();
        }
        break;
    }
    default:
        break;
    }
}
