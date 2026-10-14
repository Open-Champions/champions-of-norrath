// Observed sceCdSyncS at 0x002e2f78, 108 bytes.
// Mode 0 prints "S cmd wait\n" when SCE_CD_debug is positive, then waits until
// sceSifCheckStatRpc on _sceCd_cd_scmd returns 0. Each remaining wait is
// DelayThread(4000). A nonzero mode returns that check directly and does not wait.
extern "C" {
extern int SCE_CD_debug;
extern unsigned char _sceCd_cd_scmd[];
int scePrintf(const char* format, ...);
void DelayThread(int microseconds);
int sceSifCheckStatRpc(void* client);
int sceCdSyncS(int mode) {
    if (mode != 0) return sceSifCheckStatRpc(_sceCd_cd_scmd);
    if (SCE_CD_debug > 0) scePrintf("S cmd wait\n");
    while (sceSifCheckStatRpc(_sceCd_cd_scmd) != 0) DelayThread(4000);
    return 0;
}
}
