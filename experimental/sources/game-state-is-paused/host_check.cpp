// Host contract for gameIsPaused. Not an EE plan input.
#include "is_paused.cpp"

#include <cstdio>

extern "C" unsigned char netNetworkMode = 0;
extern "C" int gamePause = 0;
extern "C" unsigned char cutMode = 0;
extern "C" int coreLoading = 0;
extern "C" int frameNm = 0;
extern "C" unsigned char hudNoController = 0;

static int failures = 0;
static int dialog_calls = 0;
static int dialog_result = 0;
static unsigned char cut_mode_during_dialog = 0;

static void expect(int ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

extern "C" int DLG_Active__Fv() {
    ++dialog_calls;
    cutMode = cut_mode_during_dialog;
    return dialog_result;
}

static void clear_state() {
    netNetworkMode = 0;
    gamePause = 0;
    cutMode = 0;
    coreLoading = 0;
    frameNm = 100;
    hudNoController = 0;
    dialog_calls = 0;
    dialog_result = 0;
    cut_mode_during_dialog = 0;
}

int main() {
    clear_state();
    netNetworkMode = 1;
    gamePause = 0;
    cutMode = 1;
    coreLoading = 9;
    frameNm = 0;
    expect(gameIsPaused__Fv() == 0, "network mode returns a zero pause word");
    expect(dialog_calls == 0, "network mode does not ask the dialog");

    gamePause = 5;
    expect(gameIsPaused__Fv() == 5, "network mode returns the pause word");
    gamePause = -3;
    expect(gameIsPaused__Fv() == -3, "network mode keeps a negative pause word");
    netNetworkMode = 0x80;
    gamePause = 2;
    expect(gameIsPaused__Fv() == 2, "any nonzero network byte takes that path");

    clear_state();
    cutMode = 1;
    coreLoading = 7;
    gamePause = 4;
    frameNm = 100;
    expect(gameIsPaused__Fv() == 7, "cut mode returns the loading word");
    expect(dialog_calls == 0, "cut mode returns before the dialog");
    coreLoading = 0;
    expect(gameIsPaused__Fv() == 0, "cut mode can return a zero loading word");
    coreLoading = -2;
    expect(gameIsPaused__Fv() == -2, "cut mode keeps a negative loading word");

    clear_state();
    frameNm = 59;
    gamePause = 5;
    coreLoading = 5;
    hudNoController = 1;
    expect(gameIsPaused__Fv() == 0 && dialog_calls == 0, "frame 59 returns 0");
    frameNm = -1;
    expect(gameIsPaused__Fv() == 0, "a negative frame returns 0");

    clear_state();
    frameNm = 60;
    gamePause = 5;
    expect(gameIsPaused__Fv() == 1 && dialog_calls == 0, "a later pause word becomes 1");

    clear_state();
    frameNm = 60;
    coreLoading = 4;
    expect(gameIsPaused__Fv() == 1, "a later loading word becomes 1");

    clear_state();
    frameNm = 60;
    hudNoController = 0;
    dialog_result = 1;
    expect(gameIsPaused__Fv() == 0 && dialog_calls == 0, "no controller returns before the dialog");

    clear_state();
    frameNm = 60;
    hudNoController = 1;
    dialog_result = 1;
    expect(gameIsPaused__Fv() == 0 && dialog_calls == 1, "an active dialog returns 0");

    clear_state();
    frameNm = 60;
    hudNoController = 1;
    dialog_result = 0;
    cut_mode_during_dialog = 1;
    expect(gameIsPaused__Fv() == 0 && dialog_calls == 1, "cut mode set by the dialog returns 0");

    clear_state();
    frameNm = 60;
    hudNoController = 1;
    expect(gameIsPaused__Fv() == 1 && dialog_calls == 1, "a clear late state returns 1");

    if (failures != 0) return 1;
    std::puts("gameIsPaused contract passed");
    return 0;
}
