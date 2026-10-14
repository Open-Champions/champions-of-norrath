// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -fno-builtin -o /tmp/c host_contract.cpp lgaud_start_recording.cpp
#include <cstdarg>
#include <cstdio>
#include <cstring>
extern "C" {
int norrath_lgaud_semaphore_view = 5;
unsigned char* norrath_lgaud_request_buffer_view;
unsigned char norrath_lgaud_rpc_client_view[40];
static char printed[128]; static int prints, rpc_calls, rpc_result, order[8], order_n, wait_id = -9, sig_id = -9;
static unsigned rpc_cmd, rpc_mode; static int rpc_ss, rpc_rs; static void *rpc_client, *rpc_send, *rpc_recv, *rpc_end, *rpc_param;
static unsigned char at_call[64];
int printf(const char* format, ...) {
    va_list a; va_start(a, format); std::vsnprintf(printed, 128, format, a); va_end(a); ++prints; order[order_n++] = 1; return 0;
}
int WaitSema(int s) { wait_id = s; order[order_n++] = 2; return 0; }
int SignalSema(int s) { sig_id = s; order[order_n++] = 3; return 0; }
int sceSifCallRpc(void* client, unsigned command, unsigned mode, void* send, int ss, void* recv, int rs, void* end, void* param) {
    ++rpc_calls; order[order_n++] = 4;
    rpc_client = client; rpc_cmd = command; rpc_mode = mode; rpc_send = send; rpc_ss = ss; rpc_recv = recv; rpc_rs = rs; rpc_end = end; rpc_param = param;
    std::memcpy(at_call, send, 32);
    unsigned reply = 0x600d; std::memcpy(send, &reply, 4);
    return rpc_result;
}
unsigned lgAudStartRecording(unsigned handle);
}
int main() {
    alignas(16) unsigned char buffer[64];
    norrath_lgaud_request_buffer_view = buffer;
    std::memset(buffer, 0x5a, sizeof buffer);
    norrath_lgaud_semaphore_view = -1;
    if (lgAudStartRecording(0x77) != 0x8000000au || rpc_calls != 0 || prints != 0 || wait_id != -9) return 1;
    norrath_lgaud_semaphore_view = 5;
        order_n = 0;
    if (lgAudStartRecording(0x77) != 0x600d) return 2;
    if (order_n != 3 || order[0] != 2 || order[1] != 4 || order[2] != 3 || wait_id != 5 || sig_id != 5) return 3;
    if (rpc_client != norrath_lgaud_rpc_client_view || rpc_cmd != 4 || rpc_mode != 0 || rpc_send != buffer || rpc_recv != buffer ||
        rpc_ss != 32 || rpc_rs != 32 || rpc_end != nullptr || rpc_param != nullptr) return 4;
    unsigned handle; std::memcpy(&handle, at_call + 12, 4);
    if (handle != 0x77 || at_call[8] != 0x5a || at_call[16] != 0x5a) return 5;                 // only word 3 is written
    rpc_result = 2; order_n = 0;
    if (lgAudStartRecording(0x77) != 0x80000000u || order_n != 4 || order[2] != 1 || order[3] != 3) return 6;
    if (std::strcmp(printed, "lgAudStartRecording: rpc call failed\n") != 0) return 7;
    return 0;
}
