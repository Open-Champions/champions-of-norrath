// Observed sceCdNcmdDiskReady at 0x002e2e40, 152 bytes.
// _sceCd_ncmd_prechk(2) must be nonzero. RPC function 14 on _sceCd_cd_ncmd
// receives 4 bytes at _sceCd_ncmdrdata. A negative RPC signals
// _sceCd_ncmd_semid and returns 0. Otherwise the result word is loaded through
// the uncached alias before that same signal. The original signal is SignalSema;
// this calls the published service of that syscall.
extern "C" {
extern int _sceCd_ncmd_semid;
extern unsigned char _sceCd_cd_ncmd[];
extern unsigned char _sceCd_ncmdrdata[];
int _sceCd_ncmd_prechk(int command);
int sceSifCallRpc(void* client, unsigned function, unsigned mode, void* send, int send_size,
                  void* receive, int receive_size, void* end_function, void* end_parameter);
int norrath_player_signal_semaphore(int semaphore);
int sceCdNcmdDiskReady() {
    if (_sceCd_ncmd_prechk(2) == 0) return 0;
    const int rpc = sceSifCallRpc(_sceCd_cd_ncmd, 14u, 0u, nullptr, 0, _sceCd_ncmdrdata, 4, nullptr, nullptr);
    if (rpc < 0) {
        norrath_player_signal_semaphore(_sceCd_ncmd_semid);
        return 0;
    }
#if defined(NORRATH_HOST_CONTRACT)
    const unsigned word = *reinterpret_cast<unsigned*>(_sceCd_ncmdrdata);
#else
    const unsigned address = 0x20000000u | static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_sceCd_ncmdrdata));
    const unsigned word = *reinterpret_cast<volatile unsigned*>(static_cast<__UINTPTR_TYPE__>(address));
#endif
    norrath_player_signal_semaphore(_sceCd_ncmd_semid);
    return static_cast<int>(word);
}
}
