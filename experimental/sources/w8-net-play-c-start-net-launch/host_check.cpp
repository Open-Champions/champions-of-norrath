#include "start_net_launch.cpp"
unsigned char netAttemptLaunch;
unsigned char netBroadCastStart;
unsigned char netGameLaunchInfo[328];
static int log[8];
static int n;
static int info5_at[8], attempt_at[8], bcast_at[8];
static void note(int id) {
    log[n] = id;
    info5_at[n] = netGameLaunchInfo[5];
    attempt_at[n] = netAttemptLaunch;
    bcast_at[n] = netBroadCastStart;
    ++n;
}
static int disable_arg;
extern "C" void engineSetDisplayDisable__Fi(int mode) { disable_arg = mode; note(1); }
extern "C" void engineSyncVBlank__Fv() { note(2); }
extern "C" void netInit__Fv() { note(3); }
extern "C" void netStartNetworkGameInit__Fv() { note(4); }
int main() {
    for (unsigned i = 0; i < sizeof netGameLaunchInfo; ++i) netGameLaunchInfo[i] = 0xa5;
    netAttemptLaunch = 0; netBroadCastStart = 0x77;
    startNetLaunch__Fv();
    if (n != 4) return 1;
    for (int i = 0; i < 4; ++i) if (log[i] != i + 1) return 2;
    if (disable_arg != 10) return 3;
    // flags are set before the first call and stay set
    for (int i = 0; i < 4; ++i) if (attempt_at[i] != 1 || bcast_at[i] != 0) return 4;
    // launch-info byte 5 is still untouched at net init and cleared before the last call
    if (info5_at[2] != 0xa5 || info5_at[3] != 0) return 5;
    for (unsigned i = 0; i < sizeof netGameLaunchInfo; ++i)
        if (i != 5 && netGameLaunchInfo[i] != 0xa5) return 6;
    return 0;
}
