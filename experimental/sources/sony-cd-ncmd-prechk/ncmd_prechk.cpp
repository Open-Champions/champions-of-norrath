// Observed _sceCd_ncmd_prechk at 0x002e2cd0, 368 bytes.
// PollSema on _sceCd_ncmd_semid must return that same id. A miss prints the
// format at 0x0047bb18 when SCE_CD_debug is positive and returns 0. A hit
// stores the command in ncmd_sema_keep_cmd, asks for this thread's status, and
// calls the published sceCdSync reconstruction with mode 1. A nonzero sync
// signals the semaphore and returns 0. Otherwise sceSifInitRpc runs. A
// nonnegative _ncmd_bind returns 1. A negative bind retries sceSifBindRpc of
// _sceCd_cd_ncmd to server 0x80000595 until the result is nonnegative and word
// 36 of that client is nonzero, then stores 0 to _ncmd_bind and returns 1.
extern "C" {
extern int _sceCd_ncmd_semid;
extern int ncmd_sema_keep_cmd;
extern int SCE_CD_debug;
extern int _ncmd_bind;
extern int my_thid;
extern unsigned char my_th_info[];
extern unsigned char _sceCd_cd_ncmd[];
void cmd_sem_init();
int PollSema(int semaphore);
int ReferThreadStatus(int thread, void* info);
int norrath_archive_cd_sync(int mode);
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
int _sceCd_ncmd_prechk(int command) {
    cmd_sem_init();
    if (PollSema(_sceCd_ncmd_semid) != _sceCd_ncmd_semid) {
        if (SCE_CD_debug > 0) scePrintf("Ncmd fail sema cur_cmd:%d keep_cmd:%d\n", command, ncmd_sema_keep_cmd);
        return 0;
    }
    ncmd_sema_keep_cmd = command;
    (void)ReferThreadStatus(my_thid, my_th_info);
    if (norrath_archive_cd_sync(1) != 0) {
        norrath_player_signal_semaphore(_sceCd_ncmd_semid);
        return 0;
    }
    sceSifInitRpc();
    if (_ncmd_bind >= 0) return 1;
    for (;;) {
        const int bound = sceSifBindRpc(_sceCd_cd_ncmd, 0x80000595u, 0u);
        if (bound >= 0 && load_word(_sceCd_cd_ncmd + 36) != 0u) {
            _ncmd_bind = 0;
            return 1;
        }
        if (bound < 0 && SCE_CD_debug > 0) scePrintf("Libcdvd bind err N CMD\n");
        bind_delay();
    }
}
}
