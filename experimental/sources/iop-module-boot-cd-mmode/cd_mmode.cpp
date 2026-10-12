// Observed sceCdMmode at 0x002e3f18, 208 bytes. _sceCd_scmd_prechk(34) must
// be nonzero. That stores the mode in the first word of _sceCd_scmdsdata at
// 0x004476c0 and writes those four bytes back. The RPC client is
// _sceCd_cd_scmd. The function, send size, and receive size are the words
// rpc_mmode, outcnt_mmode, and incnt_mmode. Their image values are 0x22, 4,
// and 4. The receive buffer is _sceCd_scmdrdata. A negative RPC signals
// _sceCd_scmd_semid and returns 0. Otherwise the first receive word is read
// through the uncached alias before the same signal, and that word is the
// result. The _sceCd_ objects stay imports.
extern "C" {
int rpc_mmode = 0x22;
int outcnt_mmode = 4;
int incnt_mmode = 4;
}
extern "C" int _sceCd_scmd_prechk(int command);
extern "C" unsigned char _sceCd_scmdsdata[];
extern "C" unsigned char _sceCd_cd_scmd[];
extern "C" unsigned char _sceCd_scmdrdata[];
extern "C" int _sceCd_scmd_semid;
extern "C" void sceSifWriteBackDCache(void* address, int bytes);
extern "C" int sceSifCallRpc(void* client, int operation, int mode, void* send, int send_size,
                             void* receive, int receive_size, void* completion,
                             void* completion_argument);
extern "C" int SignalSema(int semaphore);

static void store_word(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}

static int uncached_word(unsigned char* object) {
#if defined(NORRATH_HOST_CONTRACT)
    const unsigned char* address = object;
#else
    const unsigned bits = 0x20000000u | static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(object));
    const unsigned char* address = reinterpret_cast<const unsigned char*>(static_cast<__UINTPTR_TYPE__>(bits));
#endif
    const unsigned value = *reinterpret_cast<const volatile unsigned*>(address);
    return static_cast<int>(value);
}

extern "C" int sceCdMmode(int mode) {
    if (_sceCd_scmd_prechk(34) == 0)
        return 0;
    store_word(_sceCd_scmdsdata, static_cast<unsigned>(mode));
    sceSifWriteBackDCache(_sceCd_scmdsdata, 4);
    const int rpc = sceSifCallRpc(_sceCd_cd_scmd, rpc_mmode, 0, _sceCd_scmdsdata, outcnt_mmode,
                                  _sceCd_scmdrdata, incnt_mmode, nullptr, nullptr);
    if (rpc < 0) {
        SignalSema(_sceCd_scmd_semid);
        return 0;
    }
    const int word = uncached_word(_sceCd_scmdrdata);
    SignalSema(_sceCd_scmd_semid);
    return word;
}
