#include <cstdio>
#include "mc_format.cpp"
extern "C" int mcClientID[10] = {}; extern "C" int sifParamOrd[12] = {}; extern "C" int retval[16] = {};
extern "C" int mcRunCmdNo = 0; extern "C" int semaidRegFunc = 6;
static int poll_result = 0, rpc_result = 0, signals = 0, polls = 0, rpc_calls = 0;
static unsigned cmd, mode; static int send_size, recv_size; static void* done_arg = &signals;
static void (*done_fn)(void*); static void* sent; static void* received;
extern "C" int PollSema(int id) { ++polls; return id == 6 ? poll_result : -1; }
extern "C" int SignalSema(int id) { if (id == 6) ++signals; return 0; }
extern "C" void mceSifRpcEndFunc(void*) {}
extern "C" int sceSifCallRpc(void* client, unsigned c, unsigned m, void* send, int ss, void* recv, int rs,
                             void (*done)(void*), void* arg) {
    ++rpc_calls; cmd = c; mode = m; sent = send; send_size = ss; received = recv; recv_size = rs; done_fn = done; done_arg = arg;
    return client == mcClientID ? rpc_result : -9;
}
int main() {
    if (sceMcFormat(1, 2) != -100 || polls != 0) return 1;
    mcClientID[9] = 1; poll_result = -1;
    if (sceMcFormat(1, 2) != -200 || rpc_calls != 0 || sifParamOrd[1] != 0) return 2;
    poll_result = 0; rpc_result = 0;
    if (sceMcFormat(1, 2) != 0) return 3;
    if (cmd != 16 || mode != 1 || sent != sifParamOrd || send_size != 48 || received != retval || recv_size != 4 ||
        done_fn != mceSifRpcEndFunc || done_arg != nullptr || sifParamOrd[0] != 0 || sifParamOrd[1] != 1 ||
        sifParamOrd[2] != 2 || mcRunCmdNo != 16 || signals != 0) return 4;
    mcRunCmdNo = 0; rpc_result = 3;
    if (sceMcFormat(0, 1) != 3 || mcRunCmdNo != 0 || signals != 1) return 5;
    std::puts("sceMcFormat contract passed");
    return 0;
}
