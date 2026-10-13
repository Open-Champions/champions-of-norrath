// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -fno-builtin -o /tmp/c host_contract.cpp lgaud_open.cpp
#include <cstdarg>
#include <cstdio>
#include <cstring>
extern "C" {
int norrath_lgaud_semaphore_view = 5;
unsigned char* norrath_lgaud_request_buffer_view;
unsigned char norrath_lgaud_rpc_client_view[40];
static char printed[4][128]; static int prints, waits, signals, rpc_calls, rpc_result, wait_id = -9, sig_id = -9, order[8], order_n;
static unsigned rpc_cmd, rpc_mode; static int rpc_ss, rpc_rs; static void *rpc_client, *rpc_send, *rpc_recv, *rpc_end, *rpc_param;
static unsigned char buffer_at_call[64];
int printf(const char* format, ...) {
    va_list a; va_start(a, format); std::vsnprintf(printed[prints & 3], 128, format, a); va_end(a); ++prints; order[order_n++] = 1; return 0;
}
int WaitSema(int s) { ++waits; wait_id = s; order[order_n++] = 2; return 0; }
int SignalSema(int s) { ++signals; sig_id = s; order[order_n++] = 3; return 0; }
int sceSifCallRpc(void* client, unsigned command, unsigned mode, void* send, int ss, void* recv, int rs, void* end, void* param) {
    ++rpc_calls; order[order_n++] = 4;
    rpc_client = client; rpc_cmd = command; rpc_mode = mode; rpc_send = send; rpc_ss = ss; rpc_recv = recv; rpc_rs = rs; rpc_end = end; rpc_param = param;
    std::memcpy(buffer_at_call, send, 64);
    unsigned* w = static_cast<unsigned*>(send); w[0] = 0x600d; w[3] = 0x4242;
    return rpc_result;
}
unsigned lgAudOpen(unsigned device, const unsigned char* parameters, unsigned* handle);
}
static unsigned word(const unsigned char* p, int i) { unsigned v; std::memcpy(&v, p + i * 4, 4); return v; }
int main() {
    alignas(16) unsigned char buffer[64]; unsigned char params[32]; unsigned handle = 0xdead;
    for (int i = 0; i < 32; ++i) params[i] = static_cast<unsigned char>(0x41 + i);
    norrath_lgaud_request_buffer_view = buffer;
    norrath_lgaud_semaphore_view = -1;
    if (lgAudOpen(3, params, &handle) != 0x8000000au || prints != 0 || waits != 0) return 1;
    norrath_lgaud_semaphore_view = 5;
    if (lgAudOpen(3, params, nullptr) != 0x80000004u || std::strcmp(printed[0], "lgAudOpen: invalid parameter (device)\n") != 0 || waits != 0) return 2;
    if (lgAudOpen(3, nullptr, &handle) != 0x80000004u || std::strcmp(printed[1], "lgAudOpen: invalid parameter (openparam)\n") != 0) return 3;
    params[0] = 0;
    if (lgAudOpen(3, params, &handle) != 0x80000004u || std::strcmp(printed[2], "lgAudOpen: invalid parameter (openparam->Mode)\n") != 0 || waits != 0 || rpc_calls != 0) return 4;
    params[0] = 0x41;
    std::memset(buffer, 0x5a, sizeof buffer);
    order_n = 0;
    if (lgAudOpen(7, params, &handle) != 0x600d || handle != 0x4242) return 5;
    if (order_n != 3 || order[0] != 2 || order[1] != 4 || order[2] != 3 || wait_id != 5 || sig_id != 5) return 6;
    if (rpc_client != norrath_lgaud_rpc_client_view || rpc_cmd != 2 || rpc_mode != 0 || rpc_send != buffer || rpc_recv != buffer ||
        rpc_ss != 48 || rpc_rs != 32 || rpc_end != nullptr || rpc_param != nullptr) return 7;
    if (word(buffer_at_call, 2) != 7) return 8;                                              // device in word 2
    if (std::memcmp(buffer_at_call + 32, params, 14) != 0 || buffer_at_call[46] != 0x5a) return 9;   // exactly 14 bytes
    if (buffer_at_call[0] != 0x5a || word(buffer_at_call, 1) != 0x5a5a5a5a || word(buffer_at_call, 3) != 0x5a5a5a5a) return 10;
    // failure of the call: text, semaphore given back, 0x80000000, handle untouched
    handle = 0xdead; rpc_result = 1; order_n = 0;
    if (lgAudOpen(7, params, &handle) != 0x80000000u || handle != 0xdead || order_n != 4 || order[2] != 1 || order[3] != 3) return 11;
    if (std::strcmp(printed[(prints - 1) & 3], "lgAudOpen: rpc call failed\n") != 0) return 12;
    rpc_result = -1;                                                                         // any nonzero result fails
    if (lgAudOpen(7, params, &handle) != 0x80000000u) return 13;
    return 0;
}
