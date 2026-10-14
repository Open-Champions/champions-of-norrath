// Host check, not a plan input.
#include <cstdio>
#include "register_handler.cpp"
static int rpc_ret, reply, fn, snd, rcv; static void* obj; static void* buf;
extern "C" int CallRpc(void* o, int f, void* b, int s, int r) { obj = o; fn = f; buf = b; snd = s; rcv = r; static_cast<int*>(b)[0] = reply; return rpc_ret; }
int main() {
    int o, b[2] = {0, 0};
    rpc_ret = 0; reply = 9;
    if (sceLibnetRegisterHandler(&o, b) != 9 || obj != &o || buf != b || fn != 30 || snd != 0 || rcv != 4) return 1;
    rpc_ret = -1; reply = 4;
    if (sceLibnetRegisterHandler(&o, b) != -542) return 2;
    rpc_ret = 3; reply = -8;
    if (sceLibnetRegisterHandler(&o, b) != -8) return 3;
    std::puts("register handler check passed");
    return 0;
}
