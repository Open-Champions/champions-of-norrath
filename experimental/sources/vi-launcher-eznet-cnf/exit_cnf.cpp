// Observed exitEzNetCnf at 0x010bed98, 16 bytes. It returns 0 and stores
// 0 in the local word m_rpcRunning at 0x01192c68. That word is not the
// m_rpcRunning at 0x01192c6c. The file bytes are zero.
extern "C" {
static volatile int m_rpcRunning __asm__("m_rpcRunning");

int exitEzNetCnf() {
    m_rpcRunning = 0;
    return 0;
}
}
