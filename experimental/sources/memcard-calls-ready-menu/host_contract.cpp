#include "ready_menu.cpp"

#include <cstdio>

static int quick;

extern "C" void resetQuickLoad__Fv() { ++quick; }

int main() {
    MC_GetReadyForMainMenu__Fv();
    MC_GetReadyForMainMenu__Fv();
    if (quick != 2) return 1;
    std::puts("ready for main menu passed");
    return 0;
}
