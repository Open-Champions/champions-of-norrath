#include "exit_ctl.cpp"

int main() {
    m_rpcRunning = 4;
    if (exitEzNetCtl() != 0) return 1;
    if (m_rpcRunning != 0) return 2;
    return 0;
}
