// Observed sceCdGetError at 0x002e3c20. _sceCd_scmd_prechk(3) must be nonzero.
// The RPC is function 4 on _sceCd_cd_scmd at 0x00447c00, with no send buffer and
// a 4-byte receive at _sceCd_scmdrdata at 0x00447280. A negative RPC result
// signals _sceCd_scmd_semid at 0x004460ac and returns -1. Otherwise the result
// word is loaded through the uncached alias 0x20000000 before the same signal.
extern "C" {
int _sceCd_scmd_prechk(int command);
int sceSifCallRpc(void* client, unsigned function, unsigned mode, void* send, int send_size,
                  void* receive, int receive_size, void* end_function, void* end_parameter);
int norrath_player_signal_semaphore(int semaphore);
extern int _sceCd_scmd_semid;
extern unsigned char _sceCd_cd_scmd[];
extern unsigned char _sceCd_scmdrdata[];
int norrath_archive_cd_error() {
    if (_sceCd_scmd_prechk(3) == 0) return -1;
    const int rpc = sceSifCallRpc(_sceCd_cd_scmd, 4u, 0u, nullptr, 0,
                                  _sceCd_scmdrdata, 4, nullptr, nullptr);
    if (rpc < 0) {
        norrath_player_signal_semaphore(_sceCd_scmd_semid);
        return -1;
    }
#if defined(NORRATH_HOST_CONTRACT)
    const unsigned word = *reinterpret_cast<volatile unsigned*>(_sceCd_scmdrdata);
#else
    const unsigned address = 0x20000000u | static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_sceCd_scmdrdata));
    const unsigned word = *reinterpret_cast<volatile unsigned*>(static_cast<__UINTPTR_TYPE__>(address));
#endif
    norrath_player_signal_semaphore(_sceCd_scmd_semid);
    return static_cast<int>(word);
}
}
