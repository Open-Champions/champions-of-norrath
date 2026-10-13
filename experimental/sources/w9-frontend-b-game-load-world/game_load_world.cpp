// Observed: gameLoadWorld__FPc at 0x001cbc48 is 2864 bytes with a 5904-byte frame
// (its largest locals are a roughly 5120-byte buffer used for the "ambient.bin"
// text, three small file-name buffers and a 256-byte full-level-name buffer).
// With name the argument, in order it:
//  1. sets gameLevelLoading = 1; memCheckHeapReal__FPCci("game.cc", 747) and
//     _engineCheckStack__FPCci("game.cc", 749); fresh = 1.
//  2. If name equals the string at levelName + 242, scans the 128 32-byte entries
//     of levelSeeds from the start, stopping at the first entry whose text at +0
//     equals levelName + 81 and whose text at +16 equals levelName + 161, or at
//     the first entry with an empty text at +0; fresh is cleared if the scan ran
//     off the end, or if the stopped-at entry has the int at +28 equal to
//     megatileSeed and the signed byte at +24 equal to megatileRot.
//  3. If MC_GetAsyncMode__Fv() is 7, fresh = 1; gameClear__Fi(fresh); scePrintf("qa\n").
//  4. If name contains "kelsel" or equals "intro", lumpClear__Fi(10),
//     gamePlayersPresent = 0 and playerDeallocateArmorCaches__Fv(); otherwise, if
//     lumpQuery__FPc("hud.lmp") is zero, it does lumpClear__Fi(9), init__4itemv(),
//     gameInitPostFrontend__Fv() and lumpLoad__FPc of "fx.lmp", "hud.lmp",
//     "mapicons.lmp" and "icons.lmp", then lumpLock__Fi(10); then, if
//     loadPlayerLumps__Fv() is nonzero, gameClear__Fi(1) and fresh = 1; and
//     gamePlayersPresent = 1.
//  5. If name equals "kelsel2": lumpLoad__FPc("hud.lmp"), HUDInit__Fv().
//     _engineCheckStack("game.cc", 816); coreStartLoadingScreen__Fv();
//     enginePushBorderColor__FUi(0xff0000); scePrintf("qb\n").
//  6. Finds a record of the 60-byte table list.1073 (below): first the record whose
//     name equals name + "_" + (levelName + 161); else the first whose name
//     compares equal to name over strlen(name) characters (strncmp); else the first
//     whose name is contained in name (strstr); a record with a null name ends the
//     table and is then the result.
//  7. scePrintf("qc\n"); _engineCheckStack(.., 959); megatile_ramkmax = record
//     word 12; _engineCheckStack(.., 961); if fresh: sprintf(buffer, "%s.gob",
//     name), scePrintf("qc1\n"), lumpLoadGlob__FPc(buffer), scePrintf("qc2\n").
//  8. _engineCheckStack(.., 969); enginePopBorderColor__Fv(); push 0xffaaaa;
//     scePrintf("qd\n"); sprintf(buffer, "%s.lmp", name); lump = lumpLoad(buffer);
//     world = the pointer at +4 of lumpFindByType__FPvPci(lump, "world", 0);
//     eRandState = syncRandState.
//  9. If playerRecalled and recallEnter are both zero: take dir =
//     soundGetDialogDir__Fv(); walk its 80-byte entries until one has a zero first
//     byte, looking for the first whose text contains "enttag"; if found,
//     soundStartMusic__FPci(entry, 2) and set the music timer to
//     (next eRandState >> 16) % 1200 + 1800 through gameSetMusicTimer__Fi, where
//     eRandState = eRandState * 0x19660d + 0x3c6ef35f; if no music was started,
//     soundFadeStream__Fi(0). (If recallEnter is nonzero or playerRecalled is
//     nonzero this whole step is skipped.)
// 10. scePrintf("qe\n"); sprintf(buffer, "%s.lmp", name); lump = lumpLoad(buffer); if
//     lump and lumpFind__FPvPc(lump, "ambient.bin") are nonzero, copies the entry's
//     bytes (data pointer at +4, size at +8) into the ambient buffer, terminates it
//     with a zero byte (the store sits in the call's delay slot, so it happens before
//     the call) and calls soundSetAmbientData__FPc on it, else
//     soundSetAmbientData__FPc(0).
// 11. _engineCheckStack(.., 1037); if fresh and name does not contain "kelsel",
//     worldAllocateWaterPatches__Fi(record word 32).
// 12. groundType = record word 36; levelFlags = record word 44; with v = float at
//     record +4 plus 2.0f: worldDifficulty = v when gameDifficulty is neither 2 nor
//     3; v * 0.75f + 20.0f when it is 2; v * 0.75f + 35.0f when it is 3.
// 13. scePrintf("qf\n"); worldTileEntityLightScale = float at +40; domeLump =
//     lumpQuery("dome.lmp"); lump = lumpLoad("waterenv.lmp"); worldSetWaterProperties__FP8_texture
//     with the pointer at +4 of lumpFindByType(lump, "tex", 0);
//     modelSetGlobalShadowBrightness__Ff(float at +8); gameDeathHeight = float at +16,
//     gameMinCamHeight = float at +20, gameMaxCamHeight = float at +24; if the word at
//     +28 is nonzero and blockAlloc__Fi(92) returns a block, __7Weatheri(block, that
//     word); soundSetAudioEffect__Fiiiii(h0, h1, h1, h2, h3) with the halfwords at +48
//     to +54; numEnvironments = the unsigned halfword at +56.
// 14. Clears the 32 bytes of worldActivityLevel; lastMusicState = -1;
//     timeLastTransition = 0xff676981; startScorePos = currScorePos = 0;
//     currActivityScore = 0.
// 15. If name does not contain "kelsel": init__3mapi(0) and the map placement
//     globals are loaded from loc, scale and scissor: with m = gameAutoMapMode and
//     k = 0 when nmPlayers is less than 2 and 6 otherwise, mapX = loc[2m + k],
//     mapY = loc[2m + k + 1], mapScale = scale[m], mapScissorX = scissor[2m + k],
//     mapScissorY = scissor[2m + k + 1].
// 16. _engineCheckStack(.., 1099); boxSetEmpty__FP3Box(&worldBoundingBox); for each of
//     the world's count (word 0) 60-byte items starting at the pointer in word 9
//     (offset 36), boxAdd__FP3BoxT0(&worldBoundingBox, item + 8).
// 17. scePrintf("qg\n"); routeClear__Fv(); gameGetFullLevelName__FPc(full);
//     scePrintf("qg0\n"); _listCheckAllIntegrity__FPCci(.., 1121); scePrintf("qg1\n");
//     enginePopBorderColor; push 0xaaaaff; MC_LoadLevelEntities__FPcT0(name, full);
//     scePrintf("qg2\n"); integrity check 1129; scePrintf("qg3\n");
//     routeMakeLattice__Fv(); scePrintf("qg4\n"); if name contains "skirmish",
//     skirmishInit__Fv(); scePrintf("qg5\n"); worldDeactivateUntakenDoors__FP12_worldHeader(world);
//     scePrintf("qg6\n"); gameLineFuse__Fv(); scePrintf("qg7\n"); soundFrame__Fv() twice;
//     scePrintf("qh\n"); scriptActivatePostAction__Fv(); _engineCheckStack(.., 1155);
//     gameLevelLoading = 0; worldAllocateSegments__FP12_worldHeader(world); scePrintf("qi\n").
// 18. lightSetSceneLights__FG6Point3iiiiiiiii is called with a copy of the Point3 at
//     lightStart + 24 and the ints (lightStart[3], [4], [5], 0, [0], [1], [2], 0, k)
//     where k is 128 by default, 32 if levelFlags has 0x20000000, and 64 if it has
//     0x04000000 (the latter wins); then the integrity check 1173,
//     _engineCheckStack(.., 1174), MC_ResetAutoCheckPointSave__FPc(name),
//     enginePopBorderColor__Fv().
// Inferred: this is the level-load driver; the "q*" prints are progress markers.
// Unknown: the meaning of the list.1073 fields beyond how they are used here, how the
// 63-record table (3780 bytes) is populated (it stays an unresolved import under its
// own symbol name), what the first-byte tests at levelName + 81, + 161 and + 242
// represent, and the exact types of domeLump and the lump functions' returns.

struct Point3 { float x, y, z; };
struct Box;
struct LumpEntry { int pad0; void* data; int size; };
struct World { int count; int pad[8]; unsigned char* items; };

struct LevelRecord {
    const char* name;
    float forWorldDifficulty;
    float shadowBrightness;
    int megatileRamkmax;
    float deathHeight;
    float minCamHeight;
    float maxCamHeight;
    int weatherArg;
    int waterPatchesArg;
    int groundType;
    float tileEntityLightScale;
    int levelFlags;
    short audio[4];
    unsigned short numEnvironments;
    short pad;
};

extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" char* strstr(const char* a, const char* b);
extern "C" unsigned long strlen(const char* s);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" char* strcat(char* dst, const char* src);
extern "C" void* memcpy(void* dst, const void* src, unsigned long n);
extern "C" int sprintf(char* out, const char* format, ...);
extern "C" int scePrintf(const char* format, ...);
extern "C" void memCheckHeapReal__FPCci(const char* file, int line);
extern "C" void _engineCheckStack__FPCci(const char* file, int line);
extern "C" void _listCheckAllIntegrity__FPCci(const char* file, int line);
extern "C" int MC_GetAsyncMode__Fv();
extern "C" void gameClear__Fi(int mode);
extern "C" void* lumpQuery__FPc(const char* name);
extern "C" void lumpClear__Fi(int group);
extern "C" void init__4itemv();
extern "C" void gameInitPostFrontend__Fv();
extern "C" void* lumpLoad__FPc(const char* name);
extern "C" void lumpLock__Fi(int group);
extern "C" int loadPlayerLumps__Fv();
extern "C" void HUDInit__Fv();
extern "C" void playerDeallocateArmorCaches__Fv();
extern "C" void coreStartLoadingScreen__Fv();
extern "C" void enginePushBorderColor__FUi(unsigned color);
extern "C" void enginePopBorderColor__Fv();
extern "C" void lumpLoadGlob__FPc(const char* name);
extern "C" LumpEntry* lumpFindByType__FPvPci(void* lump, const char* type, int index);
extern "C" LumpEntry* lumpFind__FPvPc(void* lump, const char* name);
extern "C" char* soundGetDialogDir__Fv();
extern "C" void soundStartMusic__FPci(char* name, int mode);
extern "C" void gameSetMusicTimer__Fi(int time);
extern "C" void soundFadeStream__Fi(int mode);
extern "C" void soundSetAmbientData__FPc(char* text);
extern "C" void worldAllocateWaterPatches__Fi(int n);
extern "C" void worldSetWaterProperties__FP8_texture(void* texture);
extern "C" void modelSetGlobalShadowBrightness__Ff(float v);
extern "C" void* blockAlloc__Fi(int size);
extern "C" void __7Weatheri(void* self, int arg);
extern "C" void soundSetAudioEffect__Fiiiii(int a, int b, int c, int d, int e);
extern "C" void init__3mapi(int mode);
extern "C" void boxSetEmpty__FP3Box(Box* box);
extern "C" void boxAdd__FP3BoxT0(Box* box, void* other);
extern "C" void routeClear__Fv();
extern "C" void gameGetFullLevelName__FPc(char* out);
extern "C" void MC_LoadLevelEntities__FPcT0(char* name, char* full);
extern "C" void routeMakeLattice__Fv();
extern "C" void skirmishInit__Fv();
extern "C" void worldDeactivateUntakenDoors__FP12_worldHeader(World* world);
extern "C" void gameLineFuse__Fv();
extern "C" void soundFrame__Fv();
extern "C" void scriptActivatePostAction__Fv();
extern "C" void worldAllocateSegments__FP12_worldHeader(World* world);
extern "C" void lightSetSceneLights__FG6Point3iiiiiiiii(const Point3* p, int a, int b, int c,
                                                       int d, int e, int f, int g, int h, int i);
extern "C" void MC_ResetAutoCheckPointSave__FPc(char* name);

extern "C" int gameLevelLoading;
extern "C" char levelName[];
extern "C" unsigned char levelSeeds[];
extern "C" int megatileSeed;
extern "C" int megatileRot;
extern "C" int gamePlayersPresent;
extern "C" LevelRecord levelList[] __asm__("list.1073");
extern "C" int megatile_ramkmax;
extern "C" int eRandState;
extern "C" int syncRandState;
extern "C" signed char playerRecalled;
extern "C" signed char recallEnter;
extern "C" World* world;
extern "C" int groundType;
extern "C" int levelFlags;
extern "C" int gameDifficulty;
extern "C" float worldDifficulty;
extern "C" float worldTileEntityLightScale;
extern "C" void* domeLump;
extern "C" float gameDeathHeight;
extern "C" float gameMinCamHeight;
extern "C" float gameMaxCamHeight;
extern "C" unsigned short numEnvironments;
extern "C" unsigned char worldActivityLevel[];
extern "C" int lastMusicState;
extern "C" int timeLastTransition;
extern "C" short startScorePos;
extern "C" short currScorePos;
extern "C" int currActivityScore;
extern "C" int gameAutoMapMode;
extern "C" int nmPlayers;
extern "C" float loc[];
extern "C" float scale[];
extern "C" float scissor[];
extern "C" float mapX;
extern "C" float mapY;
extern "C" float mapScale;
extern "C" float mapScissorX;
extern "C" float mapScissorY;
extern "C" Box worldBoundingBox;
extern "C" int lightStart[];

extern "C" void gameLoadWorld__FPc(char* name) {
    char buffer[160];
    char gob[80];
    char lmp1[80];
    char lmp2[80];
    char ambient[5120];
    char fullName[256];
    int fresh;
    gameLevelLoading = 1;
    memCheckHeapReal__FPCci("game.cc", 747);
    fresh = 1;
    _engineCheckStack__FPCci("game.cc", 749);
    if (strcmp(name, levelName + 242) == 0) {
        int i;
        for (i = 0; i < 128; ++i) {
            unsigned char* entry = levelSeeds + 32 * i;
            if (strcmp(reinterpret_cast<char*>(entry), levelName + 81) == 0 &&
                strcmp(reinterpret_cast<char*>(entry) + 16, levelName + 161) == 0)
                break;
            if (strlen(reinterpret_cast<char*>(entry)) == 0) break;
        }
        if (i == 128) {
            fresh = 0;
        } else {
            unsigned char* entry = levelSeeds + 32 * i;
            if (*reinterpret_cast<int*>(entry + 28) == megatileSeed &&
                static_cast<signed char>(entry[24]) == megatileRot)
                fresh = 0;
        }
    }
    if (MC_GetAsyncMode__Fv() == 7) fresh = 1;
    gameClear__Fi(fresh);
    scePrintf("qa\n");
    if (strstr(name, "kelsel") == 0 && strcmp(name, "intro") != 0) {
        if (lumpQuery__FPc("hud.lmp") == 0) {
            lumpClear__Fi(9);
            init__4itemv();
            gameInitPostFrontend__Fv();
            lumpLoad__FPc("fx.lmp");
            lumpLoad__FPc("hud.lmp");
            lumpLoad__FPc("mapicons.lmp");
            lumpLoad__FPc("icons.lmp");
            lumpLock__Fi(10);
        }
        if (loadPlayerLumps__Fv() != 0) {
            fresh = 1;
            gameClear__Fi(1);
        }
        gamePlayersPresent = 1;
    } else {
        lumpClear__Fi(10);
        gamePlayersPresent = 0;
        playerDeallocateArmorCaches__Fv();
    }
    if (strcmp(name, "kelsel2") == 0) {
        lumpLoad__FPc("hud.lmp");
        HUDInit__Fv();
    }
    _engineCheckStack__FPCci("game.cc", 816);
    coreStartLoadingScreen__Fv();
    enginePushBorderColor__FUi(0xff0000u);
    scePrintf("qb\n");
    strcpy(buffer, name);
    strcat(buffer, "_");
    strcat(buffer, levelName + 161);
    int index = 0;
    while (levelList[index].name != 0 && strcmp(buffer, levelList[index].name) != 0) ++index;
    if (levelList[index].name == 0) {
        strcpy(buffer, name);
        index = 0;
        while (levelList[index].name != 0 &&
               strncmp(buffer, levelList[index].name, strlen(buffer)) != 0)
            ++index;
        if (levelList[index].name == 0) {
            strcpy(buffer, name);
            index = 0;
            while (levelList[index].name != 0 && strstr(buffer, levelList[index].name) == 0)
                ++index;
        }
    }
    const LevelRecord* record = &levelList[index];
    scePrintf("qc\n");
    _engineCheckStack__FPCci("game.cc", 959);
    megatile_ramkmax = record->megatileRamkmax;
    _engineCheckStack__FPCci("game.cc", 961);
    if (fresh != 0) {
        sprintf(gob, "%s.gob", name);
        scePrintf("qc1\n");
        lumpLoadGlob__FPc(gob);
        scePrintf("qc2\n");
    }
    _engineCheckStack__FPCci("game.cc", 969);
    enginePopBorderColor__Fv();
    enginePushBorderColor__FUi(0xffaaaau);
    scePrintf("qd\n");
    sprintf(lmp1, "%s.lmp", name);
    void* lump = lumpLoad__FPc(lmp1);
    LumpEntry* worldEntry = lumpFindByType__FPvPci(lump, "world", 0);
    int syncState = syncRandState;
    world = static_cast<World*>(worldEntry->data);
    eRandState = syncState;
    if (playerRecalled == 0 && recallEnter == 0) {
        bool started = false;
        char* dir = soundGetDialogDir__Fv();
        if (dir != 0) {
            int i = 0;
            for (char* entry = dir; *entry != 0; entry += 80, ++i) {
                if (strstr(entry, "enttag") == 0) continue;
                char* chosen = dir + 80 * i;
                soundStartMusic__FPci(chosen, 2);
                started = true;
                unsigned state = static_cast<unsigned>(eRandState) * 0x19660du + 0x3c6ef35fu;
                eRandState = static_cast<int>(state);
                gameSetMusicTimer__Fi(static_cast<int>((state >> 16) % 1200u) + 1800);
                break;
            }
        }
        if (!started) soundFadeStream__Fi(0);
    }
    scePrintf("qe\n");
    sprintf(lmp2, "%s.lmp", name);
    void* ambientLump = lumpLoad__FPc(lmp2);
    if (ambientLump != 0 && lumpFind__FPvPc(ambientLump, "ambient.bin") != 0) {
        LumpEntry* entry = lumpFind__FPvPc(ambientLump, "ambient.bin");
        memcpy(ambient, entry->data, entry->size);
        ambient[entry->size] = 0;
        soundSetAmbientData__FPc(ambient);
    } else {
        soundSetAmbientData__FPc(0);
    }
    _engineCheckStack__FPCci("game.cc", 1037);
    if (fresh != 0 && strstr(name, "kelsel") == 0) worldAllocateWaterPatches__Fi(record->waterPatchesArg);
    float base = record->forWorldDifficulty + 2.0f;
    groundType = record->groundType;
    levelFlags = record->levelFlags;
    if (gameDifficulty == 2) base = base * 0.75f + 20.0f;
    else if (gameDifficulty == 3) base = base * 0.75f + 35.0f;
    worldDifficulty = base;
    scePrintf("qf\n");
    worldTileEntityLightScale = record->tileEntityLightScale;
    domeLump = lumpQuery__FPc("dome.lmp");
    void* water = lumpLoad__FPc("waterenv.lmp");
    worldSetWaterProperties__FP8_texture(lumpFindByType__FPvPci(water, "tex", 0)->data);
    modelSetGlobalShadowBrightness__Ff(record->shadowBrightness);
    gameDeathHeight = record->deathHeight;
    gameMinCamHeight = record->minCamHeight;
    gameMaxCamHeight = record->maxCamHeight;
    if (record->weatherArg != 0) {
        void* block = blockAlloc__Fi(92);
        if (block != 0) __7Weatheri(block, record->weatherArg);
    }
    soundSetAudioEffect__Fiiiii(record->audio[0], record->audio[1], record->audio[1],
                                record->audio[2], record->audio[3]);
    numEnvironments = record->numEnvironments;
    for (int k = 31; k >= 0; --k) worldActivityLevel[k] = 0;
    lastMusicState = -1;
    timeLastTransition = static_cast<int>(0xff676981u);
    startScorePos = 0;
    currScorePos = 0;
    currActivityScore = 0;
    if (strstr(name, "kelsel") == 0) {
        init__3mapi(0);
        int m = gameAutoMapMode;
        int k = nmPlayers < 2 ? 0 : 6;
        mapX = loc[2 * m + k];
        mapY = loc[2 * m + k + 1];
        mapScale = scale[m];
        mapScissorX = scissor[2 * m + k];
        mapScissorY = scissor[2 * m + k + 1];
    }
    _engineCheckStack__FPCci("game.cc", 1099);
    boxSetEmpty__FP3Box(&worldBoundingBox);
    for (int n = 0; n < world->count; ++n) boxAdd__FP3BoxT0(&worldBoundingBox, world->items + 60 * n + 8);
    scePrintf("qg\n");
    routeClear__Fv();
    gameGetFullLevelName__FPc(fullName);
    scePrintf("qg0\n");
    _listCheckAllIntegrity__FPCci("game.cc", 1121);
    scePrintf("qg1\n");
    enginePopBorderColor__Fv();
    enginePushBorderColor__FUi(0xaaaaffu);
    MC_LoadLevelEntities__FPcT0(name, fullName);
    scePrintf("qg2\n");
    _listCheckAllIntegrity__FPCci("game.cc", 1129);
    scePrintf("qg3\n");
    routeMakeLattice__Fv();
    scePrintf("qg4\n");
    if (strstr(name, "skirmish") != 0) skirmishInit__Fv();
    scePrintf("qg5\n");
    worldDeactivateUntakenDoors__FP12_worldHeader(world);
    scePrintf("qg6\n");
    gameLineFuse__Fv();
    scePrintf("qg7\n");
    soundFrame__Fv();
    soundFrame__Fv();
    scePrintf("qh\n");
    scriptActivatePostAction__Fv();
    _engineCheckStack__FPCci("game.cc", 1155);
    gameLevelLoading = 0;
    worldAllocateSegments__FP12_worldHeader(world);
    scePrintf("qi\n");
    int lightK = 128;
    if ((levelFlags & 0x20000000) != 0) lightK = 32;
    if ((levelFlags & 0x04000000) != 0) lightK = 64;
    Point3 sceneDir = *reinterpret_cast<const Point3*>(lightStart + 6);
    lightSetSceneLights__FG6Point3iiiiiiiii(&sceneDir, lightStart[3], lightStart[4], lightStart[5], 0,
                                            lightStart[0], lightStart[1], lightStart[2], 0, lightK);
    _listCheckAllIntegrity__FPCci("game.cc", 1173);
    _engineCheckStack__FPCci("game.cc", 1174);
    MC_ResetAutoCheckPointSave__FPc(name);
    enginePopBorderColor__Fv();
}
