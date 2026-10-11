// Observed sceSifQueryMaxFreeMemSize at 0x002dc420, 112 bytes.
// The word _bind at 0x00444a0c is a file-backed .data word whose image value
// is -1. Its original binding is local. It is published global because the
// paired total-free query reads the same word. A negative word returns 0
// and does not call. Otherwise sceSifCallRpc is called with the local client
// object at 0x008e1cc0, operation 6, mode 0, no send buffer, the local
// receive object at 0x008e1d00, receive size 4, and no completion function.
// A negative call result returns -1. A nonnegative result returns the first
// receive word. Both objects are .bss, st_size 0. The 64-byte lengths are
// the spans to the next symbols, rdata and sdata. This function reads four
// receive bytes. What the operation does on the IOP is unknown.
extern "C" int sceSifCallRpc(void* client, int operation, int mode, void* send, int send_size,
                             void* receive, int receive_size, void* completion,
                             void* completion_argument);

extern "C" {
int _bind = -1;
}

static unsigned char cd[64];
static union {
    int word;
    unsigned char bytes[64];
} rdata;

extern "C" int sceSifQueryMaxFreeMemSize() {
    if (_bind < 0)
        return 0;
    const int call = sceSifCallRpc(cd, 6, 0, 0, 0, &rdata, 4, 0, 0);
    if (call < 0)
        return -1;
    return rdata.word;
}

// Observed sceSifQueryTotalFreeMemSize at 0x002dc490, 112 bytes. It uses the
// same _bind word, client, and receive object as the maximum-free query.
// The operation is 7. The negative-word, negative-call, and receive-word
// results match that query.
extern "C" int sceSifQueryTotalFreeMemSize() {
    if (_bind < 0)
        return 0;
    const int call = sceSifCallRpc(cd, 7, 0, 0, 0, &rdata, 4, 0, 0);
    if (call < 0)
        return -1;
    return rdata.word;
}
