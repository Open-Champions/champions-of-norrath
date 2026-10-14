// Observed allDone__Fi at 0x0027dde0, 288 bytes, 32-byte frame. It leaves the player selection screens:
//  1. Camera::Pop on gameCam, then Camera::SetMode(gameCam, 1); playerSelRunning = 0; engineDisable30fps = 0;
//     engineRemoveTask(playerSelectionTask__Fv); lightPopSceneLights; engineSetDisplayDisable(10); engineSyncVBlank.
//  2. With a nonzero argument: needEnforce = 0 and the name taken is playerSelectPrevName (0x004389f8, 64 bytes).
//     With zero: scriptSetGameState("needPlayers", 0), the byte playerSelectSuccessful = 1, and the name is
//     playerSelectNextName (0x004389b8, 64 bytes).
//  3. If strcmp(name, "") is zero (the name is empty) gameBailFromGameLoop = 1; otherwise the byte at levelName + 80 is
//     set to 0xff and the name is copied into levelName with strcpy.
// Nothing is returned. The empty string is a literal in the image's string pool at 0x00485fe8.
// Inferred: the argument selects going back to the previous level name (nonzero) or forward to the next (zero); an
// empty name means leave the game loop. The camera, task and light calls undo what the selection setup did.
// Unknown: the exact role of the byte at levelName + 80, the argument's meaning, and the reason "needPlayers" is a
// game state name.
#if defined(__mips__)
#define LIBRARY_SYMBOL(name) __asm__(#name)
#else
#define LIBRARY_SYMBOL(name)
#endif

extern "C" {
extern void* gameCam;
extern int playerSelRunning;
extern int engineDisable30fps;
extern int needEnforce;
extern unsigned char playerSelectSuccessful;
extern int gameBailFromGameLoop;
extern char playerSelectPrevName[];
extern char playerSelectNextName[];
extern char levelName[];

void Pop__6Camera(void* camera);
void SetMode__6Camerai(void* camera, int mode);
void engineRemoveTask__FPFv_v(void (*task)());
void playerSelectionTask__Fv();
void lightPopSceneLights__Fv();
void engineSetDisplayDisable__Fi(int frames);
void engineSyncVBlank__Fv();
void scriptSetGameState__FPci(char* name, int value);
int compareText(const char* a, const char* b) LIBRARY_SYMBOL(strcmp);
char* copyText(char* destination, const char* source) LIBRARY_SYMBOL(strcpy);

void allDone__Fi(int previous) {
    Pop__6Camera(gameCam);
    SetMode__6Camerai(gameCam, 1);
    playerSelRunning = 0;
    engineDisable30fps = 0;
    engineRemoveTask__FPFv_v(playerSelectionTask__Fv);
    lightPopSceneLights__Fv();
    engineSetDisplayDisable__Fi(10);
    engineSyncVBlank__Fv();
    const char* name;
    if (previous) {
        needEnforce = 0;
        name = playerSelectPrevName;
    } else {
        scriptSetGameState__FPci(const_cast<char*>("needPlayers"), 0);
        playerSelectSuccessful = 1;
        name = playerSelectNextName;
    }
    if (compareText(name, "") == 0) {
        gameBailFromGameLoop = 1;
    } else {
        levelName[80] = static_cast<char>(-1);
        copyText(levelName, name);
    }
}
}
