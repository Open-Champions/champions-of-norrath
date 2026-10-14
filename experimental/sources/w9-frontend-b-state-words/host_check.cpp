#include "state_words.cpp"

#include <cstdio>

int main() {
    if (sizeof curSel != 4 || sizeof configMode != 4 || sizeof inGameConfigShuttingDown != 4) return 1;
    if (sizeof emergencyMusicWaitAbort != 4 || sizeof dialogPacket != 4 || sizeof needEnforce != 4) return 2;
    if (sizeof gotPlayerStartThisLevel != 1 || sizeof hudDisplayNetCamToggle != 1) return 3;
    if (sizeof netForceLocalUpdate != 1 || sizeof deathState != 1 || sizeof deathNetHack != 1) return 4;
    if (sizeof needActAlign != 4) return 5;
    if (curSel || configMode || inGameConfigShuttingDown || emergencyMusicWaitAbort) return 6;
    if (dialogPacket || needEnforce || gotPlayerStartThisLevel || hudDisplayNetCamToggle) return 7;
    if (netForceLocalUpdate || deathState || deathNetHack) return 8;
    for (signed char b : needActAlign) if (b) return 9;
    std::puts("state words passed");
    return 0;
}
