#include "exit_cnf.cpp"

int main() {
    m_rpcRunning = 6;
    if (exitEzNetCnf() != 0) return 1;
    if (m_rpcRunning != 0) return 2;
    if (exitEzNetCnf() != 0 || m_rpcRunning != 0) return 3;
    return 0;
}
