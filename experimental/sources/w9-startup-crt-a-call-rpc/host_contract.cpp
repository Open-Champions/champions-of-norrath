#include "call_rpc.cpp"

#include <cstdarg>
#include <cstdio>
#include <cstring>

static char printed[2][96];
static int print_count;
static int rpc_calls;
static void* seen_object;
static int seen_function;
static int seen_mode;
static void* seen_send;
static int seen_send_size;
static void* seen_receive;
static int seen_receive_size;
static void (*seen_end)(void*) = reinterpret_cast<void (*)(void*)>(1);
static void* seen_end_argument = reinterpret_cast<void*>(1);

extern "C" int scePrintf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    std::vsnprintf(printed[print_count < 2 ? print_count : 1], sizeof printed[0], format, args);
    va_end(args);
    ++print_count;
    return 0;
}

extern "C" int sceSifMCallRpc(void* object, int function, int mode, void* send, int send_size,
                              void* receive, int receive_size, void (*end_function)(void*), void* end_argument) {
    ++rpc_calls;
    seen_object = object;
    seen_function = function;
    seen_mode = mode;
    seen_send = send;
    seen_send_size = send_size;
    seen_receive = receive;
    seen_receive_size = receive_size;
    seen_end = end_function;
    seen_end_argument = end_argument;
    return 77;
}

int main() {
    alignas(4) unsigned char object[64] = {};
    unsigned char buffer[8];
    *reinterpret_cast<int*>(object + 48) = 128;

    if (CallRpc(object, 9, buffer, 1, 64) != 77 || rpc_calls != 1 || print_count != 0) return 1;
    if (seen_object != object || seen_function != 9 || seen_mode != 0) return 2;
    if (seen_send != buffer || seen_receive != buffer) return 3;
    if (seen_send_size != 64 || seen_receive_size != 64) return 4;
    if (seen_end != nullptr || seen_end_argument != nullptr) return 5;

    // Exactly at the limit is accepted; sizes round up to multiples of 64.
    if (CallRpc(object, 3, buffer, 65, 128) != 77 || seen_send_size != 128 || seen_receive_size != 128) return 6;
    if (CallRpc(object, 3, buffer, 0, 0) != 77 || seen_send_size != 0 || seen_receive_size != 0) return 7;
    if (rpc_calls != 3) return 8;

    // Either size over the limit prints both lines and does not call the service.
    if (CallRpc(object, 5, buffer, 129, 10) != -543 || rpc_calls != 3 || print_count != 2) return 9;
    if (std::strcmp(printed[0], "[fatal error] insufficient buffer size in Libnet\n") != 0) return 10;
    if (std::strcmp(printed[1], "(fno=5, ssize=192, rsize=64, buffer=128)\n") != 0) return 11;
    print_count = 0;
    if (CallRpc(object, 6, buffer, 10, 200) != -543 || rpc_calls != 3 || print_count != 2) return 12;
    if (std::strcmp(printed[1], "(fno=6, ssize=64, rsize=256, buffer=128)\n") != 0) return 13;
    std::puts("call rpc contract passed");
    return 0;
}
