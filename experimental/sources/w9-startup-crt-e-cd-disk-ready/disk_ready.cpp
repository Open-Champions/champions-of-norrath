// Observed sceCdDiskReady at 0x002e36e0, 516 bytes. The argument is a mode word.
// A positive SCE_CD_debug (0x00446090) prints "NEW DiskReady Call\n". cmd_sem_init() runs,
// then PollSema is called on _sceCd_rcmd_semid (0x004460b0); the id is read again after the
// call and compared with the result. A result that differs from the id returns a busy
// value: -1 when the mode equals 8, otherwise 6. That same value is returned when the RPC
// itself fails, after the semaphore is signalled.
// Otherwise sceSifInitRpc(0) runs. When _scmd_dr_bind (0x004460c8) is already
// non-negative the bind step is skipped. When it is negative, sceSifBindRpc(cddr,
// 0x8000059c, 0) is repeated: a negative bind result prints "Libcdvd bind err
// CdDiskReady\n" under debug, spins for 0x100001 counts and retries without limit; a
// non-negative result with the client's word at +36 nonzero stores 0 in _scmd_dr_bind and
// goes on; a zero word waits the same spin and retries until the attempt counter, which
// starts at 0 and is raised after each such spin, reaches 17. Once it has, the semaphore is
// signalled and the result is sceCdDiskReady_old(mode).
// The mode is stored in _dr_mode (0x008e83d0), 4 bytes of it are written back from the
// data cache, and sceSifCallRpc runs on cddr with function 0, mode 0, _dr_mode as the
// 4-byte send area, _sceCd_rcmdrdata (0x00447b00) as the 4-byte receive area, no end
// function and no parameter. A failed call signals the semaphore and returns the busy value.
// On success "DiskReady ended\n" is printed under debug, the first receive word is read
// through the uncached alias (address | 0x20000000) before the semaphore is signalled, and
// that word is returned.
// Inferred: the spin is a timing delay; its exact instruction count is not reproduced.
// Unknown: what server 0x8000059c does with the mode.
extern "C" {
extern int SCE_CD_debug;
extern int _sceCd_rcmd_semid;
extern int _scmd_dr_bind;
extern int cddr[];
extern int _dr_mode;
extern unsigned char _sceCd_rcmdrdata[];

void cmd_sem_init();
int PollSema(int semaphore);
int SignalSema(int semaphore);
int scePrintf(const char* format, ...);
int sceSifInitRpc(int mode);
int sceSifBindRpc(void* client, unsigned server, unsigned mode);
void sceSifWriteBackDCache(void* address, int size);
int sceSifCallRpc(void* client, unsigned function, unsigned mode, void* send, int send_size,
                  void* receive, int receive_size, void* end_function, void* end_parameter);
int sceCdDiskReady_old(int mode);

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

int sceCdDiskReady(int mode) {
    if (SCE_CD_debug > 0) scePrintf("NEW DiskReady Call\n");
    cmd_sem_init();
    const int polled = PollSema(_sceCd_rcmd_semid);
    const int busy = (mode == 8) ? -1 : 6;
    if (_sceCd_rcmd_semid != polled) return busy;
    sceSifInitRpc(0);
    if (_scmd_dr_bind < 0) {
        int attempts = 0;
        for (;;) {
            if (sceSifBindRpc(cddr, 0x8000059cu, 0u) < 0) {
                if (SCE_CD_debug > 0) scePrintf("Libcdvd bind err CdDiskReady\n");
                spin();
                continue;
            }
            if (cddr[9] != 0) {
                _scmd_dr_bind = 0;
                break;
            }
            if (attempts >= 17) {
                SignalSema(_sceCd_rcmd_semid);
                return sceCdDiskReady_old(mode);
            }
            spin();
            ++attempts;
        }
    }
    _dr_mode = mode;
    sceSifWriteBackDCache(&_dr_mode, 4);
    if (sceSifCallRpc(cddr, 0u, 0u, &_dr_mode, 4, _sceCd_rcmdrdata, 4, nullptr, nullptr) < 0) {
        SignalSema(_sceCd_rcmd_semid);
        return busy;
    }
    if (SCE_CD_debug > 0) scePrintf("DiskReady ended\n");
    const int result = static_cast<int>(uncached_word(_sceCd_rcmdrdata));
    SignalSema(_sceCd_rcmd_semid);
    return result;
}
}
