/* Observed sceCdDiskReady_old at 0x002e34e8, 504 bytes: the earlier form of
 * the disk-ready call that sceCdDiskReady falls back to. The argument is a
 * mode word. A positive SCE_CD_debug (0x00446090) prints "OLD DiskReady
 * Call\n". cmd_sem_init() runs, then PollSema is called on _sceCd_scmd_semid
 * (0x004460ac); a result that differs from the id (read again after the call)
 * returns 6. sceCdSyncS(1) is then called: a nonzero result signals the
 * semaphore and returns -1 when the mode is 8, otherwise 6. Otherwise
 * sceSifInitRpc(0) runs. When _scmd_dr_bind (0x004460c8) is already
 * non-negative the bind step is skipped. When it is negative,
 * sceSifBindRpc(cddr, 0x8000059a, 0) is repeated: a negative result prints
 * "Libcdvd bind err CdDiskReady\n" under debug, spins and retries without
 * limit; a non-negative result with the client's word at +36 nonzero stores 0
 * in _scmd_dr_bind and goes on; a zero word spins and retries without limit.
 * The mode is stored in _dr_mode (0x008e83d0), 4 bytes of it are written back
 * from the data cache, and sceSifCallRpc runs on cddr (0x008e8390) with
 * function 0, mode 0, _dr_mode as the 4-byte send area, _sceCd_scmdrdata
 * (0x00447280) as the 4-byte receive area, no end function and no parameter.
 * A failed call signals the semaphore and returns -1 for mode 8, otherwise 6.
 * On success "DiskReady ended\n" is printed under debug, the first receive
 * word is read through the uncached alias (address OR 0x20000000) before the
 * semaphore is signalled, and that word is returned.
 * Inferred: each spin is a timing delay of 0x100001 counts; its instruction
 * count is not reproduced. Unknown: what server 0x8000059a does with the mode.
 */
extern "C" {
extern int SCE_CD_debug;
extern int _sceCd_scmd_semid;
extern int _scmd_dr_bind;
extern int cddr[];
extern int _dr_mode;
extern unsigned char _sceCd_scmdrdata[];

void cmd_sem_init();
int PollSema(int semaphore);
int SignalSema(int semaphore);
int sceCdSyncS(int mode);
int scePrintf(const char* format, ...);
void sceSifInitRpc(int mode);
int sceSifBindRpc(void* client, unsigned server, unsigned mode);
void sceSifWriteBackDCache(void* address, int size);
int sceSifCallRpc(void* client, unsigned function, unsigned mode, void* send, int send_size,
                  void* receive, int receive_size, void* end_function, void* end_parameter);

static void spin() {
    for (int count = 0x100000;;) {
        --count;
#if defined(__mips__)
        asm volatile("nop\n\tnop\n\tnop\n\tnop");
#endif
        if (count == -1) break;
    }
}

static unsigned uncached_word(const unsigned char* address) {
#if defined(NORRATH_HOST_CONTRACT)
    return *reinterpret_cast<const unsigned*>(address);
#else
    const __UINTPTR_TYPE__ alias = 0x20000000u | reinterpret_cast<__UINTPTR_TYPE__>(address);
    return *reinterpret_cast<const volatile unsigned*>(alias);
#endif
}

int sceCdDiskReady_old(int mode) {
    if (SCE_CD_debug > 0) scePrintf("OLD DiskReady Call\n");
    cmd_sem_init();
    const int polled = PollSema(_sceCd_scmd_semid);
    if (_sceCd_scmd_semid != polled) return 6;
    const int busy = (mode == 8) ? -1 : 6;
    if (sceCdSyncS(1) != 0) {
        SignalSema(_sceCd_scmd_semid);
        return busy;
    }
    sceSifInitRpc(0);
    if (_scmd_dr_bind < 0) {
        for (;;) {
            if (sceSifBindRpc(cddr, 0x8000059au, 0u) < 0) {
                if (SCE_CD_debug > 0) scePrintf("Libcdvd bind err CdDiskReady\n");
                spin();
                continue;
            }
            if (cddr[9] != 0) {
                _scmd_dr_bind = 0;
                break;
            }
            spin();
        }
    }
    _dr_mode = mode;
    sceSifWriteBackDCache(&_dr_mode, 4);
    if (sceSifCallRpc(cddr, 0u, 0u, &_dr_mode, 4, _sceCd_scmdrdata, 4, nullptr, nullptr) < 0) {
        SignalSema(_sceCd_scmd_semid);
        return busy;
    }
    if (SCE_CD_debug > 0) scePrintf("DiskReady ended\n");
    const int result = static_cast<int>(uncached_word(_sceCd_scmdrdata));
    SignalSema(_sceCd_scmd_semid);
    return result;
}
}
