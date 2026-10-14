// Host check, not a plan input.
#include <cstdio>
#include <cstring>
#include "address2string.cpp"
static int mc_ret, rpc_ret, sc_ret, reply, calls, mc_off, mc_size, rpc_fn, rpc_snd, rpc_rcv, sc_off, buf0_at_mc;
extern "C" int MemcpyD(void*, void* b, int off, const void*, int size) { ++calls; mc_off = off; mc_size = size; buf0_at_mc = static_cast<int*>(b)[0]; return mc_ret; }
extern "C" int CallRpc(void*, int fn, void* b, int s, int r) { ++calls; rpc_fn = fn; rpc_snd = s; rpc_rcv = r; static_cast<int*>(b)[0] = reply; return rpc_ret; }
extern "C" int StrcpyS(void*, char*, void*, int off) { ++calls; sc_off = off; return sc_ret; }
int main() {
    int o, b[12]; char t[8]; char a[16];
    mc_ret = rpc_ret = sc_ret = 0; reply = 6;
    if (sceInetAddress2String(&o, b, t, 100, a) != 6 || calls != 3) return 1;
    if (mc_off != 1 || mc_size != 16 || buf0_at_mc != 100 || rpc_fn != 7 || rpc_snd != 20 || rpc_rcv != 120 || sc_off != 5) return 2;
    calls = 0; mc_ret = -3;
    if (sceInetAddress2String(&o, b, t, 100, a) != -3 || calls != 1) return 3;
    calls = 0; mc_ret = 0; rpc_ret = -1;
    if (sceInetAddress2String(&o, b, t, 100, a) != -542 || calls != 2) return 4;
    calls = 0; rpc_ret = 0; sc_ret = -9;
    if (sceInetAddress2String(&o, b, t, 100, a) != -9 || calls != 3) return 5;
    std::puts("address2string check passed");
    return 0;
}
