// Observed _sceCd_scmd_prechk at 0x002e2fe8, 368 bytes.
// Same semaphore and bind shape as the N-command check, using
// _sceCd_scmd_semid, scmd_sema_keep_cmd, sceCdSyncS, client _sceCd_cd_scmd,
// and server 0x80000593. The miss and bind strings are the bytes at
// 0x0047bb78 and 0x0047bba0.
extern "C" {
extern int _sceCd_scmd_semid;
extern int scmd_sema_keep_cmd;
extern int SCE_CD_debug;
extern int _scmd_bind;
extern int my_thid;
extern unsigned char my_th_info[];
extern unsigned char _sceCd_cd_scmd[];
void cmd_sem_init();
int PollSema(int semaphore);
int ReferThreadStatus(int thread, void* info);
int sceCdSyncS(int mode);
int norrath_player_signal_semaphore(int semaphore);
void sceSifInitRpc();
int sceSifBindRpc(void* client, unsigned server, unsigned mode);
int scePrintf(const char* format, ...);
static unsigned load_word(const unsigned char* slot) {
    return static_cast<unsigned>(slot[0]) | (static_cast<unsigned>(slot[1]) << 8) |
           (static_cast<unsigned>(slot[2]) << 16) | (static_cast<unsigned>(slot[3]) << 24);
}
static void bind_delay() {
    volatile unsigned remaining = 0x00100000u;
    do {
        remaining = remaining - 1u;
    } while (remaining != 0xffffffffu);
}
int _sceCd_scmd_prechk(int command) {
    cmd_sem_init();
    if (PollSema(_sceCd_scmd_semid) != _sceCd_scmd_semid) {
        if (SCE_CD_debug > 0) scePrintf("Scmd fail sema cur_cmd:%d keep_cmd:%d\n", command, scmd_sema_keep_cmd);
        return 0;
    }
    scmd_sema_keep_cmd = command;
    (void)ReferThreadStatus(my_thid, my_th_info);
    if (sceCdSyncS(1) != 0) {
        norrath_player_signal_semaphore(_sceCd_scmd_semid);
        return 0;
    }
    sceSifInitRpc();
    if (_scmd_bind >= 0) return 1;
    for (;;) {
        const int bound = sceSifBindRpc(_sceCd_cd_scmd, 0x80000593u, 0u);
        if (bound >= 0 && load_word(_sceCd_cd_scmd + 36) != 0u) {
            _scmd_bind = 0;
            return 1;
        }
        if (bound < 0 && SCE_CD_debug > 0) scePrintf("Libcdvd bind err S cmd\n");
        bind_delay();
    }
}
}
