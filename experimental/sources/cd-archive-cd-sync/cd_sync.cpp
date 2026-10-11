// Observed sceCdSync at 0x002e2ed8.
// Mode 0 prints "N cmd wait\n" when SCE_CD_debug at 0x00446090 is positive,
// then waits until _sceCd_c_cb_sem at 0x004460b4 and sceSifCheckStatRpc on
// _sceCd_cd_ncmd at 0x00447250 are both zero. Each remaining wait is
// DelayThread(4000). Nonzero modes do not wait: they return 1 when either
// word is nonzero and 0 when both are clear. Blocking mode returns 0.
extern "C" {
extern int SCE_CD_debug;
extern int _sceCd_c_cb_sem;
extern unsigned char _sceCd_cd_ncmd[];
int norrath_sce_printf(const char* format, ...);
void DelayThread(int microseconds);
int sceSifCheckStatRpc(void* client);
int norrath_archive_cd_sync(int mode) {
    if (mode == 0) {
        if (SCE_CD_debug > 0) norrath_sce_printf("N cmd wait\n");
        for (;;) {
            if (_sceCd_c_cb_sem == 0 && sceSifCheckStatRpc(_sceCd_cd_ncmd) == 0) return 0;
            DelayThread(4000);
        }
    }
    if (_sceCd_c_cb_sem != 0) return 1;
    if (sceSifCheckStatRpc(_sceCd_cd_ncmd) != 0) return 1;
    return 0;
}
}
