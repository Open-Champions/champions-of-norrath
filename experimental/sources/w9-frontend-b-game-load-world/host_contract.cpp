#include "game_load_world.cpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

LevelRecord levelList[4] __asm__("list.1073");
int gameLevelLoading, megatileSeed, megatileRot, gamePlayersPresent, megatile_ramkmax, eRandState,
    syncRandState, groundType, levelFlags, gameDifficulty, lastMusicState, timeLastTransition,
    currActivityScore, gameAutoMapMode, nmPlayers;
char levelName[512];
unsigned char levelSeeds[128 * 32];
signed char playerRecalled, recallEnter;
World* world;
float worldDifficulty, worldTileEntityLightScale, gameDeathHeight, gameMinCamHeight, gameMaxCamHeight;
float mapX, mapY, mapScale, mapScissorX, mapScissorY;
float loc[24], scale[4], scissor[24];
void* domeLump;
unsigned short numEnvironments;
unsigned char worldActivityLevel[32];
short startScorePos, currScorePos;
int lightStart[9];
struct Box { char bytes[24]; };
Box worldBoundingBox;
static std::string events;
static int async_mode, query_hud, loaded_players, boxes_added;
static float shadow;
static int music_timer = -1, fade_calls, weather_arg, water_patches = -1, ambient_set;
static unsigned char worldBlob[64 + 2 * 60];
static LumpEntry worldEntry, texEntry, ambientEntry;
static char ambientBytes[8] = "ab";
static char dialogDir[3 * 80];

template <class T> static void note(const T& s) { events += s; events += ";"; }

extern "C" {
int scePrintf(const char* f, ...) { note(std::string("p:") + f); return 0; }
void memCheckHeapReal__FPCci(const char*, int) {}
void _engineCheckStack__FPCci(const char*, int) {}
void _listCheckAllIntegrity__FPCci(const char*, int) {}
int MC_GetAsyncMode__Fv() { return async_mode; }
void gameClear__Fi(int m) { note("clear" + std::to_string(m)); }
void* lumpQuery__FPc(const char*) { return query_hud ? &query_hud : 0; }
void lumpClear__Fi(int g) { note("lclear" + std::to_string(g)); }
void init__4itemv() {}
void gameInitPostFrontend__Fv() {}
void* lumpLoad__FPc(const char* n) { note(std::string("load:") + n); return worldBlob; }
void lumpLock__Fi(int) {}
int loadPlayerLumps__Fv() { return loaded_players; }
void HUDInit__Fv() {}
void playerDeallocateArmorCaches__Fv() { note("dealloc"); }
void coreStartLoadingScreen__Fv() {}
void enginePushBorderColor__FUi(unsigned) {}
void enginePopBorderColor__Fv() {}
void lumpLoadGlob__FPc(const char* n) { note(std::string("glob:") + n); }
LumpEntry* lumpFindByType__FPvPci(void*, const char* t, int) { return std::strcmp(t, "world") == 0 ? &worldEntry : &texEntry; }
LumpEntry* lumpFind__FPvPc(void*, const char*) { return &ambientEntry; }
char* soundGetDialogDir__Fv() { return dialogDir; }
void soundStartMusic__FPci(char* n, int) { note(std::string("music:") + n); }
void gameSetMusicTimer__Fi(int t) { music_timer = t; }
void soundFadeStream__Fi(int) { ++fade_calls; }
void soundSetAmbientData__FPc(char* t) { ambient_set = t ? static_cast<int>(std::strlen(t)) : -1; }
void worldAllocateWaterPatches__Fi(int n) { water_patches = n; }
void worldSetWaterProperties__FP8_texture(void*) {}
void modelSetGlobalShadowBrightness__Ff(float v) { shadow = v; }
void* blockAlloc__Fi(int) { static char block[92]; return block; }
void __7Weatheri(void*, int a) { weather_arg = a; }
void soundSetAudioEffect__Fiiiii(int a, int b, int c, int d, int e) {
    note("fx" + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + "," + std::to_string(d) + "," + std::to_string(e));
}
void init__3mapi(int) { note("mapinit"); }
void boxSetEmpty__FP3Box(Box*) {}
void boxAdd__FP3BoxT0(Box*, void*) { ++boxes_added; }
void routeClear__Fv() {}
void gameGetFullLevelName__FPc(char* o) { std::strcpy(o, "full"); }
void MC_LoadLevelEntities__FPcT0(char*, char*) {}
void routeMakeLattice__Fv() {}
void skirmishInit__Fv() { note("skirmish"); }
void worldDeactivateUntakenDoors__FP12_worldHeader(World*) {}
void gameLineFuse__Fv() {}
void soundFrame__Fv() {}
void scriptActivatePostAction__Fv() {}
void worldAllocateSegments__FP12_worldHeader(World*) {}
void lightSetSceneLights__FG6Point3iiiiiiiii(const Point3* p, int a, int, int, int d, int e, int, int, int h, int i) {
    note("light" + std::to_string(i) + "," + std::to_string(a) + "," + std::to_string(e) + "," + std::to_string(d + h) + "," + std::to_string(static_cast<int>(p->y)));
}
void MC_ResetAutoCheckPointSave__FPc(char*) { note("autosave"); }
}

int main() {
    // The world blob: count 2, item pointer at word 9 (offset 36)
    std::memset(worldBlob, 0, sizeof worldBlob);
    int count = 2;
    std::memcpy(worldBlob, &count, 4);
    unsigned char* items = worldBlob + 64;
    std::memcpy(worldBlob + 36, &items, sizeof items);
    worldEntry.data = worldBlob;
    ambientBytes[0] = 'a'; ambientBytes[1] = 'b';
    ambientEntry.data = ambientBytes; ambientEntry.size = 2;
    levelList[0] = LevelRecord{"cave", 3.0f, 0.5f, 99, 1.0f, 2.0f, 3.0f, 7, 5, 11, 0.25f, 0x20000000, {1, 2, 3, 4}, 6, 0};
    levelList[1] = LevelRecord{"cave_x", 4.0f, 0.5f, 98, 1.0f, 2.0f, 3.0f, 0, 6, 12, 0.25f, 0x04000000, {5, 6, 7, 8}, 9, 0};
    levelList[2] = LevelRecord{0, 8.0f, 0.5f, 97, 1.0f, 2.0f, 3.0f, 0, 3, 13, 0.25f, 0, {0, 0, 0, 0}, 1, 0};
    lightStart[0] = 10; lightStart[3] = 20; lightStart[4] = 30; lightStart[5] = 40;
    float py = 5.0f;
    std::memcpy(lightStart + 7, &py, 4);
    for (int i = 0; i < 24; ++i) { loc[i] = 100.0f + i; scissor[i] = 200.0f + i; }
    for (int i = 0; i < 4; ++i) scale[i] = 300.0f + i;
    std::strcpy(levelName + 81, "sub");
    std::strcpy(levelName + 161, "x");
    std::strcpy(levelName + 242, "cave");
    std::memcpy(dialogDir, "aa\0", 3);
    std::strcpy(dialogDir + 80, "an enttag here");
    char name[16];
    std::strcpy(name, "cave");
    // 1. exact name+suffix record 1, difficulty 2, lights from the 0x04000000 flag, one player
    gameDifficulty = 2;
    nmPlayers = 1; gameAutoMapMode = 1;
    syncRandState = 5;
    megatileSeed = 5;
    eRandState = 0;
    query_hud = 1;
    gameLoadWorld__FPc(name);
    if (megatile_ramkmax != 98 || groundType != 12 || levelFlags != 0x04000000) return 1;
    if (worldDifficulty != (4.0f + 2.0f) * 0.75f + 20.0f) return 2;
    if (mapX != 102.0f || mapY != 103.0f || mapScale != 301.0f || mapScissorX != 202.0f || mapScissorY != 203.0f) return 3;
    if (events.find("fx5,6,6,7,8;") == std::string::npos) return 4;
    if (events.find("light64,20,10,0,5;") == std::string::npos) return 5;
    if (events.find("music:an enttag here;") == std::string::npos || music_timer < 1800 || music_timer >= 3000) return 6;
    if (water_patches != 6 || numEnvironments != 9 || weather_arg != 0) return 7;
    if (ambient_set != 2 || boxes_added != 2 || gameLevelLoading != 0 || lastMusicState != -1) return 8;
    if (events.find("load:cave.lmp;") == std::string::npos || events.find("glob:cave.gob;") == std::string::npos) return 9;
    // 2. kelsel names skip the player lumps and the map; a missing suffix match falls back to the prefix scan
    events.clear(); weather_arg = 0; water_patches = -1;
    std::strcpy(name, "caveXX");
    nmPlayers = 3;
    gameDifficulty = 3;
    gameLoadWorld__FPc(name);
    // "caveXX_x" matches nothing exactly; the strncmp pass needs the record to start with the whole name; the
    // strstr pass finds "cave" inside "caveXX"
    if (megatile_ramkmax != 99 || worldDifficulty != (3.0f + 2.0f) * 0.75f + 35.0f) return 10;
    if (weather_arg != 7 || water_patches != 5) return 11;
    if (mapX != 108.0f || mapY != 109.0f) return 12;  // two or more players use the second column group
    std::strcpy(name, "kelsel");
    events.clear();
    gameLoadWorld__FPc(name);
    if (events.find("dealloc;") == std::string::npos || gamePlayersPresent != 0) return 13;
    std::puts("game load world passed");
    return 0;
}
