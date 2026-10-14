// Observed: MC_Init stores its argument in disableMC, asks for the default
// config, and returns 1 after sceMcInit reports failure. Otherwise it resets
// the card state, sizes the act tables, clears iconHeader, writes the icon
// header, and registers the two async card tasks.
// Inferred: act records are 8-byte name/size pairs. The aligned size keeps
// the low 7 bits clear.
// Unknown: the meaning of the icon-header float slots.

extern "C" void MCA_Init__Fv();
extern "C" void gameSetDefaultGameConfig__Fi(int value);
extern "C" int sceMcInit();
extern "C" void MC_NewGame__Fi(int value);
extern "C" int sprintf(char* destination, const char* format, ...);
extern "C" char* strcpy(char* destination, const char* source);
extern "C" void* memset(void* destination, int value, unsigned size);
extern "C" void AsciiString2Sjis__FPUcPUs(unsigned char* ascii, unsigned short* sjis);
extern "C" void engineAddTask__FPFv_viPci(void (*callback)(), int priority, char* name, int argument);
extern "C" void MCA_Cmd__Fiiii(int command, const void* text, int third, int fourth);
extern "C" void MCA_Sync__FPi(int* error);
extern "C" void mcHeartBeatRun__Fv();
extern "C" void mcHeartBeat__Fv();

extern "C" int disableMC;
extern "C" int flareAlpha;
extern "C" unsigned char frontEndWasRunning;
extern "C" int curAct;
extern "C" int asyncMode;
extern "C" int oldAsyncMode;
extern "C" int configState;
extern "C" int lastCardPort;
extern "C" int lastCardSlot;
extern "C" unsigned char pauseStackPos;
extern "C" unsigned char keepPlayerPositions;
extern "C" unsigned char MC_SavingOrLoading;
extern "C" int browserState;
extern "C" unsigned char MC_LastModeSuccessful;
extern "C" int saveCheckPointCount;
extern "C" int flareDelta;
extern "C" unsigned char killPlayer;
extern "C" unsigned char MC_KickProtect;
extern "C" int MC_CheckPointAge;
extern "C" int slotFootPrint;
extern "C" int actCount;
extern "C" int actLevelCount[];
extern "C" char* allActs[];
extern "C" int _3map$autoMapBitsSize;
extern "C" int bareSlotFootPrint;
extern "C" int totalClusters;
extern "C" int errorCondition;
extern "C" int slotClusters;
extern "C" int totalFootPrint;
extern "C" unsigned char iconHeader[];
extern "C" unsigned char sortedTimes[];
extern "C" unsigned char MC_Debug;
extern "C" int MCA_Port;
extern "C" int MCA_Slot;
extern "C" int MCA_LastDirCount;

#ifdef NORRATH_HOST_ARENA
extern "C" const char* norrath_host_level_names[];
#endif

namespace {
void store_word(unsigned char* at, unsigned bits) {
    at[0] = static_cast<unsigned char>(bits);
    at[1] = static_cast<unsigned char>(bits >> 8);
    at[2] = static_cast<unsigned char>(bits >> 16);
    at[3] = static_cast<unsigned char>(bits >> 24);
}

int read_word(const unsigned char* at) {
    unsigned word = at[0] | (unsigned(at[1]) << 8) | (unsigned(at[2]) << 16) | (unsigned(at[3]) << 24);
    return static_cast<int>(word);
}

const char* level_name(const unsigned char* entry) {
    unsigned word = static_cast<unsigned>(read_word(entry));
#ifdef NORRATH_HOST_ARENA
    return norrath_host_level_names[word];
#else
    return reinterpret_cast<const char*>(word);
#endif
}
}

extern "C" int MC_Init__Fi(int argument) {
    MCA_Init__Fv();
    disableMC = argument;
    gameSetDefaultGameConfig__Fi(-1);
    if (disableMC == 0) {
        if (sceMcInit() != 0) {
            disableMC = 1;
            return 1;
        }
    }

    flareAlpha = 128;
    frontEndWasRunning = 0xffu;
    curAct = -1;
    asyncMode = -1;
    oldAsyncMode = -1;
    configState = -1;
    lastCardPort = -1;
    lastCardSlot = -1;
    pauseStackPos = 0;
    keepPlayerPositions = 0;
    MC_SavingOrLoading = 0;
    browserState = 0;
    MC_LastModeSuccessful = 0;
    saveCheckPointCount = 0;
    flareDelta = 0;
    killPlayer = 0;
    MC_KickProtect = 0;
    MC_CheckPointAge = 0;
    MC_NewGame__Fi(1);

    sprintf(reinterpret_cast<char*>(sortedTimes + 64), "%s", "BASLUS-20565");
    sprintf(reinterpret_cast<char*>(sortedTimes + 112), "%s_config", "BASLUS-20565");

    slotFootPrint = 0;
    if (actCount > 0) {
        int best = 0;
        char scratch[336];
        for (int act = 0; act < actCount; ++act) {
            int sum = 0;
            if (actLevelCount[act] > 0) {
                unsigned char* table = reinterpret_cast<unsigned char*>(allActs[act]);
                for (int level = 0; level < actLevelCount[act]; ++level) {
                    unsigned char* entry = table + level * 8;
                    int aligned = (read_word(entry + 4) + _3map$autoMapBitsSize + 1000) & ~127;
                    store_word(entry + 4, static_cast<unsigned>(aligned));
                    if (best < aligned) {
                        strcpy(scratch, level_name(entry));
                        best = aligned;
                    }
                    sum += aligned;
                }
            }
            if (slotFootPrint < sum) slotFootPrint = sum;
        }
    }

    bareSlotFootPrint = slotFootPrint + 648;
    totalClusters = 849;
    slotClusters = 715;
    totalFootPrint = 0xb6f5c;
    slotFootPrint = 0xb29c8;
    memset(iconHeader, 0, 0xb29c8u);

    store_word(iconHeader + 132, 0x3f75c28fu);
    store_word(iconHeader + 136, 0x3f6b851eu);
    store_word(iconHeader + 80, 0x3e199999u);
    store_word(iconHeader + 84, 0x3f000000u);
    store_word(iconHeader + 88, 0xbf000000u);
    store_word(iconHeader + 176, 0x3f1eb851u);
    store_word(iconHeader + 180, 0x3f2147aeu);
    store_word(iconHeader + 184, 0x3f35c28fu);
    store_word(iconHeader + 188, 0x3f800000u);
    store_word(iconHeader + 128, 0x3f800000u);
    store_word(iconHeader + 140, 0x3f800000u);
    store_word(iconHeader + 92, 0x3f800000u);
    iconHeader[0] = 0x50u;
    iconHeader[1] = 0x53u;
    iconHeader[2] = 0x32u;
    iconHeader[3] = 0x44u;
    iconHeader[6] = 26u;
    iconHeader[7] = 0;
    AsciiString2Sjis__FPUcPUs(reinterpret_cast<unsigned char*>(const_cast<char*>("Champions Of Norrath")),
                              reinterpret_cast<unsigned short*>(iconHeader + 192));
    const char icon_name[] = "icon.ico";
    for (int copy = 0; copy < 3; ++copy) {
        unsigned char* at = iconHeader + 260 + copy * 64;
        for (int index = 0; index < 9; ++index) at[index] = static_cast<unsigned char>(icon_name[index]);
    }
    engineAddTask__FPFv_viPci(mcHeartBeatRun__Fv, 20, const_cast<char*>("Async Load/Save Daemon RUN"), 0);
    engineAddTask__FPFv_viPci(mcHeartBeat__Fv, 20, const_cast<char*>("Async Load/Save Daemon DRAW"), 1);

    MC_Debug = 0;
    MCA_Port = 0;
    MCA_Slot = 0;
    MCA_Cmd__Fiiii(8, nullptr, 0, 0);
    MCA_Sync__FPi(&errorCondition);
    // The present and formatted table addresses are nonzero, so this command runs.
    MCA_Cmd__Fiiii(6, "debug.sht", 0, 0);
    MCA_Sync__FPi(&errorCondition);
    if (MCA_LastDirCount != 0) MC_Debug = 1;
    MCA_Slot = -1;
    MCA_Port = -1;
    return -1;
}
