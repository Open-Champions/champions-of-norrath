#include "flush_iop.cpp"

#include <cstdio>

int norrath_cdstream_client[16], norrath_cdstream_send[16], norrath_cdstream_recv[16];
static int check_calls, call_calls, check_result, call_result;
static unsigned seen_function, seen_mode;
static int seen_ssize, seen_rsize;
static void *seen_client, *seen_send, *seen_recv, *seen_end, *seen_end_arg;

int sceSifCheckStatRpc(void* client) {
    ++check_calls;
    seen_client = client;
    return check_result;
}
int sceSifCallRpc(void* client, unsigned function, unsigned mode, void* send, int send_size,
                  void* receive, int receive_size, void* end_function, void* end_argument) {
    ++call_calls;
    if (client != seen_client) return -99;
    seen_function = function; seen_mode = mode; seen_send = send; seen_ssize = send_size;
    seen_recv = receive; seen_rsize = receive_size; seen_end = end_function; seen_end_arg = end_argument;
    return call_result;
}

int main() {
    check_result = 1;
    call_result = 5;
    if (FlushIop__Fi(0) != -1 || check_calls != 1 || call_calls != 0) return 1;
    check_result = 0;
    if (FlushIop__Fi(0) != 0 || call_calls != 1) return 2;
    if (seen_client != norrath_cdstream_client || seen_function != 0x80f0 || seen_mode != 0) return 3;
    if (seen_send != norrath_cdstream_send || seen_ssize != 64) return 4;
    if (seen_recv != norrath_cdstream_recv || seen_rsize != 64 || seen_end != nullptr || seen_end_arg != nullptr) return 5;
    call_result = -7;
    if (FlushIop__Fi(9) != 0 || seen_mode != 1) return 6;
    if (FlushIop__Fi(-1) != 0 || seen_mode != 1) return 7;
    std::puts("FlushIop contract passed");
    return 0;
}
