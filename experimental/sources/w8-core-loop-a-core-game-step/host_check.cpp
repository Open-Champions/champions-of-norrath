// Host check for coreGameStep__Fii. Not an EE plan input; calls into the other packages are mocks.
#include "core_game_step.cpp"
#include <cstdio>
#include <cstring>
#include <string>

extern "C" {
int gameControlPaused, gameControlDontResetPause, coreLoading, frameNm;
signed char netNetworkMode;
PadRecord pad[4];
}

static std::string trace;
static int pauseAtCall;  // padRunFrame call index (1-based) from which gameIsPaused returns 1
static int stepCalls;
static int padSeenNonZero;

extern "C" void engineRunTasks__Fii(int a, int b) {
    trace += "R";
    if (a != 1 || b != -1) trace += "!";
    ++stepCalls;
    if (stepCalls > 1)
        for (int i = 0; i < 4; ++i) if (pad[i].word12 || pad[i].word16) padSeenNonZero = 1;
}
extern "C" int gameIsPaused__Fv() { trace += "P"; return pauseAtCall && stepCalls >= pauseAtCall; }
extern "C" void enumerateAlivePlayers__Fv() {
    trace += "E";
    for (int i = 0; i < 4; ++i) if (pad[i].word12 || pad[i].word16) trace += "?";
}
extern "C" void gameCheckForDeathScreen__Fv() { trace += "D"; }
extern "C" void padRunFrame__Fv() { trace += "F"; }

static void reset() {
    std::memset(pad, 0, sizeof pad);
    for (int i = 0; i < 4; ++i) { pad[i].word12 = 100 + i; pad[i].word16 = 200 + i; }
    gameControlPaused = gameControlDontResetPause = coreLoading = frameNm = 0;
    netNetworkMode = 0;
    trace.clear();
    pauseAtCall = stepCalls = padSeenNonZero = 0;
}
static bool padsRestored() {
    for (int i = 0; i < 4; ++i) if (pad[i].word12 != 100 + i || pad[i].word16 != 200 + i) return false;
    return true;
}

int main() {
    // three steps: order of calls, frame counter, pad words cleared inside and restored after
    reset();
    coreGameStep__Fii(3, 99);
    if (trace != "RPEDFRPEDFRPEDF") { std::printf("trace %s\n", trace.c_str()); return 1; }
    if (frameNm != 3 || padSeenNonZero) return 2;
    if (!padsRestored()) return 3;
    // zero or negative step count: no calls, restore only, pause test still runs
    reset();
    gameControlPaused = 5;
    coreGameStep__Fii(0, 0);
    if (!trace.empty() || frameNm || !padsRestored()) return 4;
    if (gameControlPaused != 0) return 5;           // no pad holds: pause cleared
    reset();
    coreGameStep__Fii(-2, 0);
    if (!trace.empty()) return 6;
    // network mode: pause forced to 0, no gameIsPaused call, no frame count
    reset();
    netNetworkMode = 1;
    gameControlPaused = 7;
    coreGameStep__Fii(2, 0);
    if (gameControlPaused != 0 || frameNm != 0 || trace != "REDFREDF") return 7;
    reset();
    netNetworkMode = -1;
    gameControlPaused = 7;
    pad[0].flags = 1;
    coreGameStep__Fii(0, 0);
    if (gameControlPaused != 0) return 18;
    // paused game or loading suppresses the frame counter
    reset();
    pauseAtCall = 2;
    coreGameStep__Fii(3, 0);
    if (frameNm != 1) return 8;
    reset();
    coreLoading = 1;
    coreGameStep__Fii(2, 0);
    if (frameNm != 0 || trace != "RPEDFRPEDF") return 9;
    // pause countdown: positive, not held
    reset();
    gameControlPaused = 3;
    pad[2].flags = 0x00f00000;                      // only ignored bits: does not hold the pause
    coreGameStep__Fii(1, 0);
    if (gameControlPaused != 0) return 10;          // no pad holds: pause cleared to 0
    reset();
    gameControlPaused = 3;
    pad[2].flags = 0x00000001;
    coreGameStep__Fii(1, 0);
    if (gameControlPaused != 2) return 11;
    reset();
    gameControlPaused = 3;
    pad[3].flags = 0x01000000;
    coreGameStep__Fii(1, 0);
    if (gameControlPaused != 2) return 12;
    reset();
    gameControlPaused = 3;
    pad[1].flags = 0x00100000;                      // bit 20 lies in the ignored mask 0x00f00000
    coreGameStep__Fii(1, 0);
    if (gameControlPaused != 0) return 13;
    reset();
    gameControlPaused = 3;
    pad[0].flags = 0x00080000;                      // bit 19 is outside the ignored mask: holds
    coreGameStep__Fii(1, 0);
    if (gameControlPaused != 2) return 14;
    // held by gameControlDontResetPause, and nonpositive pause untouched
    reset();
    gameControlPaused = 3;
    gameControlDontResetPause = 1;
    coreGameStep__Fii(1, 0);
    if (gameControlPaused != 3) return 15;
    reset();
    gameControlPaused = -1;
    coreGameStep__Fii(1, 0);
    if (gameControlPaused != -1) return 16;
    // pause read again after the steps is the stored value, not changed by mocks
    reset();
    gameControlPaused = 1;
    pad[0].flags = 0x1;
    coreGameStep__Fii(2, 0);
    if (gameControlPaused != 0) return 17;
    return 0;
}
