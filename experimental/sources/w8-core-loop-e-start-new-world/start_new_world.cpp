// Observed startNewWorld__Fv at 0x002cd1e0, 1244 bytes. Its caller is the
// level start in gameLoop. levelName (0x0034b660) holds the requested level
// name at +0, with working strings at +81, +161 and +242 and single bytes at
// +80 and +241. The text "core.cc" is passed to _engineCheckStack with line
// numbers 285, 302, 349 and 351 at the places noted below.
//
// 1. (285) With levelName[81] set and no "deathTask" engine task, the full
//    level name is built into a local buffer and MC_SaveLevelEntities is
//    called with it. If that returns nonzero the function stores a zero word
//    at the address given by engineTaskActive's result, which is zero on this
//    path, then clears levelName[0] and returns. This null-address store is
//    observed in the instructions and kept here through an opaque pointer.
// 2. (302) A local flag is set to zero and, when frontEndRunning is zero, to
//    MC_SetActByLevelName(levelName). MC_CheckPlayerImportOrChange and
//    playerSelectionEnforce run. levelName[242] receives "%s_%s" of levelName
//    +81 and +161 when +161 is set, otherwise a copy of +81. The part of
//    levelName before the first '_' is copied to +81 and terminated; the
//    text after that '_' is copied to +161, or +161 is cleared when there is
//    no '_'. levelName[0] is cleared and engineCurrentLevelName (0x0037f988)
//    receives the +81 text. levelName[241] takes the old byte at +80, which
//    becomes 0xfe. soundDisable(0.05f) runs.
// 3. (349) gameLoadWorld(levelName+81). (351) With playerRecalled set and
//    nmPlayers positive, the x and y floats of nmPlayers entries of
//    recallPos (12-byte entries) are copied to playerStartPos and
//    playerRecalled is cleared. With nmPlayers positive, every playerStartPos
//    entry's z becomes worldFindHeight(world, entry, 12.0f).
// 4. scePrintf(">>  %d %d\n", gamePlayersPresent). With gamePlayersPresent
//    and nmPlayers positive, each player index i runs: for i > 0 when
//    gotPlayerStartThisLevel is not 3 the start position is moved to the free
//    spot from objectFindEmptySpot(position, 36, 14, 0), playerStartYaw[i]
//    gains 8000 (16 bits) and the z is refitted with worldFindHeight(...,
//    12.0f). scePrintf("    %f\n", fptodp(float at player+172)) follows. When
//    that float is above zero and netPlayerIsConnected(i), a 1408-byte
//    blockAlloc result, if non-null, is constructed as a Player from the
//    start position, the signed 16-bit yaw, i and 0. When the local flag from
//    step 2 is set, MC_GetAsyncMode is not 3 and there is no "deathTask" task,
//    the five bytes "None\0" are written to player+36 of that index and
//    removeAllQuestsFromLog runs. nmPlayers is read again at every loop end.
// 5. recallEnter is cleared before MC_HasThisLevelBeenSaved. If the level was
//    not saved, coreLevelCommentTimer becomes 300. A nonzero commEnable is then
//    cleared and coreHeadsetResetTimer set to 20.
// Inferred: playerStartPos/Yaw are per-player start data and recallPos holds
// their recalled positions. Unknown: the Player constructor's last two
// arguments, the meaning of the local flag, and why step 1 stores through the
// null result.
struct Point3 {
    float x, y, z;
};
extern "C" {
void _engineCheckStack__FPCci(const char* file, int line);
int engineTaskActive__FPc(const char* name);
void gameGetFullLevelName__FPc(char* output);
int MC_SaveLevelEntities__FPc(char* name);
int MC_SetActByLevelName__FPc(char* name);
void MC_CheckPlayerImportOrChange__Fv();
void playerSelectionEnforce__Fv();
int sprintf(char* destination, const char* format, ...);
char* strcpy(char* destination, const char* source);
unsigned strlen(const char* text);
void soundDisable__Ff(float value);
void gameLoadWorld__FPc(char* name);
float worldFindHeight__FP12_worldHeaderG6Point3f(void* world, const Point3* position, float limit);
int scePrintf(const char* format, ...);
unsigned long long fptodp(float value);
void objectFindEmptySpot__FG6Point3iiPc(Point3* result, const Point3* position, int a, int b, char* c);
int netPlayerIsConnected__Fi(int index);
void* blockAlloc__Fi(int size);
void* __6PlayerG6Point3sii(void* player, const Point3* position, short yaw, int index, int zero);
int MC_GetAsyncMode__Fv();
void removeAllQuestsFromLog__Fv();
int MC_HasThisLevelBeenSaved__Fv();
extern int frontEndRunning;
extern int nmPlayers;
extern int gamePlayersPresent;
extern signed char playerRecalled;
extern signed char recallEnter;
extern signed char gotPlayerStartThisLevel;
extern int commEnable;
extern int coreLevelCommentTimer;
extern int coreHeadsetResetTimer;
extern void* world;
extern char levelName[];
extern char engineCurrentLevelName[];
extern Point3 recallPos[4];
extern Point3 playerStartPos[4];
extern unsigned short playerStartYaw[4];
extern unsigned char player[];
}

#if defined(NORRATH_HOST_CONTRACT)
extern "C" void norrath_host_store_zero_word(unsigned address);
#endif

namespace {
const unsigned kPlayerStride = 23984u;

void storeZeroWord(int address) {
#if defined(NORRATH_HOST_CONTRACT)
    norrath_host_store_zero_word(static_cast<unsigned>(address));
#else
    int* target = reinterpret_cast<int*>(static_cast<__UINTPTR_TYPE__>(address));
    // Hide the value so the null address is stored rather than treated as a fault.
    asm volatile("" : "+r"(target));
    *target = 0;
#endif
}
}

extern "C" void startNewWorld__Fv() {
    char fullName[256];
    _engineCheckStack__FPCci("core.cc", 285);
    if (levelName[81] != 0) {
        const int task = engineTaskActive__FPc("deathTask");
        if (task == 0) {
            gameGetFullLevelName__FPc(fullName);
            if (MC_SaveLevelEntities__FPc(fullName) != 0) {
                storeZeroWord(task);
                levelName[0] = 0;
                return;
            }
        }
    }
    _engineCheckStack__FPCci("core.cc", 302);
    int actResult = 0;
    if (frontEndRunning == 0) actResult = MC_SetActByLevelName__FPc(levelName);
    MC_CheckPlayerImportOrChange__Fv();
    playerSelectionEnforce__Fv();
    if (levelName[161] != 0)
        sprintf(levelName + 242, "%s_%s", levelName + 81, levelName + 161);
    else
        strcpy(levelName + 242, levelName + 81);
    int length = 0;
    const int total = static_cast<int>(strlen(levelName));
    while (length < total && levelName[length] != '_') {
        levelName[81 + length] = levelName[length];
        ++length;
    }
    levelName[81 + length] = 0;
    if (levelName[length] == '_')
        strcpy(levelName + 161, levelName + length + 1);
    else
        levelName[161] = 0;
    levelName[0] = 0;
    strcpy(engineCurrentLevelName, levelName + 81);
    const char previous = levelName[80];
    levelName[80] = static_cast<char>(0xfe);
    levelName[241] = previous;
    soundDisable__Ff(0.05f);
    _engineCheckStack__FPCci("core.cc", 349);
    gameLoadWorld__FPc(levelName + 81);
    _engineCheckStack__FPCci("core.cc", 351);
    int count = nmPlayers;
    if (playerRecalled != 0 && count > 0) {
        for (int i = 0; i < count; ++i) {
            playerStartPos[i].x = recallPos[i].x;
            playerStartPos[i].y = recallPos[i].y;
        }
        playerRecalled = 0;
    }
    if (count > 0) {
        for (int i = 0; i < nmPlayers; ++i) {
            const Point3 position = playerStartPos[i];
            playerStartPos[i].z = worldFindHeight__FP12_worldHeaderG6Point3f(world, &position, 12.0f);
        }
    }
    scePrintf(">>  %d %d\n", gamePlayersPresent);
    if (gamePlayersPresent != 0 && nmPlayers > 0) {
        for (int i = 0; i < nmPlayers; ++i) {
            unsigned char* const slot = player + i * kPlayerStride;
            if (i != 0 && gotPlayerStartThisLevel != 3) {
                Point3 found;
                const Point3 request = playerStartPos[i];
                objectFindEmptySpot__FG6Point3iiPc(&found, &request, 36, 14, nullptr);
                playerStartPos[i] = found;
                playerStartYaw[i] = static_cast<unsigned short>(playerStartYaw[i] + 8000);
                const Point3 refit = playerStartPos[i];
                playerStartPos[i].z = worldFindHeight__FP12_worldHeaderG6Point3f(world, &refit, 12.0f);
            }
            const float health = *reinterpret_cast<float*>(slot + 172);
            scePrintf("    %f\n", fptodp(health));
            if (*reinterpret_cast<float*>(slot + 172) > 0.0f && netPlayerIsConnected__Fi(i) != 0) {
                void* const block = blockAlloc__Fi(1408);
                if (block != nullptr) {
                    const Point3 start = playerStartPos[i];
                    __6PlayerG6Point3sii(block, &start, static_cast<short>(playerStartYaw[i]), i, 0);
                }
            }
            if (actResult != 0) {
                if (MC_GetAsyncMode__Fv() != 3 && engineTaskActive__FPc("deathTask") == 0) {
                    const char none[5] = {'N', 'o', 'n', 'e', '\0'};
                    for (int k = 0; k < 5; ++k) slot[36 + k] = static_cast<unsigned char>(none[k]);
                    removeAllQuestsFromLog__Fv();
                }
            }
        }
    }
    recallEnter = 0;
    const int saved = MC_HasThisLevelBeenSaved__Fv();
    if (saved == 0) coreLevelCommentTimer = 300;
    if (commEnable != 0) {
        commEnable = 0;
        coreHeadsetResetTimer = 20;
    }
}
