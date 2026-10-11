// Observed sceCdLayerSearchFile at 0x002e2998. PollSema on _sceCd_ncmd_semid
// must return that same word. The name is copied to _sf_data+36, at most 256
// bytes, and a full 256 forces a NUL at offset 291. The layer argument is stored
// at offset 296 and the buffer address at offset 292. RPC function 0 sends 300
// bytes from _sf_data and receives 4 at sfrdata. Success copies 36 bytes back
// through the uncached alias and returns the receive word.
extern "C" {
extern int _sceCd_ncmd_semid;
extern int ncmd_sema_keep_cmd;
extern int _sf_bind;
extern int SCE_CD_debug;
extern int my_thid;
extern unsigned char my_th_info[];
extern unsigned char cdrdsf[];
extern unsigned char _sf_data[];
extern unsigned char sfrdata[];
void cmd_sem_init();
int PollSema(int semaphore);
int ReferThreadStatus(int thread, void* info);
int norrath_archive_cd_sync(int mode);
void sceSifInitRpc();
int sceSifBindRpc(void* client, unsigned server, unsigned mode);
void sceSifWriteBackDCache(void* address, int bytes);
int sceSifCallRpc(void* client, unsigned function, unsigned mode, void* send, int send_size,
                  void* receive, int receive_size, void* end_function, void* end_parameter);
int norrath_sce_printf(const char* format, ...);
int norrath_player_signal_semaphore(int semaphore);
static unsigned char* uncached(unsigned char* pointer) {
#if defined(NORRATH_HOST_CONTRACT)
    return pointer;
#else
    const unsigned address = 0x20000000u | static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(address));
#endif
}
static void store_word(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}
static unsigned load_word(const unsigned char* slot) {
    return slot[0] | (slot[1] << 8) | (slot[2] << 16) | (slot[3] << 24);
}
static void spin_bind_delay() {
    volatile unsigned remaining = 0x00100000u;
    do {
        remaining = remaining - 1u;
    } while (remaining != 0xffffffffu);
}
static void bind_search_client() {
    if (_sf_bind >= 0) return;
    for (;;) {
        const int bound = sceSifBindRpc(cdrdsf, 0x80000597u, 0u);
        if (bound >= 0) {
            if (load_word(cdrdsf + 36) != 0u) {
                _sf_bind = 0;
                return;
            }
        } else if (SCE_CD_debug > 0) {
            norrath_sce_printf("Libcdvd bind err CdSearchFile\n");
        }
        spin_bind_delay();
    }
}
int norrath_archive_cd_layer_search(unsigned char* file, const char* name, int layer) {
    cmd_sem_init();
    if (PollSema(_sceCd_ncmd_semid) != _sceCd_ncmd_semid) return 0;
    ncmd_sema_keep_cmd = 1;
    (void)ReferThreadStatus(my_thid, my_th_info);
    if (norrath_archive_cd_sync(1) != 0) {
        norrath_player_signal_semaphore(_sceCd_ncmd_semid);
        return 0;
    }
    sceSifInitRpc();
    bind_search_client();
    unsigned index = 0;
    _sf_data[36] = static_cast<unsigned char>(name[0]);
    if (name[0] != 0) {
        index = 1;
        while (index < 256u) {
            const unsigned char byte = static_cast<unsigned char>(name[index]);
            _sf_data[36u + index] = byte;
            if (byte == 0u) break;
            index += 1u;
        }
    }
    if (index == 256u) _sf_data[291] = 0;
    if (SCE_CD_debug > 0) norrath_sce_printf("ee call cmd search %s\n", reinterpret_cast<char*>(_sf_data + 36));
    store_word(_sf_data + 296, static_cast<unsigned>(layer));
    store_word(_sf_data + 292, static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_sf_data)));
    sceSifWriteBackDCache(_sf_data, 300);
    const int rpc = sceSifCallRpc(cdrdsf, 0u, 0u, _sf_data, 300, sfrdata, 4, nullptr, nullptr);
    if (rpc < 0) {
        norrath_player_signal_semaphore(_sceCd_ncmd_semid);
        return 0;
    }
    const unsigned char* source = uncached(_sf_data);
    for (unsigned byte = 0; byte < 36u; ++byte) file[byte] = source[byte];
    if (SCE_CD_debug > 0) {
        norrath_sce_printf("search name %s\n", reinterpret_cast<char*>(file + 8));
        norrath_sce_printf("search size %d\n", static_cast<int>(load_word(file + 4)));
        norrath_sce_printf("search loc lbn %d\n", static_cast<int>(load_word(file)));
    }
    const unsigned result = load_word(uncached(sfrdata));
    norrath_player_signal_semaphore(_sceCd_ncmd_semid);
    return static_cast<int>(result);
}
}
