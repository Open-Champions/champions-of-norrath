#include "query.cpp"

#include <cstdio>
#include <cstring>

static int calls;
static void* seen_client;
static int seen_operation;
static int seen_mode;
static void* seen_send;
static int seen_send_size;
static void* seen_receive;
static int seen_receive_size;
static void* seen_completion;
static void* seen_completion_argument;
static int call_result;
static int planted_word;

extern "C" int sceSifCallRpc(void* client, int operation, int mode, void* send, int send_size,
                             void* receive, int receive_size, void* completion,
                             void* completion_argument) {
    ++calls;
    seen_client = client;
    seen_operation = operation;
    seen_mode = mode;
    seen_send = send;
    seen_send_size = send_size;
    seen_receive = receive;
    seen_receive_size = receive_size;
    seen_completion = completion;
    seen_completion_argument = completion_argument;
    if (receive != 0 && receive_size >= 4)
        std::memcpy(receive, &planted_word, 4);
    return call_result;
}

static void reset(int bind_word, int rpc_result, int word) {
    _bind = bind_word;
    call_result = rpc_result;
    planted_word = word;
    calls = 0;
    seen_client = 0;
    seen_operation = -1;
    seen_mode = -1;
    seen_send = reinterpret_cast<void*>(1);
    seen_send_size = -1;
    seen_receive = 0;
    seen_receive_size = -1;
    seen_completion = reinterpret_cast<void*>(1);
    seen_completion_argument = reinterpret_cast<void*>(1);
}

static int expect_call() {
    if (calls != 1 || seen_client != cd || seen_operation != 6 || seen_mode != 0) return 1;
    if (seen_send != 0 || seen_send_size != 0 || seen_receive != &rdata) return 2;
    if (seen_receive_size != 4 || seen_completion != 0 || seen_completion_argument != 0) return 3;
    return 0;
}

int main() {
    if (_bind != -1) return 10;
    reset(-1, 0, 99);
    if (sceSifQueryMaxFreeMemSize() != 0 || calls != 0) return 11;
    reset(-2147483647 - 1, 0, 99);
    if (sceSifQueryMaxFreeMemSize() != 0 || calls != 0) return 12;
    reset(0, -1, 99);
    if (sceSifQueryMaxFreeMemSize() != -1 || expect_call() != 0) return 13;
    reset(0, -2147483647 - 1, 99);
    if (sceSifQueryMaxFreeMemSize() != -1 || expect_call() != 0) return 14;
    reset(0, 0, 0x12345678);
    if (sceSifQueryMaxFreeMemSize() != 0x12345678 || expect_call() != 0) return 15;
    reset(1, 4, -50);
    if (sceSifQueryMaxFreeMemSize() != -50 || expect_call() != 0) return 16;
    reset(2147483647, -1, 7);
    if (sceSifQueryMaxFreeMemSize() != -1 || expect_call() != 0) return 17;
    std::puts("max free query passed");
    return 0;
}
