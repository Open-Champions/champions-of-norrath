// Observed sceCdInit at 0x002e3208, 736 bytes. sceCdSyncS(1) must return 0.
// It then calls sceSifInitRpc(0) and GetThreadId, adds one to the command
// count, and stores the thread id. The count is the four bytes at c_cnt.66.
// The selected object for that address is norrath_archive_cd_command_count_view.
// This unit does not define a second one. Init_seq becomes 1. The bind words
// and _it_bind become -1, and _sceCd_ee_read_mode becomes 0. It binds cdit at
// 0x008e8368, 40 bytes to the next symbol, to server 0x80000592. A negative
// bind prints Libcdvd bind err when SCE_CD_debug is positive, counts down
// from 0x00100000 to -1, and tries again. A zero word at cdit+36 does the
// same countdown without the print. A bound server stores the mode in _i_mode,
// clears _it_bind, and writes those four bytes back. The RPC sends those four
// bytes and receives 16 at _sceCd_scmdrdata. A negative RPC clears Init_seq
// and returns 0. Otherwise the uncached words at offsets 12, 4, and 8 choose
// the result. Word 12 equal to 255 or 254 yields 1, and 254 also stores 1 in
// SCE_CD_debug. Any other word 12 uses this probe on word 4, then word 8:
// the word itself when the word is at least 0, otherwise the word plus 255,
// then an arithmetic shift of 8. A probe below 2 yields 2. Otherwise the
// result is 1. Init_seq is cleared. Mode 5 prints Libcdvd Exit when
// SCE_CD_debug is positive, calls cdvd_exit, stores -1 in the two command
// semaphores and cb_semid, and returns the result. Every other mode calls
// cmd_sem_init and PowerOffCB and returns the result. The _sceCd_ objects,
// SCE_CD_debug, cb_semid, _ncmd_bind, _sf_bind, _scmd_bind, and my_thid are
// defined by sony-cd-words and stay imports here. Image values: Init_seq 0;
// _icmd_bind, _scmd_dr_bind, and _it_bind -1; cdit and _i_mode zero. The
// countdown is volatile so -O2 keeps it.
extern "C" {
int Init_seq = 0;
int _icmd_bind = -1;
int _scmd_dr_bind = -1;
int _it_bind = -1;
int _i_mode = 0;
alignas(4) unsigned char cdit[40] = {};
}
extern "C" int SCE_CD_debug;
extern "C" int cb_semid;
extern "C" int _ncmd_bind;
extern "C" int _sf_bind;
extern "C" int _scmd_bind;
extern "C" int my_thid;
extern "C" unsigned char norrath_archive_cd_command_count_view[4];
extern "C" unsigned _sceCd_ee_read_mode;
extern "C" int _sceCd_ncmd_semid;
extern "C" int _sceCd_scmd_semid;
extern "C" unsigned char _sceCd_scmdrdata[];
extern "C" int sceCdSyncS(int mode);
extern "C" void sceSifInitRpc(int mode);
extern "C" int GetThreadId();
extern "C" int scePrintf(const char* text, ...);
extern "C" int sceSifBindRpc(void* client, unsigned server, int mode);
extern "C" void sceSifWriteBackDCache(void* address, int bytes);
extern "C" int sceSifCallRpc(void* client, int operation, int mode, void* send, int send_size,
                             void* receive, int receive_size, void* completion,
                             void* completion_argument);
extern "C" void cdvd_exit();
extern "C" void cmd_sem_init();
extern "C" void PowerOffCB();

static unsigned load_word(const unsigned char* slot) {
    return slot[0] | (slot[1] << 8) | (slot[2] << 16) | (slot[3] << 24);
}

static void store_word(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}

static int uncached_word(unsigned char* object, int offset) {
    unsigned char* slot = object + offset;
#if defined(NORRATH_HOST_CONTRACT)
    const unsigned char* address = slot;
#else
    const unsigned bits = 0x20000000u | static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(slot));
    const unsigned char* address = reinterpret_cast<const unsigned char*>(static_cast<__UINTPTR_TYPE__>(bits));
#endif
    return static_cast<int>(*reinterpret_cast<const volatile unsigned*>(address));
}

static void wait_for_server() {
    volatile int remaining = 0x00100000;
#if defined(NORRATH_HOST_CONTRACT)
    extern int norrath_cd_init_wait_steps;
#endif
    do {
        remaining = remaining - 1;
#if defined(NORRATH_HOST_CONTRACT)
        norrath_cd_init_wait_steps += 1;
#endif
    } while (remaining != -1);
}

static int shifted_probe(int word) {
    int probe = word + 255;
    if (-1 < word)
        probe = word;
    return probe >> 8;
}

extern "C" int sceCdInit(int mode) {
    if (sceCdSyncS(1) != 0)
        return 0;
    sceSifInitRpc(0);
    const int thread = GetThreadId();
    unsigned count = load_word(norrath_archive_cd_command_count_view) + 1u;
    Init_seq = 1;
    my_thid = thread;
    _icmd_bind = -1;
    _sf_bind = -1;
    _ncmd_bind = -1;
    _scmd_bind = -1;
    _scmd_dr_bind = -1;
    _sceCd_ee_read_mode = 0;
    store_word(norrath_archive_cd_command_count_view, count);
    _it_bind = -1;
    for (;;) {
        const int bound = sceSifBindRpc(cdit, 0x80000592u, 0);
        if (bound < 0) {
            if (SCE_CD_debug > 0)
                scePrintf("Libcdvd bind err %d CD_Init %d\n", bound, static_cast<int>(count));
            wait_for_server();
            continue;
        }
        if (load_word(cdit + 36) == 0) {
            wait_for_server();
            continue;
        }
        break;
    }
    _i_mode = mode;
    _it_bind = 0;
    sceSifWriteBackDCache(&_i_mode, 4);
    const int rpc = sceSifCallRpc(cdit, 0, 0, &_i_mode, 4, _sceCd_scmdrdata, 16, nullptr, nullptr);
    if (rpc < 0) {
        Init_seq = 0;
        return 0;
    }
    const int word12 = uncached_word(_sceCd_scmdrdata, 12);
    int result = 1;
    if (word12 == 254)
        SCE_CD_debug = 1;
    else if (word12 != 255) {
        const int word4 = uncached_word(_sceCd_scmdrdata, 4);
        const int word8 = uncached_word(_sceCd_scmdrdata, 8);
        if (shifted_probe(word4) < 2 || shifted_probe(word8) < 2)
            result = 2;
    }
    Init_seq = 0;
    if (mode != 5) {
        cmd_sem_init();
        PowerOffCB();
        return result;
    }
    if (SCE_CD_debug > 0)
        scePrintf("Libcdvd Exit\n");
    cdvd_exit();
    _sceCd_ncmd_semid = -1;
    _sceCd_scmd_semid = -1;
    cb_semid = -1;
    return result;
}
