// Observed sceCdReadDvdDualInfo at 0x002e3d90, 208 bytes.
// _sceCd_scmd_prechk(39) must be nonzero or the result is 0. RPC function 39 on
// _sceCd_cd_scmd sends nothing and receives 12 bytes at _sceCd_scmdrdata. A
// negative RPC result signals _sceCd_scmd_semid and returns 0. Otherwise the
// words at offsets 4 and 8 of the reply, read through the uncached alias, are
// stored to the first and second pointer arguments. The word at offset 0 is
// read through the alias as well, before the semaphore is signaled, and is the
// return value. What the reply words mean is unknown.
extern "C" {
extern int _sceCd_scmd_semid;
extern unsigned char _sceCd_scmdrdata[];
extern unsigned char _sceCd_cd_scmd[];
int _sceCd_scmd_prechk(int command);
int sceSifCallRpc(void* client, unsigned function, unsigned mode, void* send, int send_size,
                  void* receive, int receive_size, void* end_function, void* end_parameter);
int norrath_player_signal_semaphore(int semaphore);
static unsigned reply_word(unsigned offset) {
#if defined(NORRATH_HOST_CONTRACT)
    return *reinterpret_cast<unsigned*>(_sceCd_scmdrdata + offset);
#else
    const unsigned address = 0x20000000u | static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_sceCd_scmdrdata + offset));
    return *reinterpret_cast<volatile unsigned*>(static_cast<__UINTPTR_TYPE__>(address));
#endif
}
unsigned sceCdReadDvdDualInfo(unsigned* dual, unsigned* layer_start) {
    if (_sceCd_scmd_prechk(39) == 0) return 0u;
    if (sceSifCallRpc(_sceCd_cd_scmd, 39u, 0u, nullptr, 0, _sceCd_scmdrdata, 12, nullptr, nullptr) < 0) {
        norrath_player_signal_semaphore(_sceCd_scmd_semid);
        return 0u;
    }
    *dual = reply_word(4);
    *layer_start = reply_word(8);
    const unsigned result = reply_word(0);
    norrath_player_signal_semaphore(_sceCd_scmd_semid);
    return result;
}
}
