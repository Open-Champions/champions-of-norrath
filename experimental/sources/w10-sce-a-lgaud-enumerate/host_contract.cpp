// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -fno-builtin -o /tmp/c host_contract.cpp lgaud_enumerate.cpp
#include <cstdarg>
#include <cstdio>
#include <cstring>
extern "C" {
int norrath_lgaud_semaphore_view = 5;
unsigned char* norrath_lgaud_request_buffer_view;
unsigned char norrath_lgaud_rpc_client_view[40];
static char printed[128]; static int prints, rpc_calls, rpc_result, signals, order[8], order_n;
static unsigned rpc_cmd, rpc_mode; static int rpc_ss, rpc_rs; static void *rpc_client, *rpc_send, *rpc_recv, *rpc_end, *rpc_param;
static unsigned word2_at_call;
int printf(const char* format, ...) {
    va_list a; va_start(a, format); std::vsnprintf(printed, 128, format, a); va_end(a); ++prints; order[order_n++] = 1; return 0;
}
int WaitSema(int) { order[order_n++] = 2; return 0; }
int SignalSema(int) { ++signals; order[order_n++] = 3; return 0; }
int sceSifCallRpc(void* client, unsigned command, unsigned mode, void* send, int ss, void* recv, int rs, void* end, void* param) {
    ++rpc_calls; order[order_n++] = 4;
    rpc_client = client; rpc_cmd = command; rpc_mode = mode; rpc_send = send; rpc_ss = ss; rpc_recv = recv; rpc_rs = rs; rpc_end = end; rpc_param = param;
    unsigned char* b = static_cast<unsigned char*>(send);
    std::memcpy(&word2_at_call, b + 8, 4);
    unsigned status = 0x80000002u; std::memcpy(b, &status, 4);
    for (int i = 32; i < 368; ++i) b[i] = static_cast<unsigned char>(i);
    return rpc_result;
}
unsigned lgAudEnumerate(unsigned index, unsigned char* description);
}
int main() {
    alignas(16) static unsigned char buffer[400]; static unsigned char out[400];
    norrath_lgaud_request_buffer_view = buffer;
    std::memset(out, 0xee, sizeof out);
    norrath_lgaud_semaphore_view = -1;
    if (lgAudEnumerate(2, out) != 0x8000000au || rpc_calls != 0 || prints != 0) return 1;
    norrath_lgaud_semaphore_view = 5;
    if (lgAudEnumerate(2, nullptr) != 0x80000004u || std::strcmp(printed, "lgAudEnumerate: invalid parameter (description)\n") != 0 || rpc_calls != 0) return 2;
    order_n = 0;
    if (lgAudEnumerate(9, out) != 0x80000002u) return 3;
    if (order_n != 3 || order[0] != 2 || order[1] != 4 || order[2] != 3) return 4;
    if (word2_at_call != 9 || rpc_client != norrath_lgaud_rpc_client_view || rpc_cmd != 1 || rpc_mode != 0 || rpc_send != buffer ||
        rpc_recv != buffer || rpc_ss != 32 || rpc_rs != 368 || rpc_end != nullptr || rpc_param != nullptr) return 5;
    for (int i = 0; i < 332; ++i) if (out[i] != static_cast<unsigned char>(32 + i)) return 6;       // bytes 32..363 of the buffer
    for (int i = 332; i < 400; ++i) if (out[i] != 0xee) return 7;                                      // and no more
    // unaligned destination takes the same bytes
    std::memset(out, 0xee, sizeof out);
    lgAudEnumerate(1, out + 3);
    for (int i = 0; i < 332; ++i) if (out[3 + i] != static_cast<unsigned char>(32 + i)) return 8;
    if (out[2] != 0xee || out[335] != 0xee) return 9;
    rpc_result = 3; order_n = 0;
    if (lgAudEnumerate(1, out) != 0x80000000u || order_n != 4 || order[2] != 1 || order[3] != 3) return 10;
    if (std::strcmp(printed, "lgAudEnumerate: rpc call failed\n") != 0) return 11;
    return 0;
}
