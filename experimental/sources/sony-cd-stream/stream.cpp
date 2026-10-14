// Observed sceCdStream at 0x002e4458, 360 bytes.
// _sceCd_ncmd_prechk(15) must be nonzero. The 20-byte command at
// _sceCd_ncmdsdata stores the first argument, the count, the buffer, and the
// command. A non-null mode pointer copies its first three bytes. RPC function
// 9 sends those 20 bytes and receives 4 at _sceCd_ncmdrdata. Failure signals
// _sceCd_ncmd_semid and returns 0. Success loads the result through the
// uncached alias before the same signal.
extern "C" {
extern int SCE_CD_debug;
extern int _sceCd_ncmd_semid;
extern unsigned char _sceCd_ncmdsdata[];
extern unsigned char _sceCd_ncmdrdata[];
extern unsigned char _sceCd_cd_ncmd[];
int _sceCd_ncmd_prechk(int command);
void sceSifWriteBackDCache(void* address, int bytes);
int sceSifCallRpc(void* client, unsigned function, unsigned mode, void* send, int send_size,
                  void* receive, int receive_size, void* end_function, void* end_parameter);
int norrath_player_signal_semaphore(int semaphore);
int scePrintf(const char* format, ...);
static void store_word(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}
static unsigned pointer_bits(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}
unsigned sceCdStream(unsigned argument, unsigned count, void* buffer, unsigned command, const unsigned char* mode) {
    if (_sceCd_ncmd_prechk(15) == 0) return 0u;
    if (SCE_CD_debug > 0) scePrintf("call cdreadstm call\n");
    store_word(_sceCd_ncmdsdata, argument);
    store_word(_sceCd_ncmdsdata + 4, count);
    store_word(_sceCd_ncmdsdata + 8, pointer_bits(buffer));
    store_word(_sceCd_ncmdsdata + 12, command);
    if (mode != nullptr) {
        _sceCd_ncmdsdata[16] = mode[0];
        _sceCd_ncmdsdata[17] = mode[1];
        _sceCd_ncmdsdata[18] = mode[2];
    }
    if (SCE_CD_debug > 0) scePrintf("call cdreadstm cmd\n");
    sceSifWriteBackDCache(_sceCd_ncmdsdata, 20);
    const int rpc = sceSifCallRpc(_sceCd_cd_ncmd, 9u, 0u, _sceCd_ncmdsdata, 20, _sceCd_ncmdrdata, 4, nullptr, nullptr);
    if (rpc < 0) {
        norrath_player_signal_semaphore(_sceCd_ncmd_semid);
        return 0u;
    }
    if (SCE_CD_debug > 0) scePrintf("cdread end\n");
#if defined(NORRATH_HOST_CONTRACT)
    const unsigned word = *reinterpret_cast<unsigned*>(_sceCd_ncmdrdata);
#else
    const unsigned address = 0x20000000u | static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_sceCd_ncmdrdata));
    const unsigned word = *reinterpret_cast<volatile unsigned*>(static_cast<__UINTPTR_TYPE__>(address));
#endif
    norrath_player_signal_semaphore(_sceCd_ncmd_semid);
    return word;
}
}
