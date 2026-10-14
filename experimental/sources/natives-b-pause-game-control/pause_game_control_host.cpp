#include <cstdio>
extern "C" int amxPauseGameControl__FP5__amxPi(void*, int*);
extern "C" int gameControlDontResetPause = 0;
extern "C" int gameControlPaused = 0;

int main() {
    int cells[] = {4, -6};
    if (amxPauseGameControl__FP5__amxPi(nullptr, cells) != 0) return 1;
    if (gameControlDontResetPause != 1) return 2;
    if (gameControlPaused != -6) return 3;
    std::puts("amxPauseGameControl passed");
    return 0;
}
