// Observed: sceMcInit creates two semaphores when semaidRegFunc is negative,
// then synchronizes, binds the memory-card RPC client, and checks the
// service version words in retval. A failed bind prints and does not return.
// Inferred: the three stored semaphore-parameter words are the only ones
// this function writes; the remaining parameter bytes are cleared here
// because the image leaves them as whatever was already on the stack.
// Unknown: who publishes the version words in retval.

extern "C" int semaidRegFunc;
extern "C" int semaid_timerfunc;
extern "C" unsigned char mcClientID[];
extern "C" unsigned char sifParamOrd[];
extern "C" unsigned char retval[];

extern "C" int CreateSema(void* param);
extern "C" int sceMcSync(int mode, int a, int b);
extern "C" int WaitSema(int id);
extern "C" int sceSifInitRpc(int mode);
extern "C" int sceSifBindRpc(void* client, unsigned int server, int mode);
extern "C" int scePrintf(const char* text);
extern "C" int sceSifCallRpc(void* client, int command, int mode, void* send,
                             int send_size, void* receive, int receive_size,
                             void* done, void* done_arg);
extern "C" int SignalSema(int id);

static int load_word(const unsigned char* base, int offset) {
    unsigned value = static_cast<unsigned>(base[offset]) |
                     (static_cast<unsigned>(base[offset + 1]) << 8) |
                     (static_cast<unsigned>(base[offset + 2]) << 16) |
                     (static_cast<unsigned>(base[offset + 3]) << 24);
    return static_cast<int>(value);
}

static void store_word(unsigned char* base, int offset, int value) {
    unsigned bits = static_cast<unsigned>(value);
    base[offset] = static_cast<unsigned char>(bits);
    base[offset + 1] = static_cast<unsigned char>(bits >> 8);
    base[offset + 2] = static_cast<unsigned char>(bits >> 16);
    base[offset + 3] = static_cast<unsigned char>(bits >> 24);
}

static void store_sema_word(unsigned char* param, int offset, int value) {
    store_word(param, offset, value);
}

extern "C" int sceMcInit() {
    if (semaidRegFunc < 0) {
        unsigned char param[24] = {};
        store_sema_word(param, 4, 1);
        store_sema_word(param, 8, 1);
        store_sema_word(param, 20, 0);
        semaidRegFunc = CreateSema(param);
        store_sema_word(param, 4, 1);
        store_sema_word(param, 8, 0);
        store_sema_word(param, 20, 0);
        semaid_timerfunc = CreateSema(param);
    }
    sceMcSync(0, 0, 0);
    WaitSema(semaidRegFunc);
    sceSifInitRpc(0);
    for (;;) {
        volatile int remaining = 0x100000;
        while (remaining != 0) {
            remaining = remaining - 1;
        }
        int bound = sceSifBindRpc(mcClientID, 0x80000400u, 0);
        if (bound < 0) {
            scePrintf("bind error libmc \n");
            volatile int hang = 1;
            while (hang) {
            }
        }
        if (load_word(mcClientID, 36) != 0) {
            break;
        }
    }
    int rpc = sceSifCallRpc(mcClientID, 254, 0, sifParamOrd, 48, retval, 12,
                            0, 0);
    SignalSema(semaidRegFunc);
    if (rpc < 0) {
        store_word(mcClientID, 36, 0);
        return rpc - 100;
    }
    if (load_word(retval, 4) < 522) {
        scePrintf("libmc: too old release of mcserv.irx\n");
        store_word(mcClientID, 36, 0);
        return -120;
    }
    if (load_word(retval, 8) < 526) {
        scePrintf("libmc: too old release of mcman.irx\n");
        store_word(mcClientID, 36, 0);
        return -121;
    }
    return load_word(retval, 0);
}
