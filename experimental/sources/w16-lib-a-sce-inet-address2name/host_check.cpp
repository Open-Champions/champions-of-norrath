// Host check, not a plan input.
#include <cstdio>
#include "address2name.cpp"
static int mc_ret, rpc_ret, sc_ret, reply, calls, mc_off, rpc_fn, rpc_snd, rpc_rcv, sc_off, seen[9];
extern "C" int MemcpyD(void*, void* b, int off, const void*, int) { ++calls; mc_off = off; for (int i = 0; i < 9; ++i) seen[i] = static_cast<int*>(b)[i]; return mc_ret; }
extern "C" int CallRpc(void*, int fn, void* b, int s, int r) { ++calls; rpc_fn = fn; rpc_snd = s; rpc_rcv = r; static_cast<int*>(b)[0] = reply; return rpc_ret; }
extern "C" int StrcpyS(void*, char*, void*, int off) { ++calls; sc_off = off; return sc_ret; }
int main() {
    int o, b[12]; char t[8]; char a[16];
    for (int i = 0; i < 12; ++i) b[i] = 0x5a5a;
    mc_ret = rpc_ret = sc_ret = 0; reply = 6;
    if (sceInetAddress2Name(&o, b, 0x80, t, 64, a, 7, 8, 99) != 6 || calls != 3) return 1;
    if (mc_off != 4 || seen[0] != 0x80 || seen[1] != 64 || seen[2] != 7 || seen[3] != 8) return 2;
    if (rpc_fn != 18 || rpc_snd != 36 || rpc_rcv != 96 || sc_off != 8 || b[8] != 99) return 3;
    calls = 0;
    sceInetAddress2Name(&o, b, 0x7f, t, 64, a, 7, 8, 99);
    if (b[8] != 0) return 4;
    calls = 0; mc_ret = -3;
    if (sceInetAddress2Name(&o, b, 0, t, 64, a, 0, 0, 0) != -3 || calls != 1) return 5;
    calls = 0; mc_ret = 0; rpc_ret = -1;
    if (sceInetAddress2Name(&o, b, 0, t, 64, a, 0, 0, 0) != -542 || calls != 2) return 6;
    calls = 0; rpc_ret = 0; sc_ret = -9;
    if (sceInetAddress2Name(&o, b, 0, t, 64, a, 0, 0, 0) != -9 || calls != 3) return 7;
    std::puts("address2name check passed");
    return 0;
}
