#include <cstdio>

extern "C" int gameRecallInhibit = 0;
extern "C" int amxSetSaveInhibit__FP5__amxPi(void*, int*);

int main() {
    int params[2] = {4, 5};
    gameRecallInhibit = 9;
    if (amxSetSaveInhibit__FP5__amxPi(nullptr, params) != 0) return 1;
    if (gameRecallInhibit != 5) return 2;
    params[1] = -3;
    if (amxSetSaveInhibit__FP5__amxPi(nullptr, params) != 0) return 3;
    if (gameRecallInhibit != -3) return 4;
    params[1] = 0;
    if (amxSetSaveInhibit__FP5__amxPi(nullptr, params) != 0) return 5;
    if (gameRecallInhibit != 0) return 6;
    std::printf("set save inhibit passed\n");
    return 0;
}
