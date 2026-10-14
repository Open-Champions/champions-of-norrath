// Observed sceCdGetError at 0x002e3c20, 152 bytes. _sceCd_scmd_prechk(3) must
// be nonzero, else -1. RPC function 4 on _sceCd_cd_scmd (0x00447c00) has no
// send buffer and a 4-byte receive at _sceCd_scmdrdata (0x00447280). A negative
// RPC result signals _sceCd_scmd_semid (0x004460ac) and returns -1. Otherwise
// the result word is loaded through the 0x20000000 uncached alias, then the
// same semaphore is signalled and the word is returned.
extern "C" {
extern int _sceCd_scmd_semid;
extern unsigned char _sceCd_cd_scmd[];
extern unsigned char _sceCd_scmdrdata[];
int _sceCd_scmd_prechk(int command);
int sceSifCallRpc(void* client, unsigned function, unsigned mode, void* send, int send_size,
                  void* receive, int receive_size, void* end_function, void* end_parameter);
int SignalSema(int semaphore);
int sceCdGetError() {
    if (_sceCd_scmd_prechk(3) == 0) return -1;
    if (sceSifCallRpc(_sceCd_cd_scmd, 4u, 0u, nullptr, 0, _sceCd_scmdrdata, 4, nullptr, nullptr) < 0) {
        SignalSema(_sceCd_scmd_semid);
        return -1;
    }
#if defined(NORRATH_HOST_CONTRACT)
    const unsigned* word = reinterpret_cast<const unsigned*>(_sceCd_scmdrdata);
#else
    const unsigned* word = reinterpret_cast<const unsigned*>(
        0x20000000u | static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_sceCd_scmdrdata)));
#endif
    const int value = static_cast<int>(*word);
    SignalSema(_sceCd_scmd_semid);
    return value;
}
}
