// Host check only; not an EE build input.
extern "C" {
int mcClientID[16], sifParamOrd[12], retval[4], mcRunCmdNo, semaidRegFunc = 6;
static int poll_result, rpc_result, signals, rpc_calls, last_signal;
int PollSema(int id) { return id == 6 ? poll_result : -1; }
int SignalSema(int id) { ++signals; last_signal = id; return 0; }
void mceSifRpcEndFunc(void*) {}
int sceSifCallRpc(void* client, unsigned command, unsigned mode, void* send, int send_size,
                  void* receive, int receive_size, void (*done)(void*), void* done_arg) {
    ++rpc_calls;
    if (client != mcClientID || command != 4 || mode != 1 || send != sifParamOrd || send_size != 48 ||
        receive != retval || receive_size != 4 || done != mceSifRpcEndFunc || done_arg) return -99;
    return rpc_result;
}
int sceMcSeek(int fd, int offset, int origin);
}
int main() {
    if (sceMcSeek(1, 2, 3) != -100 || rpc_calls != 0) return 1;
    mcClientID[9] = 5; poll_result = -1;
    if (sceMcSeek(1, 2, 3) != -200 || rpc_calls != 0 || signals != 0) return 2;
    poll_result = 0; rpc_result = 0;
    if (sceMcSeek(7, -8, 2) != 0 || mcRunCmdNo != 4 || signals != 0) return 3;
    if (sifParamOrd[0] != 7 || sifParamOrd[4] != -8 || sifParamOrd[5] != 2) return 4;
    rpc_result = -3; mcRunCmdNo = 0;
    if (sceMcSeek(7, 0, 0) != -3 || mcRunCmdNo != 0 || signals != 1 || last_signal != 6) return 5;
    return 0;
}
