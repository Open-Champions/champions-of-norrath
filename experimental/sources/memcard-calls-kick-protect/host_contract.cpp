#include "kick_protect.cpp"

#include <cstdio>

int main() {
    if (sizeof MC_KickProtect != 1 || MC_KickProtect != 0) return 1;
    MC_KickProtect = 1;
    if (MC_KickProtect != 1) return 2;
    std::puts("kick protect passed");
    return 0;
}
