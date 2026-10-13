// Host check only; not an EE build input.
#include <cstring>
extern "C" {
int mcClientID[16], retval[4], mcRunCmdNo, semaidRegFunc = 6;
unsigned char sifParamFname[1044];
static int poll_result, rpc_result, signals, rpc_calls;
int PollSema(int id) { return id == 6 ? poll_result : -1; }
int SignalSema(int) { ++signals; return 0; }
char* strncpy(char* d, const char* s, unsigned long n) {
    unsigned long i = 0;
    for (; i < n && s[i]; ++i) d[i] = s[i];
    for (; i < n; ++i) d[i] = 0;
    return d;
}
void mceSifRpcEndFunc(void*) {}
int sceSifCallRpc(void* client, unsigned command, unsigned mode, void* send, int send_size,
                  void* receive, int receive_size, void (*done)(void*), void* done_arg) {
    ++rpc_calls;
    if (client != mcClientID || command != 2 || mode != 1 || send != sifParamFname || send_size != 1044 ||
        receive != retval || receive_size != 4 || done != mceSifRpcEndFunc || done_arg) return -99;
    return rpc_result;
}
int sceMcOpen(int port, int slot, const char* name, int mode);
}
int main() {
    if (sceMcOpen(0, 0, "a", 1) != -100) return 1;
    mcClientID[9] = 5; poll_result = -1;
    if (sceMcOpen(0, 0, "a", 1) != -200) return 2;
    poll_result = 0;
    if (sceMcOpen(0, 0, nullptr, 1) != -210 || signals != 1) return 3;
    if (sceMcOpen(0, 0, "", 1) != -210 || signals != 2 || rpc_calls != 0) return 4;
    rpc_result = 0;
    std::memset(sifParamFname, 0xee, sizeof sifParamFname);
    if (sceMcOpen(1, 2, "BWSAVE/x", 0x203) != 0 || mcRunCmdNo != 2) return 5;
    const int* words = reinterpret_cast<const int*>(sifParamFname);
    if (words[0] != 1 || words[1] != 2 || words[2] != 0x203) return 6;
    if (std::strcmp(reinterpret_cast<const char*>(sifParamFname) + 20, "BWSAVE/x") != 0) return 7;
    if (sifParamFname[1043] != 0 || sifParamFname[1042] != 0) return 8;
    rpc_result = -2; mcRunCmdNo = 0;
    if (sceMcOpen(1, 2, "x", 0) != -2 || mcRunCmdNo != 0 || signals != 3) return 9;
    return 0;
}
