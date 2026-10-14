// Observed ezNetCtlDownInterface at 0x010bfe80, 116 bytes. It returns
// an int. A zero m_rpcRunning at 0x01192c6c prints the unnamed text below
// and returns -1. Any other word stores the argument at the first word of
// m_buf and calls sceSifCallRpc. The call result is discarded and this
// function returns 0.
//
// m_cd at 0x011e8500 and m_buf at 0x011e8540 are local, st_size 0. The next
// symbols are 64 and 320 bytes later. Those spans are the published sizes.
// What the RPC programs is unknown.
extern "C" {
extern int m_rpcRunning;
int printf(const char* text, ...);
int sceSifCallRpc(void* client, int operation, int mode, void* send, int send_size,
                  void* receive, int receive_size, void* completion, void* completion_argument);

static unsigned char m_cd[64] __asm__("m_cd");
static unsigned char m_buf[320] __asm__("m_buf");

int ezNetCtlDownInterface(int value) {
    if (m_rpcRunning == 0) {
        printf("**** eznetctl RPC server not currently running\n");
        return -1;
    }
    *reinterpret_cast<int*>(m_buf) = value;
    sceSifCallRpc(m_cd, 6, 0, m_buf, 4, m_buf, 4, 0, 0);
    return 0;
}
}
