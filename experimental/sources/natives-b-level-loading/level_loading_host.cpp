#include <cstdio>
extern "C" int amxLevelLoading__FP5__amxPi(void*, int*);
extern "C" int gameLevelLoading = 0;

int main() {
    gameLevelLoading = 0;
    if (amxLevelLoading__FP5__amxPi(nullptr, nullptr) != 0) return 1;
    gameLevelLoading = -2;
    if (amxLevelLoading__FP5__amxPi(nullptr, nullptr) != -2) return 2;
    std::puts("amxLevelLoading passed");
    return 0;
}
