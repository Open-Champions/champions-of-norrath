// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_contract.cpp pad_set_button_info.cpp
extern "C" {
unsigned char norrath_libpad_rpc_clients_view[64];
int norrath_libpad_rpc_buffer_view[32];
static int rpc_result, reply4, rpc_calls, req_calls, req_port, req_slot, req_state, req_reply4;
static int req_saw_words[4];
int sceSifCallRpc(void* client, unsigned command, unsigned mode, void* send, int send_size, void* receive,
                  int receive_size, void* done, void* done_arg) {
    ++rpc_calls;
    if (client != norrath_libpad_rpc_clients_view || command != 1 || mode != 0 ||
        send != norrath_libpad_rpc_buffer_view || receive != norrath_libpad_rpc_buffer_view ||
        send_size != 128 || receive_size != 128 || done || done_arg) return -99;
    norrath_libpad_rpc_buffer_view[4] = reply4;
    return rpc_result;
}
int scePadSetReqState(int port, int slot, int state) {
    ++req_calls; req_port = port; req_slot = slot; req_state = state;
    for (int i = 0; i < 4; ++i) req_saw_words[i] = norrath_libpad_rpc_buffer_view[i];
    norrath_libpad_rpc_buffer_view[4] = req_reply4;
    return 1;
}
int scePadSetButtonInfo(int port, int slot, unsigned mask);
}
int main() {
    rpc_result = -1; reply4 = 1;
    if (scePadSetButtonInfo(1, 0, 0xfff) != 0 || req_calls != 0) return 1;
    rpc_result = 0; reply4 = 7;
    if (scePadSetButtonInfo(1, 0, 0xfff) != 7 || req_calls != 0) return 2;
    if (norrath_libpad_rpc_buffer_view[0] != 10 || norrath_libpad_rpc_buffer_view[1] != 1 ||
        norrath_libpad_rpc_buffer_view[2] != 0 || norrath_libpad_rpc_buffer_view[3] != 0xfff) return 3;
    rpc_result = 5; reply4 = 0;                                  // any non-negative result continues
    if (scePadSetButtonInfo(0, 1, 0x80000000u) != 0 || req_calls != 0) return 4;
    if (norrath_libpad_rpc_buffer_view[1] != 0 || norrath_libpad_rpc_buffer_view[2] != 1 ||
        static_cast<unsigned>(norrath_libpad_rpc_buffer_view[3]) != 0x80000000u) return 5;
    reply4 = 1; req_reply4 = 9;
    if (scePadSetButtonInfo(1, 1, 0xfff) != 9 || req_calls != 1) return 6;   // reply word is read again after the request
    if (req_port != 1 || req_slot != 1 || req_state != 2) return 7;
    if (req_saw_words[0] != 10 || req_saw_words[3] != 0xfff) return 8;
    reply4 = -1;
    if (scePadSetButtonInfo(1, 1, 0xfff) != -1 || req_calls != 1) return 9;
    return 0;
}
