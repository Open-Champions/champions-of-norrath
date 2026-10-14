#include <cstdio>

extern "C" int shopRunning = 0;
extern "C" int amxShopRunning__FP5__amxPi(void*, int*);

int main() {
    int params[1] = {0};
    shopRunning = 0;
    if (amxShopRunning__FP5__amxPi(nullptr, params) != 0) return 1;
    shopRunning = 7;
    if (amxShopRunning__FP5__amxPi(nullptr, params) != 7) return 2;
    shopRunning = -1;
    if (amxShopRunning__FP5__amxPi(nullptr, params) != -1) return 3;
    std::printf("shop running passed\n");
    return 0;
}
