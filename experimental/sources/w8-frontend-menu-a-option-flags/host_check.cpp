#include <cstdio>
#include "option_flags.cpp"
static_assert(sizeof(gameControlDontResetPause) == 4 && sizeof(controlPaused) == 4, "word objects");
static_assert(sizeof(cheatOn) == 1 && sizeof(helpMessageOn) == 1 && sizeof(language) == 1 &&
                  sizeof(hudNoController) == 1, "byte objects");
int main() {
    if (gameControlDontResetPause || cheatOn || helpMessageOn || language || controlPaused || hudNoController)
        return 1;
    language = -3;
    if (language >= 0) return 2;  // signed, as the image's lb loads require
    hudNoController = 255;
    if (hudNoController != 255) return 3;
    controlPaused = 90;
    if (controlPaused != 90) return 4;
    std::puts("option flag words check passed");
    return 0;
}
