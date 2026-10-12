#include <cstdio>
#include "get_error.cpp"
extern "C" int _sceCd_scmd_semid = 77;
extern "C" alignas(4) unsigned char _sceCd_cd_scmd[40] = {};
extern "C" alignas(4) unsigned char _sceCd_scmdrdata[0x420] = {};
static int prechk_result = 1, prechk_arg = -1, rpc_result = 0, signals = 0, signaled = -1, rpc_calls = 0;
static unsigned bad_args = 0;
extern "C" int _sceCd_scmd_prechk(int c) { prechk_arg = c; return prechk_result; }
extern "C" int sceSifCallRpc(void* client, unsigned fn, unsigned mode, void* send, int ss, void* recv, int rs, void* ef, void* ep) {
    ++rpc_calls;
    if (client != _sceCd_cd_scmd || fn != 4u || mode != 0u || send != nullptr || ss != 0 || recv != _sceCd_scmdrdata || rs != 4 || ef || ep) bad_args = 1;
    *reinterpret_cast<unsigned*>(_sceCd_scmdrdata) = 0x12345678u;
    return rpc_result;
}
extern "C" int SignalSema(int s) { ++signals; signaled = s; return 0; }
int main() {
    prechk_result = 0;
    if (sceCdGetError() != -1 || prechk_arg != 3 || rpc_calls != 0 || signals != 0) return 1;
    prechk_result = 1; rpc_result = -1;
    if (sceCdGetError() != -1 || signals != 1 || signaled != 77 || bad_args) return 2;
    rpc_result = 0;
    if (sceCdGetError() != 0x12345678 || signals != 2 || signaled != 77 || bad_args) return 3;
    rpc_result = 5; *reinterpret_cast<unsigned*>(_sceCd_scmdrdata) = 0;
    *reinterpret_cast<unsigned*>(_sceCd_scmdrdata) = 0xffffffffu;
    if (sceCdGetError() != 0x12345678) return 4;   // RPC stub rewrites the word each call
    std::puts("sceCdGetError contract passed");
    return 0;
}
