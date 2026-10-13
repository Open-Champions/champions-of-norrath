// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_contract.cpp mc_read.cpp
#include <cstdint>
#include <initializer_list>
extern "C" {
int mcClientID[16], sifParamOrd[12], retval[4], mcRunCmdNo, semaidRegFunc = 6;
int norrath_libmc_param_next_view[48];
static int poll_result, rpc_result, signals, polls, rpc_calls, order_ok = 1, flushes;
static const void* flush_addr[4];
static int flush_size[4];
static unsigned rpc_command, rpc_mode;
static void* rpc_client; static const void* rpc_send; static int rpc_send_size, rpc_recv_size;
static const void* rpc_recv; static void (*rpc_done)(void*); static void* rpc_done_arg;
int PollSema(int id) { ++polls; return id == 6 ? poll_result : -1; }
int SignalSema(int id) { if (id != 6) return -1; ++signals; return 0; }
void sceSifWriteBackDCache(void* address, int size) {
    if (flushes < 4) { flush_addr[flushes] = address; flush_size[flushes] = size; }
    ++flushes;
}
void mceIntrReadFixAlign(void*) {}
int sceSifCallRpc(void* client, unsigned command, unsigned mode, void* send, int send_size, void* receive,
                  int receive_size, void (*done)(void*), void* done_arg) {
    ++rpc_calls;
    if (flushes != 2) order_ok = 0;  // both write-backs happen before the call
    rpc_client = client; rpc_command = command; rpc_mode = mode; rpc_send = send; rpc_send_size = send_size;
    rpc_recv = receive; rpc_recv_size = receive_size; rpc_done = done; rpc_done_arg = done_arg;
    return rpc_result;
}
int sceMcRead(int fd, void* buffer, int size);
}
static int word_of(const void* p) { return static_cast<int>(static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(p))); }
static void reset() {
    polls = signals = rpc_calls = flushes = 0; order_ok = 1; mcRunCmdNo = 0; poll_result = 0; rpc_result = 0;
    for (int& w : sifParamOrd) w = 0x77777777;
}
int main() {
    char buffer[64];
    mcClientID[9] = 0;
    reset();
    if (sceMcRead(3, buffer, 16) != -100 || polls != 0 || rpc_calls != 0 || sifParamOrd[0] != 0x77777777) return 1;
    mcClientID[9] = 1;
    reset(); poll_result = -1;
    if (sceMcRead(3, buffer, 16) != -200 || rpc_calls != 0 || signals != 0 || sifParamOrd[0] != 0x77777777) return 2;
    reset();
    if (sceMcRead(7, buffer, 40) != 0 || rpc_calls != 1 || !order_ok) return 3;
    if (sifParamOrd[0] != 7 || sifParamOrd[3] != 40 || sifParamOrd[6] != word_of(buffer) ||
        sifParamOrd[7] != word_of(norrath_libmc_param_next_view)) return 4;
    for (int i : {1, 2, 4, 5, 8, 9, 10, 11}) if (sifParamOrd[i] != 0x77777777) return 5;  // nothing else is touched
    if (flush_addr[0] != buffer || flush_size[0] != 40 || flush_addr[1] != norrath_libmc_param_next_view ||
        flush_size[1] != 192) return 6;
    if (rpc_client != mcClientID || rpc_command != 5 || rpc_mode != 1 || rpc_send != sifParamOrd ||
        rpc_send_size != 48 || rpc_recv != retval || rpc_recv_size != 4 || !rpc_done ||
        rpc_done_arg != norrath_libmc_param_next_view) return 7;
    if (mcRunCmdNo != 5 || signals != 0) return 8;
    reset(); rpc_result = -5;
    if (sceMcRead(7, buffer, 40) != -5 || mcRunCmdNo != 0 || signals != 1) return 9;
    reset(); rpc_result = 2;
    if (sceMcRead(1, buffer, 0) != 2 || mcRunCmdNo != 0 || signals != 1 || sifParamOrd[3] != 0) return 10;
    return 0;
}
