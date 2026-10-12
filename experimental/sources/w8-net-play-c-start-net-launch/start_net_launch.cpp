// Observed startNetLaunch__Fv at 0x001ca920, 68 bytes. In order: store 1 to the
// byte netAttemptLaunch and 0 to the byte netBroadCastStart, call
// engineSetDisplayDisable__Fi(10), engineSyncVBlank__Fv(), netInit__Fv(), store
// 0 to byte 5 of netGameLaunchInfo (a 328-byte object), then call
// netStartNetworkGameInit__Fv(). Return values of the calls are not used. What
// the display-disable argument or the launch-info byte mean is not established.
extern "C" {
extern unsigned char netAttemptLaunch;
extern unsigned char netBroadCastStart;
extern unsigned char netGameLaunchInfo[];
void engineSetDisplayDisable__Fi(int mode);
void engineSyncVBlank__Fv();
void netInit__Fv();
void netStartNetworkGameInit__Fv();

void startNetLaunch__Fv() {
    netAttemptLaunch = 1;
    netBroadCastStart = 0;
    engineSetDisplayDisable__Fi(10);
    engineSyncVBlank__Fv();
    netInit__Fv();
    netGameLaunchInfo[5] = 0;
    netStartNetworkGameInit__Fv();
}
}
