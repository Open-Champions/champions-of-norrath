#include <cstdio>
#include <cstring>
#include <string>
#include "mc_delete.cpp"
extern "C" int mcClientID[10] = {}; extern "C" int retval[16] = {};
extern "C" alignas(16) int sifParamFname[0x450 / 4 + 4] = {};
extern "C" int mcRunCmdNo = 0; extern "C" int semaidRegFunc = 6;
static int poll_result = 0, rpc_result = 0, signals = 0, polls = 0, rpc_calls = 0, strncpy_calls = 0;
static unsigned cmd, mode; static int send_size, recv_size; static void* done_arg = &signals;
static void (*done_fn)(void*); static void* sent; static void* received;
extern "C" int PollSema(int id) { ++polls; return id == 6 ? poll_result : -1; }
extern "C" int SignalSema(int id) { if (id == 6) ++signals; return 0; }
extern "C" void mceSifRpcEndFunc(void*) {}
extern "C" char* strncpy(char* d, const char* s, __SIZE_TYPE__ n) {
    ++strncpy_calls;
    if (n != 1023u) return d;
    std::size_t i = 0;
    for (; i < n && s[i]; ++i) d[i] = s[i];
    for (; i < n; ++i) d[i] = 0;
    return d;
}
extern "C" int sceSifCallRpc(void* client, unsigned c, unsigned m, void* send, int ss, void* recv, int rs,
                             void (*done)(void*), void* arg) {
    ++rpc_calls; cmd = c; mode = m; sent = send; send_size = ss; received = recv; recv_size = rs; done_fn = done; done_arg = arg;
    return client == mcClientID ? rpc_result : -9;
}
int main() {
    if (sceMcDelete(1, 2, "a") != -100 || polls != 0) return 1;
    mcClientID[9] = 1; poll_result = -1;
    if (sceMcDelete(1, 2, "a") != -200 || rpc_calls != 0) return 2;
    poll_result = 0;
    if (sceMcDelete(1, 2, nullptr) != -210 || signals != 1 || rpc_calls != 0) return 3;
    if (sceMcDelete(1, 2, "") != -210 || signals != 2 || rpc_calls != 0 || strncpy_calls != 0) return 4;
    sifParamFname[2] = 99;
    std::string longname(1100, 'x');
    rpc_result = 0;
    if (sceMcDelete(1, 2, "BASLUS-20565") != 0) return 5;
    const char* block = reinterpret_cast<const char*>(sifParamFname);
    if (cmd != 15 || mode != 1 || sent != sifParamFname || send_size != 1044 || received != retval ||
        recv_size != 4 || done_fn != mceSifRpcEndFunc || done_arg != nullptr) return 6;
    if (sifParamFname[0] != 1 || sifParamFname[1] != 2 || sifParamFname[2] != 0 ||
        std::strcmp(block + 20, "BASLUS-20565") != 0 || block[1043] != 0 || mcRunCmdNo != 15) return 7;
    if (sceMcDelete(0, 0, longname.c_str()) != 0) return 8;
    if (block[20 + 1022] != 'x' || block[1043] != 0 || block[1042] != 'x') return 9;
    mcRunCmdNo = 0; rpc_result = 4; signals = 0;
    if (sceMcDelete(0, 0, "z") != 4 || mcRunCmdNo != 0 || signals != 1) return 10;
    std::puts("sceMcDelete contract passed");
    return 0;
}
