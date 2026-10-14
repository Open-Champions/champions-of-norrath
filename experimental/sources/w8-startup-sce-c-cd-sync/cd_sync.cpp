// Observed sceCdSync at 0x002e2ed8, 160 bytes.
// Mode 0 prints "N cmd wait\n" when SCE_CD_debug (0x00446090) is positive, then
// waits until _sceCd_c_cb_sem (0x004460b4) is zero and sceSifCheckStatRpc on
// _sceCd_cd_ncmd (0x00447250) returns zero, calling DelayThread(4000) for each
// failed check. Blocking mode returns 0. A nonzero mode never waits: it returns
// 1 when the callback word is nonzero or the RPC check is nonzero, otherwise 0.
extern "C" {
extern int SCE_CD_debug;
extern int _sceCd_c_cb_sem;
extern unsigned char _sceCd_cd_ncmd[];
int scePrintf(const char* format, ...);
void DelayThread(int microseconds);
int sceSifCheckStatRpc(void* client);
int sceCdSync(int mode) {
    if (mode != 0) {
        if (_sceCd_c_cb_sem != 0) return 1;
        return sceSifCheckStatRpc(_sceCd_cd_ncmd) != 0 ? 1 : 0;
    }
    if (SCE_CD_debug > 0) scePrintf("N cmd wait\n");
    for (;;) {
        if (_sceCd_c_cb_sem == 0 && sceSifCheckStatRpc(_sceCd_cd_ncmd) == 0) return 0;
        DelayThread(4000);
    }
}
}
