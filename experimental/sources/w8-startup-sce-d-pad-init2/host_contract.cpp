#include "pad_init2.cpp"

#include <cstdio>

unsigned char norrath_libpad_rpc_clients_view[80];
int norrath_libpad_rpc_buffer_view[32];
int norrath_libpad_port_info_view[60];
static int rpc_result, calls;
static void *g_client, *g_send, *g_recv, *g_done, *g_arg;
static unsigned g_cmd, g_mode;
static int g_ssize, g_rsize;
static int info_at_call[60];

extern "C" int sceSifCallRpc(void* client, unsigned command, unsigned mode, void* send, int send_size,
                             void* receive, int receive_size, void* done, void* done_arg) {
    g_client = client; g_cmd = command; g_mode = mode; g_send = send; g_ssize = send_size;
    g_recv = receive; g_rsize = receive_size; g_done = done; g_arg = done_arg;
    for (int i = 0; i < 60; ++i) info_at_call[i] = norrath_libpad_port_info_view[i];
    calls += 1;
    return rpc_result;
}

int main() {
    for (int i = 0; i < 60; ++i) norrath_libpad_port_info_view[i] = 1000 + i;
    for (int i = 0; i < 32; ++i) norrath_libpad_rpc_buffer_view[i] = 500 + i;
    norrath_libpad_rpc_buffer_view[3] = 6;
    rpc_result = 0;
    if (scePadInit2(9) != 6 || calls != 1) return 1;
    if (g_client != norrath_libpad_rpc_clients_view || g_cmd != 1 || g_mode != 0) return 2;
    if (g_send != norrath_libpad_rpc_buffer_view || g_recv != g_send || g_ssize != 128 || g_rsize != 128 || g_done || g_arg)
        return 3;
    if (norrath_libpad_rpc_buffer_view[0] != 16 || norrath_libpad_rpc_buffer_view[4] != 0) return 4;
    if (norrath_libpad_rpc_buffer_view[1] != 501 || norrath_libpad_rpc_buffer_view[2] != 502) return 5;
    /* exactly words 16,20,24 of each 28-byte slot, in both 112-byte port halves, were cleared before the call */
    for (int i = 0; i < 60; ++i) {
        bool cleared = false;
        for (int slot = 0; slot < 4; ++slot)
            for (int port = 0; port < 2; ++port) {
                const int base = (slot * 28 + port * 112) / 4;
                if (i == base + 4 || i == base + 5 || i == base + 6) cleared = true;
            }
        if (cleared != (info_at_call[i] == 0)) return 6;
        if (!cleared && info_at_call[i] != 1000 + i) return 7;
    }
    rpc_result = -2;
    if (scePadInit2(0) != 0) return 8;
    std::puts("pad init2 contract passed");
    return 0;
}
