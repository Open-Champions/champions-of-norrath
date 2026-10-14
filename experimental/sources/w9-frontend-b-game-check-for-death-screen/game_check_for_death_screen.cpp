// Observed: gameCheckForDeathScreen__Fv at 0x001cd650 is 368 bytes.
// It returns at once if the signed byte cutMode is nonzero. Otherwise:
//  1. If gamePlayersPresent is nonzero and nmPlayersAlive is zero: it returns if
//     nmPlayers is also zero; if gameIsPaused__Fv() is zero it stores 10 in
//     padFilter, and when the halfword gameDeathTimer is positive it decrements
//     it. If the decremented value is zero then: when strcmp(levelName + 81,
//     the string at groundType + 256) is zero and nmPlayers is 1, it stores 0 in
//     gamePlayersPresent, calls scriptSetGameState__FPci("whichAct", -1), copies
//     the nine bytes "actintro" and its terminator to levelName, and stores
//     -1 in the byte levelName[80]; in every other case it calls
//     gameNetKick__Fi(0) and then gameDeathStart__Fi(0).
//  2. Then (also reached directly when gamePlayersPresent is zero or
//     nmPlayersAlive is nonzero, and when the game is paused): if nmPlayers is
//     nonzero, differs from nmPlayersAlive and is positive, it visits players
//     0 to nmPlayers-1. For each whose float at offset 172 of its 23984-byte
//     record in player is <= 0.0, it decrements the halfword
//     gameDeathTimerSingle[i] when that halfword is positive (and the code that
//     clamps a negative result to zero cannot trigger).
// Inferred: this decides, each frame, when the death screen starts and counts
// down the per-player timers; the 23984-byte stride matches the player record
// size used elsewhere.
// Unknown: the owner and size of the table behind groundType (the image's symbol
// has size 4; the bytes at groundType + 256 read "hate3"), what the float at
// offset 172 of a player record is, and why the equal-level case ends the
// session rather than starting the death sequence.

extern "C" int strcmp(const char* a, const char* b);
extern "C" int gameIsPaused__Fv();
extern "C" void gameNetKick__Fi(int a);
extern "C" void gameDeathStart__Fi(int a);
extern "C" void scriptSetGameState__FPci(char* name, int value);
extern "C" signed char cutMode;
extern "C" int gamePlayersPresent;
extern "C" int nmPlayersAlive;
extern "C" int nmPlayers;
extern "C" int padFilter;
extern "C" short gameDeathTimer;
extern "C" short gameDeathTimerSingle[];
extern "C" char levelName[];
extern "C" char groundType[];
extern "C" unsigned char player[];

extern "C" void gameCheckForDeathScreen__Fv() {
    if (cutMode != 0) return;
    if (gamePlayersPresent != 0 && nmPlayersAlive == 0) {
        if (nmPlayers == 0) return;
        if (gameIsPaused__Fv() == 0) {
            padFilter = 10;
            if (gameDeathTimer > 0) {
                if (--gameDeathTimer == 0) {
                    if (strcmp(levelName + 81, groundType + 256) == 0 && nmPlayers == 1) {
                        gamePlayersPresent = 0;
                        scriptSetGameState__FPci(const_cast<char*>("whichAct"), -1);
                        __builtin_memcpy(levelName, "actintro", 9);
                        levelName[80] = -1;
                    } else {
                        gameNetKick__Fi(0);
                        gameDeathStart__Fi(0);
                    }
                }
            }
        }
    }
    int count = nmPlayers;
    if (count == 0 || count == nmPlayersAlive) return;
    for (int i = 0; i < count; ++i) {
        if (*reinterpret_cast<float*>(player + 172 + 23984 * i) <= 0.0f) {
            if (gameDeathTimerSingle[i] > 0) {
                if (--gameDeathTimerSingle[i] < 0) gameDeathTimerSingle[i] = 0;
            }
        }
    }
}
