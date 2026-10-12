#include "mc_change_players.cpp"

#include <cstdio>

int disableMC, asyncMode, errorCondition;
signed char whichPad, MC_FrontEndImport, pcNmPlayers, pcLaunch;
unsigned short* pcTitle;
unsigned char MC_LastModeSuccessful = 9;
static int log[3], logn, new_mode, new_keep, last_at_stop;
extern "C" int stopCardQuery__Fv() { log[logn++] = 1; last_at_stop = MC_LastModeSuccessful; return 0; }
extern "C" int newMode__Fii(int m, int k) { log[logn++] = 2; new_mode = m; new_keep = k; return 0; }
extern "C" void pushPause__Fv() { log[logn++] = 3; }

static bool ran() { bool r = logn == 3 && log[0] == 1 && log[1] == 2 && log[2] == 3; logn = 0; return r; }

int main() {
    unsigned short title[2];
    asyncMode = 2;
    MC_ChangePlayers__FiPUsiii(1, title, 2, 3, 4);
    if (logn != 0 || whichPad != 0) return 1;
    asyncMode = -1;
    errorCondition = 7;
    MC_ChangePlayers__FiPUsiii(1, title, 2, 3, 4);
    if (logn != 0) return 2;
    errorCondition = 0;
    MC_ChangePlayers__FiPUsiii(1, title, 2, 3, 4);
    if (!ran() || whichPad != 1 || pcTitle != title || pcNmPlayers != 2 || pcLaunch != 3 || MC_FrontEndImport != 4) return 3;
    if (MC_LastModeSuccessful != 0 || last_at_stop != 0 || new_mode != 7 || new_keep != 0) return 4;
    disableMC = 1;
    asyncMode = 5;
    errorCondition = 5;
    MC_LastModeSuccessful = 9;
    MC_ChangePlayers__FiPUsiii(2, 0, 0x1ff, -1, 0);
    if (!ran() || whichPad != 2 || pcTitle != 0 || pcNmPlayers != -1 || pcLaunch != -1 || MC_FrontEndImport != 0) return 5;
    std::puts("mc change players passed");
    return 0;
}
