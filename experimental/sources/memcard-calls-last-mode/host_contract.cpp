#include "last_mode.cpp"

#include <cstdio>

int main() {
    if (sizeof MC_LastModeSuccessful != 1 || MC_LastModeSuccessful != 0) return 1;
    MC_LastModeSuccessful = 1;
    if (MC_LastModeSuccessful != 1) return 2;
    std::puts("last mode passed");
    return 0;
}
