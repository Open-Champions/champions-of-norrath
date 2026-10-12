#include "mc_kick.cpp"

#include <cstdio>

signed char MC_KickProtect;
int ranConfig = 1, ranBrowser = 1;
static int order, busy, mode_arg, keep_arg, closes, close_order, busy_order;
extern "C" int newMode__Fii(int mode, int keep) {
    mode_arg = mode;
    keep_arg = keep;
    order = 1;
    return 0;
}
extern "C" int MCA_Busy__Fv() { busy_order = order; order = 2; return busy; }
extern "C" void resetWithClose__Fv() { ++closes; close_order = order; }

int main() {
    MC_KickProtect = 1;
    MC_Kick__Fv();
    if (order != 0 || ranConfig != 1 || ranBrowser != 1) return 1;
    MC_KickProtect = 0;
    busy = 0;
    MC_Kick__Fv();
    if (mode_arg != -1 || keep_arg != 0 || ranConfig != 0 || ranBrowser != 0) return 2;
    if (busy_order != 1 || closes != 0) return 3;
    busy = 5;
    MC_Kick__Fv();
    if (closes != 1 || close_order != 2) return 4;
    std::puts("mc kick passed");
    return 0;
}
