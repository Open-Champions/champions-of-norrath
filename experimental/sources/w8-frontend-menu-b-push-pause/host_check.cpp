#include "push_pause.cpp"

#include <cstdio>

extern "C" signed char netNetworkMode = 0;
extern "C" unsigned char pauseStack[8] = {};
extern "C" unsigned char pauseStackPos = 0;
extern "C" int gamePause = 0;
extern "C" int frontEndRunning = 0;
extern "C" signed char particlesPaused = 0;

static int failures;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL line %d: %s\n", __LINE__, #c); ++failures; } } while (0)

int main() {
    // Normal push: saves the low byte of gamePause, bumps the index, then pauses.
    gamePause = 0x1234;
    pushPause__Fv();
    CHECK(pauseStack[0] == 0x34);
    CHECK(pauseStackPos == 1);
    CHECK(gamePause == 1);
    CHECK(particlesPaused == 1);

    // A second push saves the pause word as it now stands.
    particlesPaused = 0;
    pushPause__Fv();
    CHECK(pauseStack[1] == 1);
    CHECK(pauseStackPos == 2);

    // Front end running: the stack grows but nothing is paused.
    frontEndRunning = 1;
    gamePause = 0x200;
    particlesPaused = 0;
    pushPause__Fv();
    CHECK(pauseStack[2] == 0);
    CHECK(pauseStackPos == 3);
    CHECK(gamePause == 0x200);
    CHECK(particlesPaused == 0);

    // Network game: no effect at all.
    netNetworkMode = 2;
    frontEndRunning = 0;
    pushPause__Fv();
    CHECK(pauseStackPos == 3);
    CHECK(gamePause == 0x200);
    CHECK(particlesPaused == 0);

    // A negative network mode byte is also nonzero.
    netNetworkMode = -1;
    pushPause__Fv();
    CHECK(pauseStackPos == 3);

    if (failures == 0) std::printf("push_pause ok\n");
    return failures != 0;
}
