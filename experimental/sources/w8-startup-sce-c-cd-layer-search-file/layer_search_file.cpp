// Observed sceCdLayerSearchFile at 0x002e2998, 788 bytes.
// cmd_sem_init runs first. PollSema on _sceCd_ncmd_semid must return that same
// id or the result is 0. ncmd_sema_keep_cmd is set to 1, this thread's status is
// requested, and sceCdSync(1) must return 0; otherwise the semaphore is
// signaled and the result is 0. sceSifInitRpc runs. If _sf_bind is negative the
// client cdrdsf is bound to server 0x80000597 until the bind succeeds and word
// 36 of the client is nonzero; a failed bind prints "Libcdvd bind err
// CdSearchFile\n" under SCE_CD_debug, and a delay of 0x100001 counts follows each
// attempt that does not finish the loop. _sf_bind is then set to 0.
// The name is copied to _sf_data + 36 for at most 256 bytes including the
// terminator; if no terminator fit, byte 291 of _sf_data is zeroed. Under debug
// "ee call cmd search %s\n" prints the copy. The layer goes to _sf_data + 296,
// the address of _sf_data to _sf_data + 292, and 300 bytes are written back
// from the data cache. RPC function 0 on cdrdsf sends those 300 bytes and
// receives 4 bytes at sfrdata. A negative result signals the semaphore and
// returns 0. Otherwise the first 36 bytes of _sf_data, read through the
// uncached alias, are copied to the file argument. Under debug the name at +8
// ("search name %s\n"), the word at +4 ("search size %d\n") and the word at +0
// ("search loc lbn %d\n") print, each guarded by its own debug test. The word
// at sfrdata, read through the alias before the semaphore is signaled, is
// returned. The meaning of the 36 reply bytes beyond the printed fields is
// unknown.
extern "C" {
extern int SCE_CD_debug;
extern int _sceCd_ncmd_semid;
extern int ncmd_sema_keep_cmd;
extern int _sf_bind;
extern int my_thid;
extern unsigned char my_th_info[];
extern unsigned char cdrdsf[];
extern unsigned char _sf_data[];
extern unsigned char sfrdata[];
void cmd_sem_init();
int PollSema(int semaphore);
int ReferThreadStatus(int thread, void* info);
int sceCdSync(int mode);
int norrath_player_signal_semaphore(int semaphore);
void sceSifInitRpc();
int sceSifBindRpc(void* client, unsigned server, unsigned mode);
int sceSifCallRpc(void* client, unsigned function, unsigned mode, void* send, int send_size,
                  void* receive, int receive_size, void* end_function, void* end_parameter);
void sceSifWriteBackDCache(void* address, int bytes);
int scePrintf(const char* format, ...);
static unsigned load_word(const unsigned char* slot) {
    return static_cast<unsigned>(slot[0]) | (static_cast<unsigned>(slot[1]) << 8) |
           (static_cast<unsigned>(slot[2]) << 16) | (static_cast<unsigned>(slot[3]) << 24);
}
static unsigned pointer_bits(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}
static void store_word(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}
static void bind_delay() {
    volatile unsigned remaining = 0x00100000u;
    do {
        remaining = remaining - 1u;
    } while (remaining != 0xffffffffu);
}
static const volatile unsigned char* uncached(const unsigned char* address) {
#if defined(NORRATH_HOST_CONTRACT)
    return address;
#else
    return reinterpret_cast<const volatile unsigned char*>(
        static_cast<__UINTPTR_TYPE__>(0x20000000u | pointer_bits(address)));
#endif
}
unsigned sceCdLayerSearchFile(unsigned char* file, const unsigned char* name, int layer) {
    cmd_sem_init();
    if (PollSema(_sceCd_ncmd_semid) != _sceCd_ncmd_semid) return 0u;
    ncmd_sema_keep_cmd = 1;
    (void)ReferThreadStatus(my_thid, my_th_info);
    if (sceCdSync(1) != 0) {
        norrath_player_signal_semaphore(_sceCd_ncmd_semid);
        return 0u;
    }
    sceSifInitRpc();
    if (_sf_bind < 0) {
        for (;;) {
            const int bound = sceSifBindRpc(cdrdsf, 0x80000597u, 0u);
            if (bound >= 0) {
                if (load_word(cdrdsf + 36) != 0u) break;
            } else if (SCE_CD_debug > 0) {
                scePrintf("Libcdvd bind err CdSearchFile\n");
            }
            bind_delay();
        }
        _sf_bind = 0;
    }
    unsigned char* const destination = _sf_data + 36;
    int length = 0;
    destination[0] = name[0];
    if (name[0] != 0) {
        for (;;) {
            ++length;
            if (length >= 256) break;
            destination[length] = name[length];
            if (name[length] == 0) break;
        }
    }
    if (length == 256) _sf_data[291] = 0;
    if (SCE_CD_debug > 0) scePrintf("ee call cmd search %s\n", destination);
    store_word(_sf_data + 296, static_cast<unsigned>(layer));
    store_word(_sf_data + 292, pointer_bits(_sf_data));
    sceSifWriteBackDCache(_sf_data, 300);
    if (sceSifCallRpc(cdrdsf, 0u, 0u, _sf_data, 300, sfrdata, 4, nullptr, nullptr) < 0) {
        norrath_player_signal_semaphore(_sceCd_ncmd_semid);
        return 0u;
    }
    const volatile unsigned char* reply = uncached(_sf_data);
    for (int index = 0; index < 36; ++index) file[index] = reply[index];
    if (SCE_CD_debug > 0) scePrintf("search name %s\n", file + 8);
    if (SCE_CD_debug > 0) scePrintf("search size %d\n", load_word(file + 4));
    if (SCE_CD_debug > 0) scePrintf("search loc lbn %d\n", load_word(file));
    const volatile unsigned char* result_bytes = uncached(sfrdata);
    const unsigned result = static_cast<unsigned>(result_bytes[0]) | (static_cast<unsigned>(result_bytes[1]) << 8) |
                            (static_cast<unsigned>(result_bytes[2]) << 16) | (static_cast<unsigned>(result_bytes[3]) << 24);
    norrath_player_signal_semaphore(_sceCd_ncmd_semid);
    return result;
}
}
