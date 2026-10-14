// Observed _lf_bind at 0x002dc600, 256 bytes, local binding. A nonnegative
// _bind_check returns 0. Otherwise it binds the local client at 0x008e2080
// to server 0x80000006. That client is .bss, st_size 0, and _lfversion is 40
// bytes later. Its original name, cd, is not unique, so it is published as
// norrath_loadfile_client. The module calls in sif-keyboard use the same
// client, so that copy must refer to this one. A negative bind returns -1.
// A zero word at client offset 36 counts down from 0x00100000 to -1 and tries
// again. A nonzero word stores 0 in _bind_check, calls sceSifCallRpc on that
// client with operation 255, mode 0, no send buffer, a four-byte receive, and
// no completion function. The receive object is the first word of _senddata
// at 0x008e1e80, the 512-byte local the module calls also use. It is
// published here under its original name. A negative call returns 0xfffeffff.
// A nonnegative call copies those four bytes to _lfversion and returns 0.
extern "C" int _bind_check;
extern "C" unsigned char _lfversion[4];
extern "C" int sceSifBindRpc(void* client, unsigned server, int mode);
extern "C" int sceSifCallRpc(void* client, int operation, int mode, void* send, int send_size,
                             void* receive, int receive_size, void* completion,
                             void* completion_argument);

extern "C" {
alignas(64) unsigned char norrath_loadfile_client[40] = {};
alignas(64) unsigned char _senddata[512] = {};
}

static unsigned load_word(const unsigned char* slot) {
    return slot[0] | (slot[1] << 8) | (slot[2] << 16) | (slot[3] << 24);
}

static void store_word(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}

static void wait_for_server() {
    volatile int remaining = 0x00100000;
#if defined(NORRATH_HOST_CONTRACT)
    extern int norrath_lf_bind_wait_steps;
#endif
    do {
        remaining = remaining - 1;
#if defined(NORRATH_HOST_CONTRACT)
        norrath_lf_bind_wait_steps += 1;
#endif
    } while (remaining != -1);
}

extern "C" int _lf_bind() {
    if (_bind_check >= 0)
        return 0;
    for (;;) {
        const int bound = sceSifBindRpc(norrath_loadfile_client, 0x80000006u, 0);
        if (bound < 0)
            return -1;
        if (load_word(norrath_loadfile_client + 36) == 0) {
            wait_for_server();
            continue;
        }
        _bind_check = 0;
        const int call = sceSifCallRpc(norrath_loadfile_client, 255, 0, nullptr, 0, _senddata, 4, nullptr, nullptr);
        if (call < 0)
            return static_cast<int>(0xfffeffffu);
        store_word(_lfversion, load_word(_senddata));
        return 0;
    }
}
