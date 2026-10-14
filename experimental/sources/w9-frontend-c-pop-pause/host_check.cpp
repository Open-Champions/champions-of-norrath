#include "pop_pause.cpp"
signed char netNetworkMode;
unsigned char pauseStack[8];
unsigned char pauseStackPos;
int gamePause = 99;
int frontEndRunning;
signed char particlesPaused = 77;
int gameControlPaused = 5;
#define CHECK(c, n) do { if (!(c)) return n; } while (0)
int main() {
    pauseStack[0] = 0; pauseStack[1] = 1; pauseStack[2] = 0xfe; pauseStack[3] = 0;
    pauseStackPos = 3;
    netNetworkMode = 1;  // network: nothing changes
    popPause__Fi(1);
    CHECK(pauseStackPos == 3 && gamePause == 99 && particlesPaused == 77 && gameControlPaused == 5, 1);
    netNetworkMode = 0;
    popPause__Fi(0);  // entry 2 = 0xfe: gamePause sign-extended, particlesPaused byte
    CHECK(pauseStackPos == 2 && gamePause == -2 && particlesPaused == -2 && gameControlPaused == 5, 2);
    frontEndRunning = 1;  // particlesPaused untouched, gamePause still written
    popPause__Fi(0);      // entry 1 = 1
    CHECK(pauseStackPos == 1 && gamePause == 1 && particlesPaused == -2 && gameControlPaused == 5, 3);
    frontEndRunning = 0;
    pauseStack[0] = 9;
    popPause__Fi(1);      // cleared before the read-back: zero value arms the timer
    CHECK(pauseStackPos == 0 && pauseStack[0] == 0 && gamePause == 0 && particlesPaused == 0 && gameControlPaused == 60, 4);
    return 0;
}
