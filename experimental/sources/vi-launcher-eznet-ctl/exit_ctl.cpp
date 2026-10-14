// Observed exitEzNetCtl at 0x010bfc18, 16 bytes. It returns 0 and stores
// 0 in m_rpcRunning at 0x01192c6c. The file bytes are zero. The original
// word is local. It is published global so ezNetCtlDownInterface, which
// reads that same address, can share it from another unit.
extern "C" {
int m_rpcRunning;

int exitEzNetCtl() {
    m_rpcRunning = 0;
    return 0;
}
}
