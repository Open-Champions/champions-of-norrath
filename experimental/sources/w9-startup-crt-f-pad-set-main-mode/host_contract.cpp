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
int scePadSetMainMode(int port, int slot, int mode, int lock);
}
int main() {
    rpc_result = -1; reply5 = 1;
    if (scePadSetMainMode(1, 2, 3, 4) != 0 || req_calls != 0) return 1;
    rpc_result = 0; reply5 = 5;
    if (scePadSetMainMode(1, 2, 3, 4) != 5 || req_calls != 0) return 2;
    if (norrath_libpad_rpc_buffer_view[0] != 6 || norrath_libpad_rpc_buffer_view[1] != 1 ||
        norrath_libpad_rpc_buffer_view[2] != 2 || norrath_libpad_rpc_buffer_view[3] != 3 ||
        norrath_libpad_rpc_buffer_view[4] != 4) return 3;
    reply5 = 1; set_reply5_in_req = 9;
    if (scePadSetMainMode(0, 1, 2, 3) != 9 || req_calls != 1) return 4;      // re-read after the call
    if (req_port != 0 || req_slot != 1 || req_state != 2) return 5;
    return 0;
}
