// Observed gameClear__Fi at 0x001cb5f8, 552 bytes.
// scePrintf receives the unnamed bytes "gameclear\n" at 0x0046c570.
// The unnamed bytes "game.cc" at 0x00484ce0 are the file argument of the
// integrity, heap, and stack checks. The line numbers are 436, 437, 438,
// 441, 442, then 478, 480, 583, and 584. shopActiveFrame is set to -10000
// before cutClear. Before worldSetFarClip, the body stores 0 in
// gameRecallInhibit, bossMeterCurrent, bossMeterMax, bossMeterAlpha,
// gameCameraShake, and gameShowEnemiesInMap; 0x80000000 in coreClearColor;
// 240 in the gameDeathTimer halfword; and 240 in each of the four halfwords
// of gameDeathTimerSingle. The far-clip argument bits are 0x44e10000, which
// are 1800.0f. gameHidePlayers is cleared in that call's delay.
// recallOn, helpMessageOn, lastNmLocalPlayersAlive, localNmPlayersAlive, and
// all four bytes of multiHelpMessageOn are then cleared. floorClear, cdClear,
// and P_Clear follow. After the 478 and 480 checks come drawListClear, P_Clear
// again, worldReset of the current world when that word is nonzero, then
// worldClear, lightClear, textureClear, fxClear, and gameLineClear.
// A nonzero argument calls worldAllocateWaterPatches(0) and lumpClear(0).
// The argument itself is not passed on. inventoryReset, scriptClear,
// DLG_Clear, jumpGateClear, and padClear follow. particlesPaused and
// gamePause are cleared before playerClearSpellEffect. world is cleared
// before the line 583 check. blockAllocInit is last. Nothing here was executed.
extern "C" void scePrintf(const char* text);
extern "C" void _listCheckAllIntegrity__FPCci(const char* file, int line);
extern "C" void memCheckHeapReal__FPCci(const char* file, int line);
extern "C" void _engineCheckStack__FPCci(const char* file, int line);
extern "C" void hudFinishGold__Fv();
extern "C" void hudClearInfoMessage__Fv();
extern "C" void cutClear__Fv();
extern "C" void worldSetFarClip__Ff(float distance);
extern "C" void floorClear__Fv();
extern "C" void cdClear__Fv();
extern "C" void P_Clear__Fv();
extern "C" void objectClear__Fv();
extern "C" void drawListClear__Fv();
extern "C" void worldReset__FP12_worldHeader(unsigned char* header);
extern "C" void worldClear__Fv();
extern "C" void lightClear__Fv();
extern "C" void textureClear__Fv();
extern "C" void fxClear__Fv();
extern "C" void gameLineClear__Fv();
extern "C" void worldAllocateWaterPatches__Fi(int value);
extern "C" void lumpClear__Fi(int value);
extern "C" void inventoryReset__Fv();
extern "C" void scriptClear__Fv();
extern "C" void DLG_Clear__Fv();
extern "C" void jumpGateClear__Fv();
extern "C" void padClear__Fv();
extern "C" void playerClearSpellEffect__Fv();
extern "C" void blockAllocInit__Fv();

extern "C" int shopActiveFrame;
extern "C" int gameRecallInhibit;
extern "C" int bossMeterCurrent;
extern "C" int bossMeterMax;
extern "C" int bossMeterAlpha;
extern "C" int gameCameraShake;
extern "C" int gameShowEnemiesInMap;
extern "C" unsigned coreClearColor;
extern "C" unsigned short gameDeathTimer;
extern "C" unsigned char gameDeathTimerSingle[8];
extern "C" unsigned char gameHidePlayers;
extern "C" unsigned char recallOn;
extern "C" unsigned char helpMessageOn;
extern "C" int lastNmLocalPlayersAlive;
extern "C" int localNmPlayersAlive;
extern "C" unsigned multiHelpMessageOn;
extern "C" unsigned char* world;
extern "C" unsigned char particlesPaused;
extern "C" int gamePause;

static void store_u16(unsigned char* address, unsigned value) {
    address[0] = static_cast<unsigned char>(value);
    address[1] = static_cast<unsigned char>(value >> 8);
}

extern "C" void gameClear__Fi(int release_level) {
    scePrintf("gameclear\n");
    _listCheckAllIntegrity__FPCci("game.cc", 436);
    memCheckHeapReal__FPCci("game.cc", 437);
    _engineCheckStack__FPCci("game.cc", 438);
    memCheckHeapReal__FPCci("game.cc", 441);
    _engineCheckStack__FPCci("game.cc", 442);
    hudFinishGold__Fv();
    hudClearInfoMessage__Fv();
    shopActiveFrame = -10000;
    cutClear__Fv();
    gameRecallInhibit = 0;
    bossMeterCurrent = 0;
    bossMeterMax = 0;
    bossMeterAlpha = 0;
    gameCameraShake = 0;
    gameShowEnemiesInMap = 0;
    coreClearColor = 0x80000000u;
    gameDeathTimer = 240;
    store_u16(gameDeathTimerSingle + 6, 240u);
    store_u16(gameDeathTimerSingle + 4, 240u);
    store_u16(gameDeathTimerSingle + 2, 240u);
    store_u16(gameDeathTimerSingle + 0, 240u);
    gameHidePlayers = 0;
    worldSetFarClip__Ff(1800.0f);
    recallOn = 0;
    helpMessageOn = 0;
    lastNmLocalPlayersAlive = 0;
    localNmPlayersAlive = 0;
    multiHelpMessageOn = 0;
    floorClear__Fv();
    cdClear__Fv();
    P_Clear__Fv();
    _listCheckAllIntegrity__FPCci("game.cc", 478);
    objectClear__Fv();
    _listCheckAllIntegrity__FPCci("game.cc", 480);
    drawListClear__Fv();
    P_Clear__Fv();
    if (world != 0) worldReset__FP12_worldHeader(world);
    worldClear__Fv();
    lightClear__Fv();
    textureClear__Fv();
    fxClear__Fv();
    gameLineClear__Fv();
    if (release_level != 0) {
        worldAllocateWaterPatches__Fi(0);
        lumpClear__Fi(0);
    }
    inventoryReset__Fv();
    scriptClear__Fv();
    DLG_Clear__Fv();
    jumpGateClear__Fv();
    padClear__Fv();
    particlesPaused = 0;
    gamePause = 0;
    playerClearSpellEffect__Fv();
    world = 0;
    _listCheckAllIntegrity__FPCci("game.cc", 583);
    _engineCheckStack__FPCci("game.cc", 584);
    blockAllocInit__Fv();
}
