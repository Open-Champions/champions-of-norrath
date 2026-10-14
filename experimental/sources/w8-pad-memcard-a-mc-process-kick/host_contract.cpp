#include "mc_process_kick.cpp"

#include <cstdio>

signed char MC_KickProtect;
static int beats;
static signed char seen;
extern "C" void mcHeartBeat__Fv() { ++beats; seen = MC_KickProtect; }

int main() {
    MC_KickProtect = 0;
    if (mcProcessKick__Fv() != 1 || beats != 1) return 1;
    MC_KickProtect = 1;
    if (mcProcessKick__Fv() != 0 || beats != 2) return 2;
    MC_KickProtect = -1;
    if (mcProcessKick__Fv() != 0 || seen != -1) return 3;
    std::puts("mc process kick passed");
    return 0;
}
