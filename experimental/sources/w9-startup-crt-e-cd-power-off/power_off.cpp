// Observed sceCdPowerOff at 0x002e3e60, 180 bytes. The argument is a result pointer.
// _sceCd_scmd_prechk(33) returning zero returns 0 with nothing else done. Otherwise
// sceSifCallRpc runs on _sceCd_cd_scmd (0x00447c00) with function 33, mode 0, no send
// area, _sceCd_scmdrdata (0x00447280) as an 8-byte receive area and no end function.
// A negative result signals _sceCd_scmd_semid (0x004460ac) and returns 0. Otherwise the
// word at receive+4 is stored through the argument and the word at receive+0 is returned,
// both read through the uncached alias (address | 0x20000000). The first word is loaded in
// the delay slot of the signal call, so it is read before the semaphore is released.
// Inferred: the first receive word is the command status and the second the power-off
// state; the observed code does not name them.
extern "C" {
extern int _sceCd_scmd_semid;
extern unsigned char _sceCd_cd_scmd[];
extern unsigned char _sceCd_scmdrdata[];
int _sceCd_scmd_prechk(int command);
int sceSifCallRpc(void* client, unsigned function, unsigned mode, void* send, int send_size,
                  void* receive, int receive_size, void* end_function, void* end_parameter);
int SignalSema(int semaphore);

static unsigned uncached_word(const unsigned char* address) {
#if defined(NORRATH_HOST_CONTRACT)
    return *reinterpret_cast<const unsigned*>(address);
#else
    const __UINTPTR_TYPE__ alias = 0x20000000u | reinterpret_cast<__UINTPTR_TYPE__>(address);
    return *reinterpret_cast<const volatile unsigned*>(alias);
#endif
}

int sceCdPowerOff(int* result) {
    if (_sceCd_scmd_prechk(33) == 0) return 0;
    if (sceSifCallRpc(_sceCd_cd_scmd, 33u, 0u, nullptr, 0, _sceCd_scmdrdata, 8, nullptr, nullptr) < 0) {
        SignalSema(_sceCd_scmd_semid);
        return 0;
    }
    *result = static_cast<int>(uncached_word(_sceCd_scmdrdata + 4));
    const int status = static_cast<int>(uncached_word(_sceCd_scmdrdata));
    SignalSema(_sceCd_scmd_semid);
    return status;
}
}
