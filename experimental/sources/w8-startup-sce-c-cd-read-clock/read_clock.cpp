// Observed sceCdReadClock at 0x002e3fe8, 248 bytes.
// _sceCd_scmd_prechk(15) must be nonzero or the result is 0. When SCE_CD_debug
// is positive "Libcdvd call Clock read 1\n" prints, then RPC function 1 on
// _sceCd_cd_scmd sends nothing and receives 16 bytes at _sceCd_scmdrdata. A
// negative RPC result signals _sceCd_scmd_semid and returns 0. Otherwise the
// eight bytes at offset 4 of the reply, read through the uncached alias, are
// copied to the argument, "Libcdvd call Clock read 2\n" prints under the same
// debug test, and the word at offset 0 is read through the alias before the
// semaphore is signaled and returned. What the reply bytes mean is unknown.
extern "C" {
extern int SCE_CD_debug;
extern int _sceCd_scmd_semid;
extern unsigned char _sceCd_scmdrdata[];
extern unsigned char _sceCd_cd_scmd[];
int _sceCd_scmd_prechk(int command);
int sceSifCallRpc(void* client, unsigned function, unsigned mode, void* send, int send_size,
                  void* receive, int receive_size, void* end_function, void* end_parameter);
int norrath_player_signal_semaphore(int semaphore);
int scePrintf(const char* format, ...);
static const volatile unsigned char* reply_bytes() {
#if defined(NORRATH_HOST_CONTRACT)
    return _sceCd_scmdrdata;
#else
    const unsigned address = 0x20000000u | static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_sceCd_scmdrdata));
    return reinterpret_cast<const volatile unsigned char*>(static_cast<__UINTPTR_TYPE__>(address));
#endif
}
unsigned sceCdReadClock(unsigned char* record) {
    if (_sceCd_scmd_prechk(15) == 0) return 0u;
    if (SCE_CD_debug > 0) scePrintf("Libcdvd call Clock read 1\n");
    if (sceSifCallRpc(_sceCd_cd_scmd, 1u, 0u, nullptr, 0, _sceCd_scmdrdata, 16, nullptr, nullptr) < 0) {
        norrath_player_signal_semaphore(_sceCd_scmd_semid);
        return 0u;
    }
    const volatile unsigned char* reply = reply_bytes();
    for (int index = 0; index < 8; ++index) record[index] = reply[4 + index];
    if (SCE_CD_debug > 0) scePrintf("Libcdvd call Clock read 2\n");
    const unsigned result = *reinterpret_cast<const volatile unsigned*>(reply);
    norrath_player_signal_semaphore(_sceCd_scmd_semid);
    return result;
}
}
