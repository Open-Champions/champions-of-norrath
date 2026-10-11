#include <cstdio>

extern "C" int amxSetupPlayerSel__FP5__amxPi(void*, int*);

static int setups;

extern "C" void playerSelectionSetup__Fv() { setups += 1; }

int main() {
    int params[2] = {4, 9};
    if (amxSetupPlayerSel__FP5__amxPi(nullptr, params) != 0) return 1;
    if (setups != 1) return 2;
    if (amxSetupPlayerSel__FP5__amxPi(nullptr, params) != 0) return 3;
    if (setups != 2) return 4;
    std::printf("setup player sel passed\n");
    return 0;
}
