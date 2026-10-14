// Observed coreGameStep__Fii at 0x002cee28, 544 bytes. The first argument is a step count; the second
// argument is not read (it is not preserved, and engineRunTasks is called with -1).
//   Entry: gameControlPaused is read; if the signed byte netNetworkMode is nonzero it is stored back as 0,
//   otherwise stored back unchanged. Words at +12 and +16 of the four 88-byte records of `pad` are saved.
//   Each step: engineRunTasks(1, -1); then, when netNetworkMode is 0, gameIsPaused() returns 0 and coreLoading
//   is 0, frameNm is incremented. The same two words in all four records are cleared. enumerateAlivePlayers()
//   and gameCheckForDeathScreen() run, then padRunFrame(). Between them the original runs a loop of five nop
//   words per iteration, nmPlayers times, guarded by gamePlayersPresent != 0, frontEndRunning == 0 and
//   nmPlayers > 0. It has no memory effect and is not reproduced (no inline assembly is allowed in sources).
//   After the steps (only when the count was positive) gameControlPaused is read again. When it is positive
//   and gameControlDontResetPause is 0, the first of the four records whose word +8 has any bit outside
//   0x00f00000 decrements it by one; if none does it is set to 0. Finally the saved words are written back.
// Inferred: the +12/+16 words are per-frame pad transition words (cleared each step, restored after); +8 is a
//   state word. No field names are claimed. Pad contents beyond those three words are unknown.
// Unknown: the meaning of the second argument, and what the nop loop was for.
struct PadRecord {
    int word0;
    int word4;
    unsigned flags;
    int word12;
    int word16;
    unsigned char rest[68];
};

extern "C" {
extern int gameControlPaused;
extern int gameControlDontResetPause;
extern signed char netNetworkMode;
extern int coreLoading;
extern int frameNm;
extern PadRecord pad[4];
void engineRunTasks__Fii(int, int);
int gameIsPaused__Fv();
void enumerateAlivePlayers__Fv();
void gameCheckForDeathScreen__Fv();
void padRunFrame__Fv();
}

extern "C" void coreGameStep__Fii(int steps, int) {
    int paused = gameControlPaused;
    if (netNetworkMode) paused = 0;
    gameControlPaused = paused;

    int saved12[4];
    int saved16[4];
    for (int i = 0; i < 4; ++i) {
        saved12[i] = pad[i].word12;
        saved16[i] = pad[i].word16;
    }

    if (steps > 0) {
        int remaining = steps;
        do {
            engineRunTasks__Fii(1, -1);
            if (!netNetworkMode && !gameIsPaused__Fv() && !coreLoading) frameNm = frameNm + 1;
            for (int i = 3; i >= 0; --i) {
                pad[i].word12 = 0;
                pad[i].word16 = 0;
            }
            enumerateAlivePlayers__Fv();
            gameCheckForDeathScreen__Fv();
            padRunFrame__Fv();
            --remaining;
        } while (remaining != 0);
        paused = gameControlPaused;
    }

    if (paused > 0 && !gameControlDontResetPause) {
        int i = 0;
        while (i < 4 && !(pad[i].flags & 0xff0fffffu)) ++i;
        gameControlPaused = (i < 4) ? paused - 1 : 0;
    }

    for (int i = 0; i < 4; ++i) {
        pad[i].word12 = saved12[i];
        pad[i].word16 = saved16[i];
    }
}
