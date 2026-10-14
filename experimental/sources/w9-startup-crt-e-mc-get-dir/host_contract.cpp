#include <cstdio>
#include <cstring>
#include <string>
#include "mc_get_dir.cpp"
extern "C" int mcClientID[10] = {}; extern "C" int retval[16] = {};
extern "C" alignas(16) int sifParamFname[0x450 / 4] = {};
extern "C" int mcRunCmdNo = 0; extern "C" int semaidRegFunc = 6;
static int poll_result = 0, rpc_result = 0, signals = 0, polls = 0, rpc_calls = 0, writebacks = 0, wb_size = -1;
static void* wb_address; static unsigned cmd, mode; static int send_size, recv_size; static void* done_arg = &signals;
static void (*done_fn)(void*); static void* sent; static void* received;
extern "C" int PollSema(int id) { ++polls; return id == 6 ? poll_result : -1; }
extern "C" int SignalSema(int id) { if (id == 6) ++signals; return 0; }
extern "C" void mceSifRpcEndFunc(void*) {}
extern "C" void sceSifWriteBackDCache(void* a, int n) { ++writebacks; wb_address = a; wb_size = n; }
extern "C" char* strncpy(char* d, const char* s, __SIZE_TYPE__ n) {
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
    unsigned char table[256];
    if (sceMcGetDir(1, 2, "a", 3, 4, table) != -100 || polls != 0) return 1;
    mcClientID[9] = 1; poll_result = -1;
    if (sceMcGetDir(1, 2, "a", 3, 4, table) != -200 || rpc_calls != 0) return 2;
    poll_result = 0;
    if (sceMcGetDir(1, 2, nullptr, 3, 4, table) != -210 || signals != 1) return 3;
    if (sceMcGetDir(1, 2, "", 3, 4, table) != -210 || signals != 2 || rpc_calls != 0) return 4;
    rpc_result = 0;
    if (sceMcGetDir(1, 2, "BA*", 7, 4, table) != 0) return 5;
    if (cmd != 13 || mode != 1 || sent != sifParamFname || send_size != 1044 || received != retval ||
        recv_size != 4 || done_fn != mceSifRpcEndFunc || done_arg != nullptr) return 6;
    const char* block = reinterpret_cast<const char*>(sifParamFname);
    if (sifParamFname[0] != 1 || sifParamFname[1] != 2 || sifParamFname[2] != 7 || sifParamFname[3] != 4 ||
        sifParamFname[4] != static_cast<int>(reinterpret_cast<__UINTPTR_TYPE__>(table)) ||
        std::strcmp(block + 20, "BA*") != 0 || block[1043] != 0 || mcRunCmdNo != 13) return 7;
    if (writebacks != 1 || wb_address != table || wb_size != 256) return 8;
    writebacks = 0;
    if (sceMcGetDir(1, 2, "x", 0, -1, table) != 0 || writebacks != 0) return 9;
    mcRunCmdNo = 0; rpc_result = 8; signals = 0;
    if (sceMcGetDir(0, 0, "z", 0, 1, table) != 8 || mcRunCmdNo != 0 || signals != 1) return 10;
    std::puts("sceMcGetDir contract passed");
    return 0;
}
