// Observed sceSifInitIopHeap at 0x002dc0b0, 136 bytes. It binds the local
// client at 0x008e1cc0 to server 0x80000003. That client is .bss, st_size 0,
// and rdata is 64 bytes later. Its original name, cd, is not unique, so it is
// published here as norrath_iop_heap_client. The two free-memory queries call
// through that same client, so the sif-keyboard copy must refer to this one.
// A negative bind returns -1. A zero word at client offset 36
// counts down from 0x00100000 to -1 and tries the bind again. A nonzero word
// stores 0 in _bind at 0x00444a0c and returns 0. _bind is the word already
// published by the heap-query unit. The countdown is kept volatile so -O2
// does not delete it. The original countdown is a register, with four nops
// in the body.
extern "C" int _bind;
extern "C" int sceSifBindRpc(void* client, unsigned server, int mode);

extern "C" {
alignas(64) unsigned char norrath_iop_heap_client[64] = {};
}

static unsigned load_word(const unsigned char* slot) {
    return slot[0] | (slot[1] << 8) | (slot[2] << 16) | (slot[3] << 24);
}

static void wait_for_server() {
    volatile int remaining = 0x00100000;
#if defined(NORRATH_HOST_CONTRACT)
    extern int norrath_iop_heap_wait_steps;
#endif
    do {
        remaining = remaining - 1;
#if defined(NORRATH_HOST_CONTRACT)
        norrath_iop_heap_wait_steps += 1;
#endif
    } while (remaining != -1);
}

extern "C" int sceSifInitIopHeap() {
    for (;;) {
        const int bound = sceSifBindRpc(norrath_iop_heap_client, 0x80000003u, 0);
        if (bound < 0)
            return -1;
        if (load_word(norrath_iop_heap_client + 36) == 0) {
            wait_for_server();
            continue;
        }
        _bind = 0;
        return 0;
    }
}
