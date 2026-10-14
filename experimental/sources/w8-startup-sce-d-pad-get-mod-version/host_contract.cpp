#include "pad_get_mod_version.cpp"

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
    norrath_libpad_rpc_buffer_view[3] = 0x040e;
    norrath_libpad_rpc_buffer_view[1] = 55;
    rpc_result = 0;
    if (scePadGetModVersion() != 0x040e || !call_shape_ok()) return 1;
    if (norrath_libpad_rpc_buffer_view[0] != 18 || norrath_libpad_rpc_buffer_view[1] != 55) return 2;
    calls = 0; rpc_result = -1;
    if (scePadGetModVersion() != 0) return 3;
    std::puts("pad get mod version contract passed");
    return 0;
}
