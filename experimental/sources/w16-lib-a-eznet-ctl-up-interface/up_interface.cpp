// Observed: ezNetCtlUpInterface at 0x010bfe08, 116 bytes, pinned SLUS_205.65, the sibling of ezNetCtlDownInterface in the
// eznetctl client file. With (index):
//  1. A zero m_rpcRunning (0x01192c6c) prints "**** eznetctl RPC server not currently running\n" and returns -1.
//  2. Otherwise m_buf word 0 = index, then sceSifCallRpc(m_cd, 5, 0, m_buf, 4, m_buf, 4, 0, 0). The call result is
//     discarded and the function returns 0.
// Inferred: function 5 brings the interface with that index up (the down routine uses function 6).
// Unknown: what the server does with the index, and the reply contents.
extern "C" {
extern int m_rpcRunning;
extern unsigned char m_cd[];
extern unsigned char m_buf[];
int printf(const char* format, ...);
int sceSifCallRpc(void* client, unsigned function, unsigned mode, void* send, int send_size,
                  void* receive, int receive_size, void* end_function, void* end_parameter);

int ezNetCtlUpInterface(int index) {
    if (m_rpcRunning == 0) {
        printf("**** eznetctl RPC server not currently running\n");
        return -1;
    }
    *reinterpret_cast<int*>(m_buf) = index;
    sceSifCallRpc(m_cd, 5u, 0u, m_buf, 4, m_buf, 4, nullptr, nullptr);
    return 0;
}
}
