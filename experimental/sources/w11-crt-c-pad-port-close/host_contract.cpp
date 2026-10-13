// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_contract.cpp pad_port_close.cpp
extern "C" {
unsigned char norrath_libpad_port_info_view[224];
unsigned char norrath_libpad_rpc_clients_view[80];
int norrath_libpad_rpc_buffer_view[32];
static int rpc_result, calls, reply;
static void *g_client, *g_send, *g_recv, *g_done, *g_arg;
static unsigned g_cmd, g_mode;
static int g_ssize, g_rsize, g_w[5];
int sceSifCallRpc(void* client, unsigned command, unsigned mode, void* send, int send_size, void* receive,
                  int receive_size, void* done, void* done_arg) {
    g_client = client; g_cmd = command; g_mode = mode; g_send = send; g_ssize = send_size;
    g_recv = receive; g_rsize = receive_size; g_done = done; g_arg = done_arg;
    for (int i = 0; i < 5; ++i) g_w[i] = norrath_libpad_rpc_buffer_view[i];  // what the server sees
    norrath_libpad_rpc_buffer_view[3] = reply;
    ++calls;
    return rpc_result;
}
int scePadPortClose(int port, int slot);
}
static unsigned& open_word(int port, int slot) {
    return reinterpret_cast<unsigned*>(norrath_libpad_port_info_view)[(port * 112 + slot * 28 + 16) / 4];
}
int main() {
    open_word(1, 0) = 1; open_word(0, 1) = 1;
    if (scePadPortClose(1, 1) != 0 || calls != 0 || norrath_libpad_rpc_buffer_view[0] != 0) return 1;  // closed: nothing
    open_word(1, 1) = 1;
    rpc_result = 0; reply = 77;
    if (scePadPortClose(1, 1) != 77 || calls != 1) return 2;
    if (g_client != norrath_libpad_rpc_clients_view || g_cmd != 1 || g_mode != 0 || g_send != norrath_libpad_rpc_buffer_view ||
        g_recv != g_send || g_ssize != 128 || g_rsize != 128 || g_done || g_arg) return 3;
    if (g_w[0] != 14 || g_w[1] != 1 || g_w[2] != 1 || g_w[4] != 1) return 4;
    if (open_word(1, 1) != 0 || open_word(1, 0) != 1 || open_word(0, 1) != 1) return 5;  // only its own word cleared
    if (scePadPortClose(1, 1) != 0 || calls != 1) return 6;                                // now closed again
    open_word(0, 1) = 1; rpc_result = -3; reply = 55;
    if (scePadPortClose(0, 1) != 0 || calls != 2 || open_word(0, 1) != 1) return 7;        // failure keeps it open
    if (g_w[1] != 0 || g_w[2] != 1) return 8;
    rpc_result = 5; reply = -9;
    if (scePadPortClose(0, 1) != -9 || open_word(0, 1) != 0) return 9;
    return 0;
}
