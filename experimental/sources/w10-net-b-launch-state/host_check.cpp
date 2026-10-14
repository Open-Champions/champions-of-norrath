// Host check for the launch state storage. Not an EE plan input.
#include "launch_state.cpp"
int main() {
    if (udpLaunchConnection || netLaunchTimer || netLaunchExpirationCount) return 1;
    if (netNewLaunchMaybeQuit || netNewLaunchMaybeLaunch) return 2;
    if (sizeof udpLaunchConnection != 4 || sizeof netLaunchTimer != 4 || sizeof netLaunchExpirationCount != 4) return 3;
    if (sizeof netNewLaunchMaybeQuit != 1 || sizeof netNewLaunchMaybeLaunch != 1) return 4;
    return 0;
}
