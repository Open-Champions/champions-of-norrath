#include <cstdio>
extern "C" int amxGetSaveInhibit__FP5__amxPi(void*, int*);
extern "C" int gameRecallInhibit = 0;

int main() {
    gameRecallInhibit = 0;
    if (amxGetSaveInhibit__FP5__amxPi(nullptr, nullptr) != 0) return 1;
    gameRecallInhibit = -5;
    if (amxGetSaveInhibit__FP5__amxPi(nullptr, nullptr) != -5) return 2;
    gameRecallInhibit = 42;
    if (amxGetSaveInhibit__FP5__amxPi(nullptr, nullptr) != 42) return 3;
    std::puts("amxGetSaveInhibit passed");
    return 0;
}
