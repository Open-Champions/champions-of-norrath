// Host check, not a plan input.
#include <cstdio>
#include <cstring>
#define printf norrath_host_printf
#include "up_interface.cpp"
#undef printf
extern "C" { int m_rpcRunning; unsigned char m_cd[64]; alignas(8) unsigned char m_buf[320]; }
static int prints, rpcs, fn, snd, rcv; static void* c; static void* s; static void* r; static int word;
extern "C" int norrath_host_printf(const char* f, ...) { ++prints; return std::strcmp(f, "**** eznetctl RPC server not currently running\n") ? 99 : 0; }
extern "C" int sceSifCallRpc(void* cl, unsigned f, unsigned m, void* se, int ss, void* re, int rs, void* e, void* ea) {
    ++rpcs; c = cl; fn = f; snd = ss; rcv = rs; s = se; r = re; word = *static_cast<int*>(se);
    return (m || e || ea) ? 7 : 0;
}
int main() {
    m_rpcRunning = 0;
    if (ezNetCtlUpInterface(3) != -1 || prints != 1 || rpcs != 0) return 1;
    m_rpcRunning = 4;
    if (ezNetCtlUpInterface(3) != 0 || rpcs != 1 || c != m_cd || s != m_buf || r != m_buf || fn != 5 || snd != 4 || rcv != 4 || word != 3) return 2;
    std::puts("up interface check passed");
    return 0;
}
