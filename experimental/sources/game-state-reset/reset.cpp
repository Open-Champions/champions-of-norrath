// Observed gameResetGameState__Fv at 0x001cd1e0, 204 bytes.
// The byte at 0x00484eb8 has no ELF symbol. It is the only direct reference
// to that address in .text, and the image byte is 0. That byte is copied to
// the first byte of startEntranceName. Then these stores run, still before
// MC_NewGame(0): objectDrawEnable = 1, engineCurrentLevelName[0] = 0,
// levelName[242] = 0, gamePlayerLightAttenuate = 1.0f, floorDrawEnable = 1,
// gameAutoMapMode = 2, gameRecallInhibit = 0, cheatOn = 0, gameNoDraw = 0,
// hudNoDraw = 0, levelName[81] = 0, gameOver = 0.
// The calls after that, in order, are textInit, memset(levelSeeds, 0, 4096),
// commInit, removeAllQuestsFromLog, playerGlobalInit, scriptSuperClear,
// scriptClearAllGameState, and scriptSetGameState of the unnamed bytes
// "needPlayers" at 0x0046c9b0 with 1. levelSeeds is 4096 bytes. levelName is
// 322 bytes, so offsets 81 and 242 are inside it. Nothing here was executed.
extern "C" {
unsigned char norrath_game_state_byte_00484eb8 = 0;
}
extern "C" unsigned char startEntranceName[];
extern "C" unsigned char objectDrawEnable;
extern "C" unsigned char engineCurrentLevelName[];
extern "C" unsigned char levelName[];
extern "C" float gamePlayerLightAttenuate;
extern "C" unsigned char floorDrawEnable;
extern "C" int gameAutoMapMode;
extern "C" int gameRecallInhibit;
extern "C" unsigned char cheatOn;
extern "C" int gameNoDraw;
extern "C" unsigned char hudNoDraw;
extern "C" unsigned char gameOver;
extern "C" unsigned char levelSeeds[];
extern "C" void MC_NewGame__Fi(int value);
extern "C" void textInit__Fv();
extern "C" void* memset(void* destination, int value, unsigned count);
extern "C" void commInit__Fv();
extern "C" void removeAllQuestsFromLog__Fv();
extern "C" void playerGlobalInit__Fv();
extern "C" void scriptSuperClear__Fv();
extern "C" void scriptClearAllGameState__Fv();
extern "C" void scriptSetGameState__FPci(char* name, int value);

extern "C" void gameResetGameState__Fv() {
    startEntranceName[0] = norrath_game_state_byte_00484eb8;
    objectDrawEnable = 1;
    engineCurrentLevelName[0] = 0;
    levelName[242] = 0;
    gamePlayerLightAttenuate = 1.0f;
    floorDrawEnable = 1;
    gameAutoMapMode = 2;
    gameRecallInhibit = 0;
    cheatOn = 0;
    gameNoDraw = 0;
    hudNoDraw = 0;
    levelName[81] = 0;
    gameOver = 0;
    MC_NewGame__Fi(0);
    textInit__Fv();
    memset(levelSeeds, 0, 4096u);
    commInit__Fv();
    removeAllQuestsFromLog__Fv();
    playerGlobalInit__Fv();
    scriptSuperClear__Fv();
    scriptClearAllGameState__Fv();
    scriptSetGameState__FPci(const_cast<char*>("needPlayers"), 1);
}
