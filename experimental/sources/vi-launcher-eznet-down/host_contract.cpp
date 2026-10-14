#define printf norrath_host_printf
#include "down_interface.cpp"
#undef printf

#include <cstdarg>
#include <cstdio>
#include <cstring>

static int rpc_calls;
static int printf_calls;
static const char* printf_text;
static void* seen_client;
static void* seen_send;
static void* seen_receive;
static int seen_operation;
static int seen_mode;
static int seen_send_size;
static int seen_receive_size;
static void* seen_completion;
static void* seen_completion_argument;

extern "C" int m_rpcRunning = 0;

extern "C" int norrath_host_printf(const char* text, ...) {
    ++printf_calls;
    printf_text = text;
    return 0;
}

extern "C" int sceSifCallRpc(void* client, int operation, int mode, void* send, int send_size,
                             void* receive, int receive_size, void* completion,
                             void* completion_argument) {
    ++rpc_calls;
    seen_client = client;
    seen_operation = operation;
    seen_mode = mode;
    seen_send = send;
    seen_send_size = send_size;
    seen_receive = receive;
    seen_receive_size = receive_size;
    seen_completion = completion;
    seen_completion_argument = completion_argument;
    return -7;
}

int main() {
    if (sizeof m_cd != 64 || sizeof m_buf != 320) return 1;
    m_rpcRunning = 0;
    *reinterpret_cast<int*>(m_buf) = 99;
    if (ezNetCtlDownInterface(5) != -1) return 2;
    if (rpc_calls != 0 || printf_calls != 1) return 3;
    if (std::strcmp(printf_text, "**** eznetctl RPC server not currently running\n") != 0) return 4;
    if (*reinterpret_cast<int*>(m_buf) != 99) return 5;
    m_rpcRunning = 1;
    if (ezNetCtlDownInterface(5) != 0) return 6;
    if (rpc_calls != 1 || printf_calls != 1) return 7;
    if (seen_client != m_cd || seen_send != m_buf || seen_receive != m_buf) return 8;
    if (seen_operation != 6 || seen_mode != 0 || seen_send_size != 4 || seen_receive_size != 4)
        return 9;
    if (seen_completion != nullptr || seen_completion_argument != nullptr) return 10;
    if (*reinterpret_cast<int*>(m_buf) != 5) return 11;
    return 0;
}
