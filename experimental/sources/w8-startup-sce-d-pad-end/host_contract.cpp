#include "pad_end.cpp"

#include <cstdio>

unsigned char norrath_libpad_rpc_clients_view[80];
int norrath_libpad_rpc_buffer_view[32];
int norrath_libpad_is_init_word[1];
static int rpc_result, calls;
static void *g_client, *g_send, *g_recv, *g_done, *g_arg;
static unsigned g_cmd, g_mode;
static int g_ssize, g_rsize;
extern "C" int sceSifCallRpc(void* client, unsigned command, unsigned mode, void* send, int send_size,
                             void* receive, int receive_size, void* done, void* done_arg) {
    g_client = client; g_cmd = command; g_mode = mode; g_send = send; g_ssize = send_size;
    g_recv = receive; g_rsize = receive_size; g_done = done; g_arg = done_arg;
    calls += 1;
    return rpc_result;
}
static bool call_shape_ok() {
    return calls == 1 && g_client == norrath_libpad_rpc_clients_view && g_cmd == 1 && g_mode == 0 &&
           g_send == norrath_libpad_rpc_buffer_view && g_recv == g_send && g_ssize == 128 && g_rsize == 128 &&
           !g_done && !g_arg;
}

int main() {
    norrath_libpad_is_init_word[0] = 1;
    norrath_libpad_rpc_buffer_view[3] = 1;
    rpc_result = 0;
    if (scePadEnd() != 1 || !call_shape_ok() || norrath_libpad_rpc_buffer_view[0] != 15) return 1;
    if (norrath_libpad_is_init_word[0] != 0) return 2;
    calls = 0; norrath_libpad_is_init_word[0] = 1; norrath_libpad_rpc_buffer_view[3] = 0;
    if (scePadEnd() != 0 || norrath_libpad_is_init_word[0] != 1) return 3; /* only a result of 1 clears the flag */
    calls = 0; norrath_libpad_rpc_buffer_view[3] = 7;
    if (scePadEnd() != 7 || norrath_libpad_is_init_word[0] != 1) return 4;
    calls = 0; rpc_result = -1; norrath_libpad_rpc_buffer_view[3] = 1;
    if (scePadEnd() != 0 || norrath_libpad_is_init_word[0] != 1) return 5; /* failed call leaves the flag */
    std::puts("pad end contract passed");
    return 0;
}
