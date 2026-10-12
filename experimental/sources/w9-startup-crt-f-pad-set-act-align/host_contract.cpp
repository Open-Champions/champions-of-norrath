// Host check only; not an EE build input.
extern "C" {
unsigned char norrath_libpad_rpc_clients_view[64];
int norrath_libpad_rpc_buffer_view[32];
static int rpc_result, reply5, req_calls, req_port, req_slot, req_state, set_reply5_in_req;
int sceSifCallRpc(void* client, unsigned command, unsigned mode, void* send, int send_size,
                  void* receive, int receive_size, void* done, void* done_arg) {
    if (client != norrath_libpad_rpc_clients_view || command != 1 || mode != 0 ||
        send != norrath_libpad_rpc_buffer_view || receive != norrath_libpad_rpc_buffer_view ||
        send_size != 128 || receive_size != 128 || done || done_arg) return -99;
    norrath_libpad_rpc_buffer_view[5] = reply5;
    return rpc_result;
}
void scePadSetReqState(int port, int slot, int state) {
    ++req_calls; req_port = port; req_slot = slot; req_state = state;
    norrath_libpad_rpc_buffer_view[5] = set_reply5_in_req;
}
int scePadSetActAlign(int port, int slot, const unsigned char* data);
}
int main() {
    const unsigned char data[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    rpc_result = -1; reply5 = 1;
    if (scePadSetActAlign(1, 2, data) != 0 || req_calls != 0) return 1;
    rpc_result = 0; reply5 = 5;
    if (scePadSetActAlign(1, 2, data) != 5 || req_calls != 0) return 2;
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(norrath_libpad_rpc_buffer_view);
    if (norrath_libpad_rpc_buffer_view[0] != 8 || norrath_libpad_rpc_buffer_view[1] != 1 ||
        norrath_libpad_rpc_buffer_view[2] != 2) return 3;
    for (int i = 0; i < 6; ++i) if (bytes[12 + i] != data[i]) return 4;
    if (bytes[18] != 0) return 5;                                          // only six bytes are copied
    reply5 = 1; set_reply5_in_req = 9;
    if (scePadSetActAlign(0, 1, data) != 9 || req_calls != 1) return 6;
    if (req_port != 0 || req_slot != 1 || req_state != 2) return 7;
    return 0;
}
